#include "test_pid.h"
#include "../config.h"

/* constants ================================================================ */
#define DELTA_T 50.0
#define KP  10.0
#define KI  13.0
#define KD  0.02

/* macros =================================================================== */
#define DT_S (DELTA_T*1e-3)
#define KRPM ((int16_t)(60 / (CODEUR_IMPULSIONS_PAR_TOUR * DT_S)) )
#define VIT_RPM(vitesse_codeur) ((int16_t)(KRPM * (vitesse_codeur)))
#define VIT_CODEUR(vitesse_rpm) ((int16_t)((vitesse_rpm) / KRPM))
#define SK(K) ((int16_t)((K)*SCALING_FACTOR))

/* private variables ======================================================== */
static const char msg_vitesse[] PROGMEM = "V=       T/mn";
static const char msg_format[] PROGMEM = "%+05d";

static FILE mylcd = FDEV_SETUP_STREAM (lcd_fputc, NULL, _FDEV_SETUP_WRITE);

static xPidData pid_vitesse;

/* private functions ======================================================== */
static void
init (void) {

  sequenceur_init ();
  lcd_init ();
  codeur_init ();
  moteur_init ();
  vPidInit (KP, KI * DT_S, KD / DT_S, &pid_vitesse);
  stdout = &mylcd;
  sequenceur_demarrer_service (OUI);
}

/* internal public functions ================================================ */
int
main (void) {
  int8_t pid_flag;
  int32_t position, position_precedent, consigne, mesure;
  int16_t commande;

  init ();
  pid_flag = sequenceur_flag_ajouter (DELTA_T);
  position_precedent = 0;
  consigne = VIT_CODEUR (4000);
  lcd_effacer ();
  printf_P (msg_vitesse);

  for (;;) {

    if (sequenceur_flag_synchro (pid_flag) == TRUE) {

      position = codeur_azimut_compteur ();
      mesure = position - position_precedent;
      position_precedent = position;

      commande = (int16_t) iPidProcess (consigne, mesure, &pid_vitesse);
      moteur_azimut_rotation (commande);
      lcd_deplacer_curseur (2, 0);
      printf_P (msg_format, VIT_RPM (mesure));
    }
  }
  return 0;
}

/**
 * Fonction de service du séquenceur appellée à chaque itération (1 ms)
 */
void
sequenceur_service (void) {

  moteur_service ();
}

/* ========================================================================== */
