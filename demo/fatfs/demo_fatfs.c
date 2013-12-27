/*----------------------------------------------------------------------*/
/* FAT file system sample project for FatFs            (C)ChaN, 2010    */
/*----------------------------------------------------------------------*/
/* _FS_MINIMIZE must be 0 ! */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <string.h>
#include <fatfs/ff.h>
#include <fatfs/diskio.h>
#include "uart.h"
#include "xitoa.h"
#include "rtc.h"
#include "sound.h"


FUSES = {0xAF, 0xC3, 0xFF};	/* Fuse values: Low, High, Ext */
/* This is the fuse settings for this project. The fuse data will be output into the
   hex file with program code. However some flash programmers may not support this
   sort of hex files. If it is the case, use these values to program the fuse bits.
*/


DWORD AccSize;				/* Work register for fs command */
WORD AccFiles, AccDirs;
FILINFO Finfo;
#if _USE_LFN
char Lfname[_MAX_LFN+1];
#endif


char Line[100];				/* Console input buffer */

FATFS Fatfs[_VOLUMES];		/* File system object for each logical drive */
FIL File[2];				/* File object */

BYTE Buff[2048];			/* Working buffer */

volatile WORD Timer;		/* 100Hz increment timer */



#if _MULTI_PARTITION != 0
const PARTITION Drives[] = { {0,0}, {0,1} };
#endif

/*---------------------------------------------------------*/
/* 100Hz timer interrupt generated by OC2                  */
/*---------------------------------------------------------*/

void disk_timerproc (void);
ISR(TIMER2_COMP_vect)
{
	Timer++;			/* Performance counter for this module */
	disk_timerproc();	/* Drive timer procedure of low level disk I/O module */
}



/*---------------------------------------------------------*/
/* User Provided Timer Function for FatFs module           */
/*---------------------------------------------------------*/
/* This is a real time clock service to be called from     */
/* FatFs module. Any valid time must be returned even if   */
/* the system does not support a real time clock.          */
/* This is not required in read-only configuration.        */


DWORD get_fattime (void)
{
#ifdef RTC_DEFINED
	RTC rtc;

	/* Get local time */
	rtc_gettime(&rtc);

	/* Pack date and time into a DWORD variable */
	return	  ((DWORD)(rtc.year - 1980) << 25)
			| ((DWORD)rtc.month << 21)
			| ((DWORD)rtc.mday << 16)
			| ((DWORD)rtc.hour << 11)
			| ((DWORD)rtc.min << 5)
			| ((DWORD)rtc.sec >> 1);
#else
	return	0;
#endif
}


/*--------------------------------------------------------------------------*/
/* Monitor                                                                  */


static
void put_dump (const BYTE *buff, DWORD ofs, BYTE cnt)
{
	BYTE i;


	xprintf(PSTR("%08lX "), ofs);

	for(i = 0; i < cnt; i++)
		xprintf(PSTR(" %02X"), buff[i]);

	xputc(' ');
	for(i = 0; i < cnt; i++)
		xputc((buff[i] >= ' ' && buff[i] <= '~') ? buff[i] : '.');

	xputc('\n');
}


static
void get_line (char *buff, int len)
{
	BYTE c;
	int i = 0;


	for (;;) {
		c = uart_get();
		if (c == '\r') break;
		if ((c == '\b') && i) {
			i--;
			uart_put(c);
			continue;
		}
		if (c >= ' ' && i < len - 1) {	/* Visible chars */
			buff[i++] = c;
			xputc(c);
		}
	}
	buff[i] = 0;
	uart_put('\n');
}


