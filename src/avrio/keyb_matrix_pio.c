/**
 * Copyright © 2011-2015 Pascal JEAN aka epsilonRT. All rights reserved.
 *
 * This file is part of AvrIO.
 *
 * AvrIO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * AvrIO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with AvrIO.  If not, see <http://www.gnu.org/licenses/lgpl.html>
 *
 * @file keyb_matrix_pio.c
 * @brief Clavier matriciel sur port PIO
 */

#if KEYB_IFACE_TYPE(KEYB_DRIVER_SETUP) == KEYB_IFACE_PIO
/* ========================================================================== */
# if KEYB_WIDTH > 4
#  error KEYB_WIDTH trop grand !
# endif
# if KEYB_HEIGHT > 4
#  error KEYB_HEIGHT trop grand !
# endif

/* private functions ======================================================== */
/*
 * Initialise l'interface
 */
static inline void
prvvIfaceInit (void) {

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

#  if KEYB_IFACE_DO(KEYB_DRIVER_SETUP) == KEYB_IFACE_USORTED
  uint8_t ucCode = 0;

  if (bit_is_clear (KEYB_C1_PIN, KEYB_C1_BIT))
    ucCode |= 0x01;
#   if KEYB_WIDTH > 1
  if (bit_is_clear (KEYB_C2_PIN, KEYB_C2_BIT))
    ucCode |= 0x02;
#   endif
#   if KEYB_WIDTH > 2
  if (bit_is_clear (KEYB_C3_PIN, KEYB_C3_BIT))
    ucCode |= 0x04;
#   endif
#   if KEYB_WIDTH > 3
  if (bit_is_clear (KEYB_C4_PIN, KEYB_C4_BIT))
    ucCode |= 0x08;
#   endif

  return ucCode;
#  else
  return (~KEYB_COL_PIN >> KEYB_COL_LSB) & KEYB_MSK(KEYB_WIDTH);
#  endif
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

#  if KEYB_IFACE_DO(KEYB_DRIVER_SETUP) == KEYB_IFACE_USORTED
  uint8_t ucCode = 0;

  if (bit_is_clear (KEYB_R1_PIN, KEYB_R1_BIT))
    ucCode |= 0x01;
#   if KEYB_WIDTH > 1
  if (bit_is_clear (KEYB_R2_PIN, KEYB_R2_BIT))
    ucCode |= 0x02;
#   endif
#   if KEYB_WIDTH > 2
  if (bit_is_clear (KEYB_R3_PIN, KEYB_R3_BIT))
    ucCode |= 0x04;
#   endif
#   if KEYB_WIDTH > 3
  if (bit_is_clear (KEYB_R4_PIN, KEYB_R4_BIT))
    ucCode |= 0x08;
#   endif


  return ucCode;
#  else
  return (~KEYB_ROW_PIN >> KEYB_ROW_LSB) & KEYB_MSK(KEYB_HEIGHT);
#  endif
}

/* 
 * Mets les lignes en sortie et les colonnes en entrée
 */
static void
prvvSetRowOut (void) {

#  if KEYB_IFACE_DO(KEYB_DRIVER_SETUP) == KEYB_IFACE_USORTED

  /* Colonnes avec r pull-up */
  sbi (KEYB_C1_PORT, KEYB_C1_BIT);
  /* Colonnes en entrées */
  cbi (KEYB_C1_DDR, KEYB_C1_BIT);
  /* Lignes en sortie */
  sbi (KEYB_R1_DDR, KEYB_R1_BIT);
  /* Lignes à zéro */
  cbi (KEYB_R1_PORT, KEYB_R1_BIT);

#   if KEYB_WIDTH > 1
  sbi (KEYB_C2_PORT, KEYB_C2_BIT);
  cbi (KEYB_C2_DDR, KEYB_C2_BIT);
#   endif
#   if KEYB_WIDTH > 2
  sbi (KEYB_C3_PORT, KEYB_C3_BIT);
  cbi (KEYB_C3_DDR, KEYB_C3_BIT);
#   endif
#   if KEYB_WIDTH > 3
  sbi (KEYB_C4_PORT, KEYB_C4_BIT);
  cbi (KEYB_C4_DDR, KEYB_C4_BIT);
#   endif

#   if KEYB_HEIGHT > 1
  sbi (KEYB_R2_DDR, KEYB_R2_BIT);
  cbi (KEYB_R2_PORT, KEYB_R2_BIT);
#   endif
#   if KEYB_HEIGHT > 2
  sbi (KEYB_R3_DDR, KEYB_R3_BIT);
  cbi (KEYB_R3_PORT, KEYB_R3_BIT);
#   endif
#   if KEYB_HEIGHT > 3
  sbi (KEYB_R4_DDR, KEYB_R4_BIT);
  cbi (KEYB_R4_PORT, KEYB_R4_BIT);
#   endif

#  else
  /* Colonnes avec r pull-up */
  KEYB_COL_PORT |= (KEYB_MSK(KEYB_WIDTH) << KEYB_COL_LSB);
  /* Colonnes en entrées */
  KEYB_COL_DDR &= ~(KEYB_MSK(KEYB_WIDTH) << KEYB_COL_LSB);
  /* Lignes en sortie */
  KEYB_ROW_DDR |= (KEYB_MSK(KEYB_HEIGHT) << KEYB_ROW_LSB);
  /* Lignes à zéro */
  KEYB_ROW_PORT &= ~(KEYB_MSK(KEYB_HEIGHT) << KEYB_ROW_LSB);
#  endif
}

