/**
 * @file melody.h
 * @brief Génération de mélodies musicales
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 */
/* ========================================================================== */
#ifndef _AVRIO_MELODY_H_
#  define _AVRIO_MELODY_H_

#  include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
  /* ======================================================================== */
/**
 * @addtogroup dev_group
 * @{
 *
 *  @defgroup melody_module Génération de mélodies musicales
 *
 *  Ce module permet de jouer des mélodies. L'implémentation permet de jouer
 *  des morceaux de musique sans monopoliser le processeur grâce à l'utilisation
 *  d'une tâche pour le cadencement de la lecture des notes et d'un timer pour
 *  la génération des sons.\n
 *  Le module s'interface avec le timer par l'intermédiaire de 3 fonctions
 *  implémentées par l'utilisateur dans avrio-config.h .\n
 *  La structure des sons est compatible avec BrickOs Music Studio (format
 *  BrickOS 2.0) et permet donc de jouer des fichiers midi.
 *  @{
 */
/**
 */
  /* structures ============================================================= */
/**
 * @struct xNote
 * @brief Structure Note de musique
 */
   typedef struct xNote {
  uint8_t pitch;      /**< pitch de la note, 0 ^= A_0 (~55 Hz) */
  uint8_t length;     /**< durée de note (en 1/16 de note) */
} xNote;

/* internal public functions ================================================ */

/**
 * @brief Initialise le module son
 * @param xDuration durée en ms du 1/16 ième de note (plus petite durée)
 * @param xInternote durée en ms du silence entre notes
 */
void vMelodyInit (uint8_t xDuration, uint8_t xInternote);

/**
 * @brief  Joue une suite de notes (un morceau de musique) résidant en RAM
 * @param  xMelody pointeur en RAM vers le morceau
 */
void vMelodyPlay (const xNote * xMelody);

/**
 * @brief  Joue un morceau de musique résidant en FLASH
 * @param  xMelody pointeur en FLASH vers le morceau
 */
void vMelodyPlayFlash (const xNote * xMelody);

/**
 * Joue un morceau de la table des sons système
 * Le module dispose par défaut de 4 sons.
 * @param ucIndex index du morceau dans la table des sons système
 */
void vMelodyPlaySystem (uint8_t ucIndex);

/**
 * @brief Arrête brutalement la lecture du morceau en cours
 */
void vMelodyStop (void);

/**
 * @brief Fonction appelée par le module à chaque changement de TOP
 * Si le timer est utilisé pour d'autres fonctions que melody, l'utilisateur
 * doit définir AVRIO_MELODY_TIMERCB_ENABLE et cette fonction afin de récupérer
 * la nouvelle valeur de TOP. \n
 * Le timer est stoppé pendant l'exécution de cette fonction.
 */
extern void vMelodyTopChangedCB (uint16_t usNewTop);

/* inline public functions ================================================== */

/**
 * @brief Joue un bip bref.
 */
void vMelodyBeep (void);

/**
 * @brief Met en pause ou reprend la lecture d'un morceau
 * @param xPause true pour mettre en pause, false pour reprendre
 */
void vMelodyPause (bool xPause);

/**
 * @brief Indique si un lecture est en cours
 * @return true si une lecture est en cours
 */
bool xMelodyIsPlay (void);

/**
 * @brief Modifie la durée du 1/16 ième de note
 * @param xDuration durée en ms du 1/16 ième de note
 */
void vMelodySetDuration (uint8_t xDuration);

/**
 * @brief Lecture de la durée du 1/16 ième de note courante
 */
uint8_t ucMelodyDuration (void);

/**
 * @brief Modifie la durée de l'espace internote (soustrait de la durée de note)
 *
 * Mettre 0 pour un legato parfait
 * @param internote durée en ms du silence entre notes
 */
void vMelodySetInternote (uint8_t xInternote);

/**
 * @brief Lecture de la durée de l'espace internote courant
 */
uint8_t ucMelodyInternote (void);

/* constants ================================================================ */
/**
 * @brief Index du beep dans la table système
 */
#  define MELODY_BEEP      0
/**
 * @brief Index du son de bienvenue dans la table système
 */
#  define MELODY_WELCOME  1
/**
 * @brief Index du son d'incident dans la table système
 */
#  define MELODY_WARNING  2
/**
 * @brief Index du son d'erreur dans la table système
 */
#  define MELODY_ERROR    3
/**
 * @brief Nombre de sons dans la table système
 */
#  define MELODY_SYS_MAX  4
/**
 * @brief Durée d'une note  (en 1/16 de note)
 */
#  define WHOLE   16
/**
 * @brief Durée 1/2 de note (en 1/16 de note)
 */
#  define HALF    8
/**
 * @brief Durée 1/4 de note (en 1/16 de note)
 */
#  define QUARTER 4
/**
 * @brief Durée 1/8 de note (en 1/16 de note)
 */
