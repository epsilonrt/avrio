/**
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
 *
 * This file is part of AvrIO.
 *
 * This software is governed by the CeCILL license under French law and
 * abiding by the rules of distribution of free software.  You can  use, 
 * modify and/ or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * <http://www.cecill.info>. 
 * 
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
 *
 * @file defs.h
 * @brief Définitions et macros communes à tous les modules.
 */
#ifndef _AVRIO_DEFS_H_
#define _AVRIO_DEFS_H_

/* constants ============================================================== */
/**
 * @addtogroup sys_group
 * @defgroup defs_module Définitions
 * @{
 */
# define ERROR_SUCCESS   0x00/**< Aucune erreur */

/* =======================Fin Codes d'erreur système======================= */
/**
 * @}
 */

/**
 * @ingroup keyb_module
 * @brief Code de la touche ENTER
 */
#define KEY_ENTER '\n'

/**
 * @ingroup keyb_module
 * @brief Code de la touche flèche vers la gauche
 */
#define KEY_LEFT  0x80

/**
 * @ingroup keyb_module
 * @brief Code de la touche flèche vers la droite
 */
#define KEY_RIGHT 0x81

/**
 * @ingroup keyb_module
 * @brief Code de la touche flèche vers le haut
 */
#define KEY_UP    0x82

/**
 * @ingroup keyb_module
 * @brief Code de la touche flèche vers le bas
 */
#define KEY_DOWN  0x83

/**
 * @ingroup keyb_module
 * @brief Code de la touche retour à l'origine
 */
#define KEY_HOME  0x84

/* macros ================================================================= */
#ifndef _BV
/**
 * @ingroup defs_module
 * @brief Renvoie un masque avec le bit de rang n à 1
 */
#define _BV(n) (1<<n)
#endif

/**
 * @ingroup delay_module
 * @brief Permet de convertir un intervalle de secondes en millisecondes
 */
#define SECONDS(s)  (1000L*s)

/**
 * @ingroup delay_module
 * @brief Permet de convertir un intervalle de minutes en millisecondes
 */
#define MINUTES(m)  (m*SECONDS(60))

/**
 * @ingroup delay_module
 * @brief Permet de convertir un intervalle de heures en millisecondes
 */
#define HOURS(h)  (h*MINUTES(60))

/**
 * @ingroup delay_module
 * @brief Permet de convertir un intervalle de jours en millisecondes
 */
#define DAYS(d)  (d*HOURS(24))

/**
 * @ingroup delay_module
 * @brief Permet de convertir un intervalle de semaines en millisecondes
 */
#define WEEKS(w)  (w*DAYS(7))

/**
 * @ingroup delay_module
 * @brief Permet de convertir un intervalle de (heures, minutes, secondes)
 *  en millisecondes
 */
#define HMS(h,m,s)  (HOURS(h)+MINUTES(m)+SECONDS(s))

/**
 * @ingroup delay_module
 * @brief Permet de convertir un intervalle de (jours, heures, minutes,
 *  secondes)  en millisecondes
 */
#define DHMS(d,h,m,s)  (DAYS(d)+HOURS(h)+MINUTES(m)+SECONDS(s))

/**
 * @ingroup delay_module
 * @brief Permet de convertir un intervalle de (semaines, jours, heures,
 *  minutes, secondes)  en millisecondes
 */
#define WDHMS(w,d,h,m,s)  (WEEKS(w)+DAYS(d)+HOURS(h)+MINUTES(m)+SECONDS(s))

/**
 * @ingroup defs_module
 * @defgroup timer16_defs Définitions pour le timer 16 bits
 * @{
 */
# define TIMER16_STOPPED (0) /**< Timer arrêté */
# define TIMER16_CLK1    (1) /**< ClkIO/1 */
# define TIMER16_CLK8    (2) /**< ClkIO/8 */
# define TIMER16_CLK64   (3) /**< ClkIO/64 */
# define TIMER16_CLK256  (4) /**< ClkIO/256 */
# define TIMER16_CLK1024 (5) /**< ClkIO/1024 */

/* =================Fin Définitions pour le timer 16 bits================== */
/**
 * @}
 */

/**
 * @ingroup keyb_module
 * @defgroup keyb_driver_defs Définitions pour la configuration du driver clavier
 * @{
 */
# define KEYB_IFACE_TYPE_MSK  0x0070
# define KEYB_IFACE_PIO       0x0000
# define KEYB_IFACE_BUS       0x0010
# define KEYB_IFACE_PCF8574   0x0020
# define KEYB_IFACE_TYPE(_keyb_setup)  ((_keyb_setup) & KEYB_IFACE_TYPE_MSK)