/* 
 * Mets les lignes en entrée et les colonnes en sortie
 */
static void
prvvSetRowIn (void) {

#  if KEYB_IFACE_DO(KEYB_DRIVER_SETUP) == KEYB_IFACE_USORTED
  /* Lignes avec r pull-up */
  sbi (KEYB_R1_PORT, KEYB_R1_BIT);
  /* Lignes en entrées */
  cbi (KEYB_R1_DDR, KEYB_R1_BIT);
  /* Colonnes en sortie */
  sbi (KEYB_C1_DDR, KEYB_C1_BIT);
  /* Colonnes à zéro */
  cbi (KEYB_C1_PORT, KEYB_C1_BIT);

#   if KEYB_WIDTH > 1
  sbi (KEYB_C2_DDR, KEYB_C2_BIT);
  cbi (KEYB_C2_PORT, KEYB_C2_BIT);
#   endif
#   if KEYB_WIDTH > 2
  sbi (KEYB_C3_DDR, KEYB_C3_BIT);
  cbi (KEYB_C3_PORT, KEYB_C3_BIT);
#   endif
#   if KEYB_WIDTH > 3
  sbi (KEYB_C4_DDR, KEYB_C4_BIT);
  cbi (KEYB_C4_PORT, KEYB_C4_BIT);
#   endif

#   if KEYB_HEIGHT > 1
  sbi (KEYB_R2_PORT, KEYB_R2_BIT);
  cbi (KEYB_R2_DDR, KEYB_R2_BIT);
#   endif
#   if KEYB_HEIGHT > 2
  sbi (KEYB_R3_PORT, KEYB_R3_BIT);
  cbi (KEYB_R3_DDR, KEYB_R3_BIT);
#   endif
#   if KEYB_HEIGHT > 3
  sbi (KEYB_R4_PORT, KEYB_R4_BIT);
  cbi (KEYB_R4_DDR, KEYB_R4_BIT);
#   endif

#  else
  /* Lignes à 1 / valide r pull-up */
  KEYB_ROW_PORT |= (KEYB_MSK(KEYB_HEIGHT) << KEYB_ROW_LSB);
  /* Lignes en entrée */
  KEYB_ROW_DDR &= ~(KEYB_MSK(KEYB_HEIGHT) << KEYB_ROW_LSB);
  /* Colonnes en sortie */
  KEYB_COL_DDR |= (KEYB_MSK(KEYB_WIDTH) << KEYB_COL_LSB);
  /* Colonnes à zéro */
  KEYB_COL_PORT &= ~(KEYB_MSK(KEYB_WIDTH) << KEYB_COL_LSB);
#  endif
  /* 
   *  Temps d'établissement pour la lecture des lignes:
   *    Lire le paragraphe "Reading the Pin Value" du chapitre I/O Ports
   *    du datasheet de l'atmega32 (pages 49-50)
   *    Le temps d'établissement minimal est de 1 cycle d'horloge, mais,
   *    les entrées étant avec résistances de tirage et la mise à zéro de la
   *    broche d'entrée étant réalisée par le contact du clavier, le temps réel
   *    dépendra des caractéristiques RC de ces 2 éléments (mais surtout du
   *    clavier). En particulier, c'est la valeur de la capacité en pF du
   *    contact du clavier qui est importante. Le temps d'établissement est dû
   *    aux contacts que sont relachés et non à celui qui est appuyé.
   *    La valeur théorique est de 458 ns et celle mesurée de 530 ns soit
   *    9 appels de NOP()
   */
  NOP (); /* Un NOP dure 1 cycle d'horloge du quartz soit 62.5 ns à 16 MHz */
  NOP ();
  NOP ();
  NOP ();
  NOP ();
  NOP ();
  NOP ();
  NOP ();
  NOP ();
}

#endif /* KEYB_IFACE_PIO */
/* ========================================================================== */
