/*
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
 */
#ifndef _MSL_CONFIG_H_
#define _MSL_CONFIG_H_

/* constants ================================================================ */

#define MSLCFG_DEFAULT_MIDI_CHAN  MIDI_CHANNEL(3) /* 1 à 16 */
#define MSLCFG_DEFAULT_TILT       1 /* 0: Off - 1: On */
#define MSLCFG_DEFAULT_ROTATION   1 /* 0: Off - 1: On */
#define MSLCFG_DEFAULT_ABSOLUTE   0 /* 0: Off - 1: On */

#define MSLCFG_BANK_PITCH                  MIDI_C_m1

/* Module motion TILT */
// Note TILT ON
#define MSLCFG_TILT_PITCH                  MIDI_F_0

// Note PICKUP ON
#define MSLCFG_TILT_PICKUP_PITCH           MIDI_F_1

#define MSLCFG_TILT_LEFT_CC                0
#define MSLCFG_TILT_FORWARDS_CC            1
#define MSLCFG_TILT_RIGHT_CC               2
#define MSLCFG_TILT_BACK_CC                3
#define MSLCFG_TILT_LEFT_SECONDARY_CC      4
#define MSLCFG_TILT_FORWARDS_SECONDARY_CC  5
#define MSLCFG_TILT_RIGHT_SECONDARY_CC     6
#define MSLCFG_TILT_BACK_SECONDARY_CC      7

/* Constante du filtre pass-bas de TILT */
#define MSLCFG_TILT_LPF_CONSTANT 0.1
#define MSLCFG_ROT_LPF_CONSTANT  0.1

// Angle en degré de roulis ou tanguage à partir duquel le contrôleur quitte
// le mode PickUp 
#define MSLCFG_TILT_LEVEL             10.0

// Hystérésis sur la détection du seuil ci-dessus
#define MSLCFG_TILT_HYSTERESIS        2.0

/* Module COMPASS */

// Angle en degré de lacet à partir duquel le contrôleur envoie les messages
// CC des boutons
#define MSLCFG_YAW_LEVEL             1.0

// Nombre de terme de moyennage sur la mesure d'angle ROT (1 pas de moyennage)
#define MSLCFG_ROT_MEAN_TERMS        1

#define MSLCFG_ENCODER_STEP 4

#define DEBUG_MIDI

#define MSLCFG_FADER_LIST_SIZE 3
#define MSLCFG_FADER_LIST {0,1,2}
/* ========================================================================== */
#endif  /* _MSL_CONFIG_H_ not defined */
