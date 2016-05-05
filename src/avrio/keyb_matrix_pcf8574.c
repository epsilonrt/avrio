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
 * @file keyb_matrix_pcf8574.c
 * @brief Clavier matriciel sur port PCF8574 (I2c)
 */

#if KEYB_IFACE_TYPE(KEYB_DRIVER_SETUP) == KEYB_IFACE_PCF8574
/* ========================================================================== */
# include <avrio/twi.h>
# include <util/atomic.h>

/* private variables ======================================================== */
static uint8_t ucPort;

/* private functions ======================================================== */
/*
 * Ecriture sur le port du PCF8584
 */
static void
prvvPortWrite (void) {

  eTwiWrite (KEYB_TWI_DEVICE_ADDR, ucPort);
}

/*
 * Lecture du port du PCF8584
 */
static void
prvvPortRead (void) {

  eTwiRead (KEYB_TWI_DEVICE_ADDR, &ucPort);
}
  
/*
 * Initialise l'interface
 */
static inline void
prvvIfaceInit (void) {
# ifdef KEYB_TWI_MASTER_INIT
  vTwiInit ();
  eTwiSetSpeed (KEYB_TWI_MASTER_INIT);
# endif
  ucPort = 0xFF;
  prvvPortWrite ();
}

/* 
 * lecture code colonne sur le port
 * @return
 *  0x00 pas de touche appuyée
 *  0x01 colonne 0
 *  0x02 colonne 1
 *  0x04 colonne 2
 *  0x08 colonne 3
 */
static uint8_t
prvucReadCodeX (void) {

  prvvPortRead ();
  return (~ucPort >> KEYB_COL_LSB) & KEYB_MSK(KEYB_WIDTH);
}

/* 
 * lecture code ligne sur le port
 * @return
 *  0x00 pas de touche appuyée
 *  0x01 ligne 0
 *  0x02 ligne 1
 *  0x04 ligne 2
 *  0x08 ligne 3
 */
static uint8_t
prvucReadCodeY (void) {

  prvvPortRead ();
  return (~ucPort >> KEYB_ROW_LSB) & KEYB_MSK(KEYB_HEIGHT);
}

/* 
 * Mets les lignes en sortie et les colonnes en entrée
 */
static void
prvvSetRowOut (void) {

  ucPort = (KEYB_MSK(KEYB_WIDTH) << KEYB_COL_LSB);
  prvvPortWrite ();
}

/* 
 * Mets les lignes en entrée et les colonnes en sortie
 */
static void
prvvSetRowIn (void) {

  ucPort = (KEYB_MSK(KEYB_HEIGHT) << KEYB_ROW_LSB);
  prvvPortWrite ();
}

#endif /* KEYB_IFACE_PIO */
/* ========================================================================== */
