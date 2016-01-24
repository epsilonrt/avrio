/*
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