# define KEYB_IFACE_DO_MSK    0x0100
# define KEYB_IFACE_SORTED    0x0000
# define KEYB_IFACE_USORTED   0x0100
# define KEYB_IFACE_DO(_keyb_setup)  ((_keyb_setup) & KEYB_IFACE_DO_MSK)

/* =================Fin Définitions pour le driver KEYB==================== */
/**
 * @}
 */

#if ! defined(__DOXYGEN__)
/* ======================================================================== */
#if defined(__cplusplus)
#define __BEGIN_C_DECLS  extern "C" {
#define __END_C_DECLS    }
#else
#define __BEGIN_C_DECLS
#define __END_C_DECLS
#endif

#ifndef AVRIO_CPU_FREQ
#ifdef F_CPU
#define AVRIO_CPU_FREQ F_CPU
#else
#warning AVRIO_CPU_FREQ non definie ! Valeur par defaut affectee: AVRIO_CPU_FREQ = 8000000UL
#define AVRIO_CPU_FREQ 8000000UL
#endif
#endif

#define OUI 1
#define YES 1
#define NON 0
#define NO  0
#define WAIT_INFINITE 0

#ifndef TRUE
# define TRUE 1
#endif

#ifndef FALSE
# define FALSE 0
#endif

#ifndef __ORDER_LITTLE_ENDIAN__
#define __ORDER_LITTLE_ENDIAN__ 1234
#endif
#ifndef __ORDER_BIG_ENDIAN__
#define __ORDER_BIG_ENDIAN__ 4321
#endif
#ifndef __BYTE_ORDER__
#define __BYTE_ORDER__ __ORDER_LITTLE_ENDIAN__
#endif

#ifndef RAMSIZE
#define RAMSIZE (RAMEND+1-((RAMEND+1) & (-1-RAMEND)))
#endif

#include <errno.h>
#ifndef ENOENT
#define ENOENT           2      /* No such file or directory */
#endif
#ifndef EIO
#define EIO              5      /* I/O error */
#endif
#ifndef ENOMEM
#define ENOMEM          12      /* Out of memory */
#endif
#ifndef EFAULT
#define EFAULT          14      /* Bad address */
#endif
#ifndef EBUSY
#define EBUSY           16      /* Device or resource busy */
#endif
#ifndef EBADMSG
#define EBADMSG         74      /* Not a data message */
#endif
#ifndef ETIMEDOUT
#define ETIMEDOUT       110     /* Connection timed out */
#endif
#ifndef EINVAL
#define EINVAL          22      /* Invalid argument */
#endif

#endif /* __DOXYGEN__ not defined */

#ifndef __ASSEMBLER__
  __BEGIN_C_DECLS
/* ==========================Partie Langage C============================== */
#define __need_NULL
#define __need_size_t
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* types ================================================================== */
#ifndef AVRIO_TIME_UINT32
/**
 * @ingroup defs_group
 * @brief Type permettant de stocker un temps sur 16 bits
 *
 * Cette taille est celle par défaut lorsque la macro AVRIO_TIME_UINT32 n'est pas
 * définie au moment de la compilation.
 */
  typedef uint16_t time_t;

/**
 * @ingroup defs_group
 * @brief Type permettant de stocker un nombre de ticks sur 16 bits
 *
 * Cette taille est celle par défaut lorsque la macro AVRIO_TIME_UINT32 n'est pas
 * définie au moment de la compilation.
 */
  typedef uint16_t ticks_t;
#else
/**
 * @ingroup defs_group
 * @brief Type permettant de stocker un temps sur 32 bits
 *
 * Cette taille est celle lorsque la macro AVRIO_TIME_UINT32 est
 * définie au moment de la compilation.
 */
  typedef uint32_t time_t;

/**
 * @ingroup defs_group
 * @brief Type permettant de stocker un nombre de ticks sur 16 bits
 *
 * Cette taille est celle lorsque la macro AVRIO_TIME_UINT32 est
 * définie au moment de la compilation.
 */
  typedef uint32_t ticks_t;
#endif

/* macros ================================================================= */
#ifndef cbi
/**
 * @ingroup defs_module
 * @brief Mets à zéro un bit de rang n d'une variable
 * @deprecated Ne devrait plus être utilisé.
 */
#define cbi(var,n) do { var &= ~_BV(n); } while(0)
#endif

#ifndef sbi
/**
 * @ingroup defs_module
 * @brief Mets à 1 un bit de rang n d'une variable
 * @deprecated Ne devrait plus être utilisé.
 */