static
FRESULT scan_files (
	char* path		/* Pointer to the working buffer with start path */
)
{
	DIR dirs;
	FRESULT res;
	int i;
	char *fn;

	res = f_opendir(&dirs, path);
	if (res == FR_OK) {
		i = strlen(path);
		while (((res = f_readdir(&dirs, &Finfo)) == FR_OK) && Finfo.fname[0]) {
			if (_FS_RPATH && Finfo.fname[0] == '.') continue;
#if _USE_LFN
			fn = *Finfo.lfname ? Finfo.lfname : Finfo.fname;
#else
			fn = Finfo.fname;
#endif
			if (Finfo.fattrib & AM_DIR) {
				AccDirs++;
				*(path+i) = '/'; strcpy(path+i+1, fn);
				res = scan_files(path);
				*(path+i) = '\0';
				if (res != FR_OK) break;
			} else {
//				xprintf(PSTR("%s/%s\n"), path, fn);
				AccFiles++;
				AccSize += Finfo.fsize;
			}
		}
	}

	return res;
}



static
void put_rc (FRESULT rc)
{
	const prog_char *p;
	static const prog_char str[] =
		"OK\0" "DISK_ERR\0" "INT_ERR\0" "NOT_READY\0" "NO_FILE\0" "NO_PATH\0"
		"INVALID_NAME\0" "DENIED\0" "EXIST\0" "INVALID_OBJECT\0" "WRITE_PROTECTED\0"
		"INVALID_DRIVE\0" "NOT_ENABLED\0" "NO_FILE_SYSTEM\0" "MKFS_ABORTED\0" "TIMEOUT\0"
		"LOCKED\0" "NOT_ENOUGH_CORE\0" "TOO_MANY_OPEN_FILES\0";
	FRESULT i;

	for (p = str, i = 0; i != rc && pgm_read_byte_near(p); i++) {
		while(pgm_read_byte_near(p++));
	}
	xprintf(PSTR("rc=%u FR_%S\n"), rc, p);
}




static
void IoInit (void)
{
	PORTA = 0b11111111;		// Port A

	PORTB = 0b11110000; 	// Port B
	DDRB  = 0b00000000;

	PORTC = 0b11111111;		// Port C

	PORTD = 0b11111111;		// Port D

	PORTE = 0b11110010; 	// Port E
	DDRE  = 0b10000010;

	PORTF = 0b11111111;		// Port F

	PORTG = 0b11111; 		// Port G

	uart_init();			// Initialize UART driver

	OCR2 = F_CPU / 1024 / 100 - 1;	// OC2: 100Hz interval timer
	TCCR2 = 0b00001101;
	TIMSK |= _BV(OCIE2);

#ifdef RTC_DEFINED
	rtc_init();				// Initialize RTC
#endif

	sei();
}



/*-----------------------------------------------------------------------*/
/* Main                                                                  */