#  define EIGHTH  2

/**
 *  @defgroup melody_pitch Liste des notes
 *  @{
 */
#  define PITCH_A0    0
#  define PITCH_Am0   1
#  define PITCH_H0    2
/**
 * @brief Do Octave 1
 */
#  define PITCH_C1    3
/**
 * @brief Do dièse Octave 1
 */
#  define PITCH_Cm1   4
/**
 * @brief Ré Octave 1
 */
#  define PITCH_D1    5
/**
 * @brief Ré dièse Octave 1
 */
#  define PITCH_Dm1   6
/**
 * @brief Mi Octave 1
 */
#  define PITCH_E1    7
/**
 * @brief Fa Octave 1
 */
#  define PITCH_F1    8
/**
 * @brief Fa dièse Octave 1
 */
#  define PITCH_Fm1   9
/**
 * @brief Sol Octave 1
 */
#  define PITCH_G1    10
/**
 * @brief Sol dièse Octave 1
 */
#  define PITCH_Gm1   11
/**
 * @brief La Octave 1
 */
#  define PITCH_A1    12
/**
 * @brief La dièse Octave 1
 */
#  define PITCH_Am1   13
/**
 * @brief Si Octave 1
 */
#  define PITCH_H1    14
#  define PITCH_C2    15
#  define PITCH_Cm2   16
#  define PITCH_D2    17
#  define PITCH_Dm2   18
#  define PITCH_E2    19
#  define PITCH_F2    20
#  define PITCH_Fm2   21
#  define PITCH_G2    22
#  define PITCH_Gm2   23
#  define PITCH_A2    24
#  define PITCH_Am2   25
#  define PITCH_H2    26
#  define PITCH_C3    27
#  define PITCH_Cm3   28
#  define PITCH_D3    29
#  define PITCH_Dm3   30
#  define PITCH_E3    31
#  define PITCH_F3    32
#  define PITCH_Fm3   33
#  define PITCH_G3    34
#  define PITCH_Gm3   35
#  define PITCH_A3    36
#  define PITCH_Am3   37
#  define PITCH_H3    38
#  define PITCH_C4    39
#  define PITCH_Cm4   40
#  define PITCH_D4    41
#  define PITCH_Dm4   42
#  define PITCH_E4    43
#  define PITCH_F4    44
#  define PITCH_Fm4   45
#  define PITCH_G4    46
#  define PITCH_Gm4   47
/**
 * @brief La Octave 4 de référence F=440 Hz
 */
#  define PITCH_A4    48
#  define PITCH_Am4   49
#  define PITCH_H4    50
#  define PITCH_C5    51
#  define PITCH_Cm5   52
#  define PITCH_D5    53
#  define PITCH_Dm5   54
#  define PITCH_E5    55
#  define PITCH_F5    56
#  define PITCH_Fm5   57
#  define PITCH_G5    58
#  define PITCH_Gm5   59
#  define PITCH_A5    60
#  define PITCH_Am5   61
#  define PITCH_H5    62
#  define PITCH_C6    63
#  define PITCH_Cm6   64
#  define PITCH_D6    65
#  define PITCH_Dm6   66
#  define PITCH_E6    67
#  define PITCH_F6    68
#  define PITCH_Fm6   69
#  define PITCH_G6    70
#  define PITCH_Gm6   71
#  define PITCH_A6    72
#  define PITCH_Am6   73
#  define PITCH_H6    74
#  define PITCH_C7    75
#  define PITCH_Cm7   76
#  define PITCH_D7    77
#  define PITCH_Dm7   78
#  define PITCH_E7    79
#  define PITCH_F7    80
#  define PITCH_Fm7   81
#  define PITCH_G7    82
#  define PITCH_Gm7   83
#  define PITCH_A7    84
#  define PITCH_Am7   85
#  define PITCH_H7    86
#  define PITCH_C8    87
#  define PITCH_Cm8   88
#  define PITCH_D8    89
#  define PITCH_Dm8   90
#  define PITCH_E8    91
#  define PITCH_F8    92
#  define PITCH_Fm8   93
#  define PITCH_G8    94
#  define PITCH_Gm8   95
#  define PITCH_A8    96
/**
 * @brief Spécifie une pause
 */
#  define PITCH_PAUSE 97
/**
 * @brief Valeur maximale d'un pitch
 */
#  define PITCH_MAX   98
/**
 * @brief Marque de fin d'une liste de notes (morceau de musique)
 */
#  define PITCH_END   255
/**
 * @}
 */

#  if defined(__DOXYGEN__)
/* 
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */

  /**
   *   @}
   * @}
   */
#  else
/* 
 * __DOXYGEN__ not defined
 * Partie ne devant pas être documentée.
 * =============================================================================
 */

#  endif /* __DOXYGEN__ not defined */
/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif /* _AVRIO_MELODY_H_ */