#define sbi(var,n) do { var |=  _BV(n); } while(0)
#endif

#ifndef tbi
/**
 * @ingroup defs_module
 * @brief Bascule l'état logique d'un bit de rang n d'une variable
 * @deprecated Ne devrait plus être utilisé.
 */
#define tbi(var,n) do { var ^=  _BV(n); } while(0)
#endif

#ifndef outp
/**
 * @ingroup defs_module
 * @brief Affecte un registre d'entrée-sortie
 * @deprecated Ne devrait plus être utilisé.
 */
#define outp(reg,value) do { reg = (value); } while(0)
#endif

#ifndef inp
/**
 * @ingroup defs_module
 * @brief Renvoie la valeur d'un registre d'entrée-sortie
 * @deprecated Ne devrait plus être utilisé.
 */
#define inp(reg)(reg)
#endif

#ifndef MSB16
/**
 * @ingroup defs_module
 * @brief Renvoie le poids fort d'un mot de 16 bits
 */
#define  MSB16(x) ((uint8_t) (x >> 8) & 0xFF)
#endif

#ifndef LSB
/**
 * @ingroup defs_module
 * @brief Renvoie le poids faible d'un mot
 */
#define  LSB(x) ((uint8_t) (x & 0xFF))
#endif

#ifndef PUINT8
/**
 * @ingroup defs_module
 * @brief Cast p vers un pointeur  de type uint8_t *
 */
#define PUINT8(p) ((uint8_t *)(p))
#endif

#ifndef PUINT16
/**
 * @ingroup defs_module
 * @brief Cast p vers un pointeur  de type uint16_t *
 */
#define PUINT16(p) ((uint16_t *)(p))
#endif

#ifndef PUINT32
/**
 * @ingroup defs_module
 * @brief Cast p vers un pointeur  de type uint32_t *
 */
#define PUINT32(p) ((uint32_t *)(p))
#endif

#ifndef PUINT64
/**
 * @ingroup defs_module
 * @brief Cast p vers un pointeur  de type uint64_t *
 */
#define PUINT64(p) ((uint64_t *)(p))
#endif

#ifndef ABS
/**
 * @ingroup defs_module
 * @brief Renvoie la valeur absolue de n
 */
#define ABS(n)  (((n) < 0) ? -(n) : (n))
#endif

#ifndef MIN
/**
 * @ingroup defs_module
 * @brief Renvoie la valeur min. de a et b
 */
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
/**
 * @ingroup defs_module
 * @brief Renvoie la valeur max. de a et b
 */
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef NOP
/**
 * @ingroup defs_module
 * @brief Ne fait rien mais utilise du temps processeur (1 cycle d'horloge)
 */
#define NOP() __asm__ volatile ("nop")
#endif

#ifndef STRUCT_FIELD_OFFSET
/**
 * @ingroup defs_module
 * @brief Renvoie l'offset d'un champ dans une structure
 */
#define STRUCT_FIELD_OFFSET(s,f)  ((size_t)&(((s *)0)->f))
#endif

/**
 * @ingroup defs_module
 * @brief Convertit un angle de degrés en radians
 */
#define DEG_TO_RAD(a) ((a) * M_PI / 180.0f)

/**
 * @ingroup defs_module
 * @brief Convertit un angle de radians en degrés
 */
#define RAD_TO_DEG(a) ((a) * 180.0f / M_PI)

/**
 * Perform an integer division rounding the result to the nearest int value.
 * \note \a divisor should preferibly be a constant, otherwise this macro generates
 * 2 division. Also divisor is evaluated twice.
 */
#define DIV_ROUND(dividend, divisor)  (((dividend) + (divisor) / 2) / (divisor))

/**
 * Perform an integer division rounding the result to the upper int value.
 * \note \a divisor is evaluated twice.
 */
#define DIV_ROUNDUP(dividend, divisor)  (((dividend) + (divisor) - 1) / (divisor))

/** Round up \a x to an even multiple of the 2's power \a pad. */
#define ROUND_UP2(x, pad) (((x) + ((pad) - 1)) & ~((pad) - 1))

/**
 * \name Integer round macros.
 *
 * Round \a x to a multiple of \a base.
 * \note If \a x is signed these macros generate a lot of code.
 * \{
 */
#define ROUND_DOWN(x, base)    ( (x) - ((x) % (base)) )
#define ROUND_UP(x, base)      ( ((x) + (base) - 1) - (((x) + (base) - 1) % (base)) )
#define ROUND_NEAREST(x, base) ( ((x) + (base) / 2) - (((x) + (base) / 2) % (base)) )