int main (void)
{
	char *ptr, *ptr2;
	long p1, p2, p3;
	BYTE res, b1, *bp;
	UINT s1, s2, cnt;
	DWORD ofs, sect = 0;
	FATFS *fs;
	DIR dir;
#ifdef RTC_DEFINED
	RTC rtc;
#endif


	IoInit();

	xfunc_out = uart_put;
	xputs(PSTR("\nFatFs module test monitor for AVR\n"));
	xputs(_USE_LFN ? PSTR("LFN Enabled") : PSTR("LFN Disabled"));
	xprintf(PSTR(", Code page: %u\n"), _CODE_PAGE);

#if _USE_LFN
	Finfo.lfname = Lfname;
	Finfo.lfsize = sizeof Lfname;
#endif

	for (;;) {
		xputc('>');
		ptr = (char*)Line;
		get_line(ptr, sizeof Line);
		switch (*ptr++) {

		case 'd' :
			switch (*ptr++) {
			case 'd' :	/* dd <phy_drv#> [<sector>] - Dump secrtor */
				if (!xatoi(&ptr, &p1)) break;
				if (!xatoi(&ptr, &p2)) p2 = sect;
				res = disk_read((BYTE)p1, Buff, p2, 1);
				if (res) { xprintf(PSTR("rc=%d\n"), res); break; }
				sect = p2 + 1;
				xprintf(PSTR("Sector:%lu\n"), p2);
				for (bp=Buff, ofs = 0; ofs < 0x200; bp+=16, ofs+=16)
					put_dump(bp, ofs, 16);
				break;

			case 'i' :	/* di <phy_drv#> - Initialize disk */
				if (!xatoi(&ptr, &p1)) break;
				xprintf(PSTR("rc=%d\n"), disk_initialize((BYTE)p1));
				break;

			case 's' :	/* ds <phy_drv#> - Show disk status */
				if (!xatoi(&ptr, &p1)) break;
				if (disk_ioctl((BYTE)p1, GET_SECTOR_COUNT, &p2) == RES_OK)
					{ xprintf(PSTR("Drive size: %lu sectors\n"), p2); }
				if (disk_ioctl((BYTE)p1, GET_BLOCK_SIZE, &p2) == RES_OK)
					{ xprintf(PSTR("Erase block: %lu sectors\n"), p2); }
				if (disk_ioctl((BYTE)p1, MMC_GET_TYPE, &b1) == RES_OK)
					{ xprintf(PSTR("Card type: %u\n"), b1); }
				if (disk_ioctl((BYTE)p1, MMC_GET_CSD, Buff) == RES_OK)
					{ xputs(PSTR("CSD:\n")); put_dump(Buff, 0, 16); }
				if (disk_ioctl((BYTE)p1, MMC_GET_CID, Buff) == RES_OK)
					{ xputs(PSTR("CID:\n")); put_dump(Buff, 0, 16); }
				if (disk_ioctl((BYTE)p1, MMC_GET_OCR, Buff) == RES_OK)
					{ xputs(PSTR("OCR:\n")); put_dump(Buff, 0, 4); }
				if (disk_ioctl((BYTE)p1, MMC_GET_SDSTAT, Buff) == RES_OK) {
					xputs(PSTR("SD Status:\n"));
					for (s1 = 0; s1 < 64; s1 += 16) put_dump(Buff+s1, s1, 16);
				}
				if (disk_ioctl((BYTE)p1, ATA_GET_MODEL, Line) == RES_OK)
					{ Line[40] = '\0'; xprintf(PSTR("Model: %s\n"), Line); }
				if (disk_ioctl((BYTE)p1, ATA_GET_SN, Line) == RES_OK)
					{ Line[20] = '\0'; xprintf(PSTR("S/N: %s\n"), Line); }
				break;
			}
			break;

		case 'b' :
			switch (*ptr++) {
			case 'd' :	/* bd <addr> - Dump R/W buffer */
				if (!xatoi(&ptr, &p1)) break;
				for (bp=&Buff[p1], ofs = p1, cnt = 32; cnt; cnt--, ptr+=16, ofs+=16)
					put_dump(bp, ofs, 16);
				break;

			case 'e' :	/* be <addr> [<data>] ... - Edit R/W buffer */
				if (!xatoi(&ptr, &p1)) break;
				if (xatoi(&ptr, &p2)) {
					do {
						Buff[p1++] = (BYTE)p2;
					} while (xatoi(&ptr, &p2));
					break;
				}
				for (;;) {
					xprintf(PSTR("%04X %02X-"), (WORD)p1, Buff[p1]);
					get_line(Line, sizeof Line);
					ptr = Line;
					if (*ptr == '.') break;
					if (*ptr < ' ') { p1++; continue; }
					if (xatoi(&ptr, &p2))
						Buff[p1++] = (BYTE)p2;
					else
						xputs(PSTR("???\n"));
				}
				break;

			case 'r' :	/* br <phy_drv#> <sector> [<n>] - Read disk into R/W buffer */
				if (!xatoi(&ptr, &p1)) break;
				if (!xatoi(&ptr, &p2)) break;
				if (!xatoi(&ptr, &p3)) p3 = 1;
				xprintf(PSTR("rc=%u\n"), disk_read((BYTE)p1, Buff, p2, p3));
				break;

			case 'w' :	/* bw <phy_drv#> <sector> [<n>] - Write R/W buffer into disk */
				if (!xatoi(&ptr, &p1)) break;
				if (!xatoi(&ptr, &p2)) break;
				if (!xatoi(&ptr, &p3)) p3 = 1;
				xprintf(PSTR("rc=%u\n"), disk_write((BYTE)p1, Buff, p2, p3));
				break;

			case 'f' :	/* bf <n> - Fill working buffer */
				if (!xatoi(&ptr, &p1)) break;
				memset(Buff, (BYTE)p1, sizeof Buff);
				break;

			}
			break;

		case 'f' :
			switch (*ptr++) {

			case 'i' :	/* fi <log drv#> - Initialize logical drive */
				if (!xatoi(&ptr, &p1)) break;
				put_rc(f_mount((BYTE)p1, &Fatfs[p1]));
				break;

			case 's' :	/* fs [<path>] - Show logical drive status */
				while (*ptr == ' ') ptr++;
				res = f_getfree(ptr, (DWORD*)&p2, &fs);
				if (res) { put_rc(res); break; }
				xprintf(PSTR("FAT type = %u\nBytes/Cluster = %lu\nNumber of FATs = %u\n"
							 "Root DIR entries = %u\nSectors/FAT = %lu\nNumber of clusters = %lu\n"
							 "FAT start (lba) = %lu\nDIR start (lba,clustor) = %lu\nData start (lba) = %lu\n\n..."),
						fs->fs_type, (DWORD)fs->csize * 512, fs->n_fats,
						fs->n_rootdir, fs->fsize, fs->n_fatent - 2,
						fs->fatbase, fs->dirbase, fs->database
				);
				AccSize = AccFiles = AccDirs = 0;
				res = scan_files(ptr);
				if (res) { put_rc(res); break; }
				xprintf(PSTR("\r%u files, %lu bytes.\n%u folders.\n"
							 "%lu KB total disk space.\n%lu KB available.\n"),
						AccFiles, AccSize, AccDirs,
						(fs->n_fatent - 2) * (fs->csize / 2), p2 * (fs->csize / 2)
				);
				break;

			case 'l' :	/* fl [<path>] - Directory listing */
				while (*ptr == ' ') ptr++;
				res = f_opendir(&dir, ptr);
				if (res) { put_rc(res); break; }
				p1 = s1 = s2 = 0;
				for(;;) {
					res = f_readdir(&dir, &Finfo);
					if ((res != FR_OK) || !Finfo.fname[0]) break;
					if (Finfo.fattrib & AM_DIR) {
						s2++;
					} else {
						s1++; p1 += Finfo.fsize;
					}
					xprintf(PSTR("%c%c%c%c%c %u/%02u/%02u %02u:%02u %9lu  %s"), 
								(Finfo.fattrib & AM_DIR) ? 'D' : '-',
								(Finfo.fattrib & AM_RDO) ? 'R' : '-',
								(Finfo.fattrib & AM_HID) ? 'H' : '-',
								(Finfo.fattrib & AM_SYS) ? 'S' : '-',
								(Finfo.fattrib & AM_ARC) ? 'A' : '-',
								(Finfo.fdate >> 9) + 1980, (Finfo.fdate >> 5) & 15, Finfo.fdate & 31,
								(Finfo.ftime >> 11), (Finfo.ftime >> 5) & 63,
								Finfo.fsize, &(Finfo.fname[0]));
#if _USE_LFN
					for (p2 = strlen(Finfo.fname); p2 < 14; p2++)
						xputc(' ');
					xprintf(PSTR("%s\n"), Lfname);
#else
					xputc('\n');
#endif
				}
				xprintf(PSTR("%4u File(s),%10lu bytes total\n%4u Dir(s)"), s1, p1, s2);
				if (f_getfree(ptr, (DWORD*)&p1, &fs) == FR_OK)
					xprintf(PSTR(", %10luK bytes free\n"), p1 * fs->csize / 2);
				break;

			case 'o' :	/* fo <mode> <name> - Open a file */
				if (!xatoi(&ptr, &p1)) break;
				while (*ptr == ' ') ptr++;
				put_rc(f_open(&File[0], ptr, (BYTE)p1));
				break;

			case 'c' :	/* fc - Close a file */
				put_rc(f_close(&File[0]));
				break;

			case 'e' :	/* fe - Seek file pointer */
				if (!xatoi(&ptr, &p1)) break;
				res = f_lseek(&File[0], p1);
				put_rc(res);
				if (res == FR_OK)
					xprintf(PSTR("fptr = %lu(0x%lX)\n"), File[0].fptr, File[0].fptr);
				break;

			case 'r' :	/* fr <len> - read file */
				if (!xatoi(&ptr, &p1)) break;
				p2 = 0;
				Timer = 0;
				while (p1) {
					if (p1 >= sizeof Buff)	{ cnt = sizeof Buff; p1 -= sizeof Buff; }
					else 			{ cnt = (WORD)p1; p1 = 0; }
					res = f_read(&File[0], Buff, cnt, &s2);
					if (res != FR_OK) { put_rc(res); break; }
					p2 += s2;
					if (cnt != s2) break;
				}
				s2 = Timer;
				xprintf(PSTR("%lu bytes read with %lu bytes/sec.\n"), p2, s2 ? (p2 * 100 / s2) : 0);
				break;

			case 'd' :	/* fd <len> - read and dump file from current fp */
				if (!xatoi(&ptr, &p1)) break;
				ofs = File[0].fptr;
				while (p1) {
					if (p1 >= 16)	{ cnt = 16; p1 -= 16; }
					else 			{ cnt = (WORD)p1; p1 = 0; }
					res = f_read(&File[0], Buff, cnt, &cnt);
					if (res != FR_OK) { put_rc(res); break; }
					if (!cnt) break;
					put_dump(Buff, ofs, cnt);
					ofs += 16;
				}
				break;

			case 'w' :	/* fw <len> <val> - write file */
				if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2)) break;
				memset(Buff, (BYTE)p2, sizeof Buff);
				p2 = 0;
				Timer = 0;
				while (p1) {
					if (p1 >= sizeof Buff)	{ cnt = sizeof Buff; p1 -= sizeof Buff; }
					else 			{ cnt = (WORD)p1; p1 = 0; }
					res = f_write(&File[0], Buff, cnt, &s2);
					if (res != FR_OK) { put_rc(res); break; }
					p2 += s2;
					if (cnt != s2) break;
				}
				s2 = Timer;
				xprintf(PSTR("%lu bytes written with %lu bytes/sec.\n"), p2, s2 ? (p2 * 100 / s2) : 0);
				break;

			case 'v' :	/* fv - Truncate file */
				put_rc(f_truncate(&File[0]));
				break;

			case 'n' :	/* fn <old_name> <new_name> - Change file/dir name */
				while (*ptr == ' ') ptr++;
				ptr2 = strchr(ptr, ' ');
				if (!ptr2) break;
				*ptr2++ = 0;
				while (*ptr2 == ' ') ptr2++;
				put_rc(f_rename(ptr, ptr2));
				break;

			case 'u' :	/* fu <name> - Unlink a file or dir */
				while (*ptr == ' ') ptr++;
				put_rc(f_unlink(ptr));
				break;

			case 'k' :	/* fk <name> - Create a directory */
				while (*ptr == ' ') ptr++;
				put_rc(f_mkdir(ptr));
				break;

			case 'a' :	/* fa <atrr> <mask> <name> - Change file/dir attribute */
				if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2)) break;
				while (*ptr == ' ') ptr++;
				put_rc(f_chmod(ptr, p1, p2));
				break;

			case 't' :	/* ft <year> <month> <day> <hour> <min> <sec> <name> */
				if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2) || !xatoi(&ptr, &p3)) break;
				Finfo.fdate = ((p1 - 1980) << 9) | ((p2 & 15) << 5) | (p3 & 31);
				if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2) || !xatoi(&ptr, &p3)) break;
				Finfo.ftime = ((p1 & 31) << 11) | ((p2 & 63) << 5) | ((p3 >> 1) & 31);
				while (*ptr == ' ') ptr++;
				put_rc(f_utime(ptr, &Finfo));
				break;

			case 'x' : /* fx <src_name> <dst_name> - Copy file */
				while (*ptr == ' ') ptr++;
				ptr2 = strchr(ptr, ' ');
				if (!ptr2) break;
				*ptr2++ = 0;
				while (*ptr2 == ' ') ptr2++;
				xprintf(PSTR("Opening \"%s\""), ptr);
				res = f_open(&File[0], ptr, FA_OPEN_EXISTING | FA_READ);
				if (res) {
					put_rc(res);
					break;
				}
				xprintf(PSTR("\nCreating \"%s\""), ptr2);
				res = f_open(&File[1], ptr2, FA_CREATE_ALWAYS | FA_WRITE);
				if (res) {
					put_rc(res);
					f_close(&File[0]);
					break;
				}
				xprintf(PSTR("\nCopying..."));
				p1 = 0;
				for (;;) {
					res = f_read(&File[0], Buff, sizeof Buff, &s1);
					if (res || s1 == 0) break;   /* error or eof */
					res = f_write(&File[1], Buff, s1, &s2);
					p1 += s2;
					if (res || s2 < s1) break;   /* error or disk full */
				}
				if (res) put_rc(res);
				xprintf(PSTR("\n%lu bytes copied.\n"), p1);
				f_close(&File[0]);
				f_close(&File[1]);
				break;