/** Check if \a x is an integer power of 2. */
#define IS_POW2(x)     (!(bool)((x) & ((x)-1)))

#ifndef offsetof
/**
 * Return the byte offset of the member \a m in struct \a s.
 *
 * \note This macro should be defined in "stddef.h" and is sometimes
 *       compiler-specific (g++ has a builtin for it).
 */
#define offsetof(s,m)  (size_t)&(((s *)0)->m)
#endif

#ifndef alignof
/**
 * Return the alignment in memory of a generic data type.
 *
 * \note We need to worry about alignment when allocating memory that
 * will be used later by unknown objects (e.g., malloc()) or, more
 * generally, whenever creating generic container types.
 */
#define alignof(type) offsetof(struct { char c; type member; }, member)
#endif

#define countof(a)  (sizeof(a) / sizeof(*(a)))

/* GCC attributes */
#define FORMAT(type,fmt,first)  __attribute__((__format__(type, fmt, first)))
#define NORETURN                __attribute__((__noreturn__))
#define UNUSED_ARG(type,arg)    __attribute__((__unused__)) type arg
#define UNUSED_VAR(type,name)   __attribute__((__unused__)) type name
#define USED_VAR(type,name)     __attribute__((__used__)) type name
#define INLINE                  static inline __attribute__((__always_inline__))
#define NOINLINE                __attribute__((noinline))
#define LIKELY(x)               __builtin_expect(!!(x), 1)
#define UNLIKELY(x)             __builtin_expect(!!(x), 0)
#define PURE_FUNC               __attribute__((pure))
#define CONST_FUNC              __attribute__((const))
#define UNUSED_FUNC             __attribute__((unused))
#define USED_FUNC               __attribute__((__used__))
#define RESTRICT                __restrict__
#define MUST_CHECK              __attribute__((warn_unused_result))
#define PACKED                  __attribute__((packed))
#define ALIGNED(x)              __attribute__ ((__aligned__(x)))

#define STATIC_ASSERT(exp, message_identifier) \
    struct compile_time_assertion { \
        char message_identifier : 8 + !(exp); \
    };

#if !defined(__DOXYGEN__)
/* ======================================================================== */

#define FLASH  __attribute__ ((progmem))
#define EEPROM __attribute__ ((section(".eeprom")))
#define NAKED  __attribute__ ((naked))
#define CTASK  __attribute__ ((noreturn))

/* macros ================================================================= */
#ifdef DEBUG_ENABLE
#define DEBUG_BLOCK()
#else
#define DEBUG_BLOCK() if(0)
/* Le compilateur supprimera le bloc suivant à l'optimisation */
#endif

#define __STATIC_ALWAYS_INLINE(func) \
  static __inline__ func __attribute__ ((always_inline));\
  func

/* types ================================================================== */
#ifndef _SSIZE_T_
#define _SSIZE_T_
typedef int ssize_t;
#endif /* _SSIZE_T_ not defined */

#endif /* __DOXYGEN__ not defined */

__END_C_DECLS
/* ========================Fin Partie Langage C============================ */
#endif /* __ASSEMBLER__ not defined */

/* ==========================Partie Assembleur============================= */
#if defined (__ASSEMBLER__) && !defined(__DOXYGEN__)

# define GLOBAL(__label__) \
  .global __label__

# define GLOBAL_DATA(__label__,__size__) \
  .comm __label__,__size__,1

# define LOCAL_DATA(__label__,__size__) \
  .lcomm __label__,__size__

# define STATIC_DATA(__label__,__size__) \
  .section .bss. ## __label__,"aw",@nobits $ \
__label__: $ \
  .skip __size__,0

# define EEPROM_STRING(__label__,__string__) \
  .section .eeprom,"aw",@progbits $ \
__label__: $ \
  .string __string__

# define FLASH_STRING(__label__,__string__) \
  .section .progmem.data,"a",@progbits $ \
__label__: $ \
  .string __string__

# define GLOBAL_SECTION(__label__) \
  .section .text.##__label__,"ax",@progbits

# define FUNCTION(__label__) \
  GLOBAL_SECTION(__label__) $ \
  GLOBAL(__label__) $ \
__label__: $

# define MODULE(__label__) GLOBAL_SECTION(__label__)

/* ========================Fin Partie Assembleur=========================== */
#endif /*  __ASSEMBLER__ defined and __DOXYGEN__ not defined */

#endif /* _AVRIO_DEFS_H_ */