#if _FS_RPATH
			case 'g' :	/* fg <path> - Change current directory */
				while (*ptr == ' ') ptr++;
				put_rc(f_chdir(ptr));
				break;

			case 'j' :	/* fj <drive#> - Change current drive */
				if (xatoi(&ptr, &p1)) {
					put_rc(f_chdrive((BYTE)p1));
				}
				break;
#if _FS_RPATH >= 2
			case 'q' :	/* fq - Show current dir path */
				res = f_getcwd(Line, sizeof Line);
				if (res)
					put_rc(res);
				else
					xprintf(PSTR("%s\n"), Line);
				break;
#endif
#endif
#if _USE_MKFS
			case 'm' :	/* fm <logi drv#> <part type> <bytes/clust> - Create file system */
				if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2) || !xatoi(&ptr, &p3)) break;
				xprintf(PSTR("The drive %u will be formatted. Are you sure? (Y/n)="), (WORD)p1);
				get_line(ptr, sizeof Line);
				if (*ptr == 'Y') put_rc(f_mkfs((BYTE)p1, (BYTE)p2, (WORD)p3));
				break;
#endif
			}
			break;
#ifdef SOUND_DEFINED
		case 'p' :	/* p <wavfile> - Play RIFF-WAV file (upto data rate of 48kB/sec) */
			while (*ptr == ' ') ptr++;
			res = f_open(&File[0], ptr, FA_READ);
			if (res) {
				put_rc(res);
			} else {
				load_wav(&File[0], "WAV Player", Buff, sizeof Buff);
				f_close(&File[0]);
			}
			break;
#endif
#ifdef RTC_DEFINED
		case 't' :	/* t [<year> <mon> <mday> <hour> <min> <sec>] */
			if (xatoi(&ptr, &p1)) {
				rtc.year = (WORD)p1;
				xatoi(&ptr, &p1); rtc.month = (BYTE)p1;
				xatoi(&ptr, &p1); rtc.mday = (BYTE)p1;
				xatoi(&ptr, &p1); rtc.hour = (BYTE)p1;
				xatoi(&ptr, &p1); rtc.min = (BYTE)p1;
				if (!xatoi(&ptr, &p1)) break;
				rtc.sec = (BYTE)p1;
				rtc_settime(&rtc);
			}
			rtc_gettime(&rtc);
			xprintf(PSTR("%u/%u/%u %02u:%02u:%02u\n"), rtc.year, rtc.month, rtc.mday, rtc.hour, rtc.min, rtc.sec);
			break;
#endif
		}
	}

}

