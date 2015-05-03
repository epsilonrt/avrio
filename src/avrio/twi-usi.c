/**
 * @file twi-usi.c
 * @brief Bus I2C avec coupleur USI (Implémentation)
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20120427 - Initial version by epsilonRT from AVR312 Revision 1.14
 */
#include "avrio-config.h"

#ifdef AVRIO_TWI_ENABLE
/* ========================================================================== */
#include <util/atomic.h>
#include "avrio-board-twi.h"
#include "twi-usi.h"
#ifdef __OPTIMIZE__
# include <avrio/delay.h>
#endif

#ifdef TWI_MASTER_ENABLE
/* ========================================================================== */
#error "Module Master TWI-USI non dispo. (TODO avec AVR310)"
/* ========================================================================== */
#endif /* TWI_MASTER_ENABLE */

#ifdef TWI_SLAVE_ENABLE
/* ========================================================================== */

/* macros =================================================================== */
#define TWI_SLAVE_RX_BUFFER_MASK (TWI_SLAVE_RXBUFSIZE - 1)
#if (TWI_SLAVE_RXBUFSIZE & TWI_SLAVE_RX_BUFFER_MASK)
        #error TWI RX buffer size is not a power of 2
#endif

#define TWI_SLAVE_TX_BUFFER_MASK (TWI_SLAVE_TXBUFSIZE - 1)
#if (TWI_SLAVE_TXBUFSIZE & TWI_SLAVE_TX_BUFFER_MASK)
        #error TWI TX buffer size is not a power of 2
#endif

/* constants ================================================================ */
/* Différentes phases du processus de transmission */
#define USI_SLAVE_CHECK_ADDRESS                (0x00)
#define USI_SLAVE_SEND_DATA                    (0x01)
#define USI_SLAVE_REQUEST_REPLY_FROM_SEND_DATA (0x02)
#define USI_SLAVE_CHECK_REPLY_FROM_SEND_DATA   (0x03)
#define USI_SLAVE_REQUEST_DATA                 (0x04)
#define USI_SLAVE_GET_DATA_AND_SEND_ACK        (0x05)

/* private variables ======================================================== */
static uint8_t ucTwiSlaveAddress;
static volatile uint8_t ucTwiUsiOverflowState;

static uint8_t ucTwiUsiRxBuffer[TWI_SLAVE_RXBUFSIZE];
static volatile uint8_t ucTwiUsiRxHead;
static volatile uint8_t ucTwiUsiRxTail;

static uint8_t ucTwiUsiTxBuffer[TWI_SLAVE_TXBUFSIZE];
static volatile uint8_t ucTwiUsiTxHead;
static volatile uint8_t ucTwiUsiTxTail;

static vTwiUsiSlaveHandler pvTwiUsiSlaveTxHandler;

/* inline functions ========================================================= */

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (void prvvSendAck (void)) {

  USIDR    =  0; /* SDA = 0 => ACK */
  USI_DDR |=  _BV(USI_SDA); /* SDA en sortie */
  /* Clear tous les flags sauf celui du START et arme l'interruption d'overfow
   * pour se déclencher au bout d'une impulsion d'horloge (2 fronts) */
  USISR    =  _BV(USIOIF)|_BV(USIPF)|_BV(USIDC)|(0x0E<<USICNT0);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (void prvvSendData (void)) {

  USI_DDR |=  _BV(USI_SDA); /* SDA en sortie */
  /* Clear tous les flags sauf celui du START et arme l'interruption d'overfow
   * pour se déclencher au bout de 8 impulsions d'horloge (16 fronts) */
  USISR    =  _BV(USIOIF)|_BV(USIPF)|_BV(USIDC);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (void prvvReadAck (void)) {

  USI_DDR &=  ~_BV(USI_SDA); /* SDA en entrée */
  USIDR    =  0; /* Clear pour lire l'état ACK */
  /* Clear tous les flags sauf celui du START et arme l'interruption d'overfow
   * pour se déclencher au bout d'une impulsion d'horloge (2 fronts) */
  USISR    =  _BV(USIOIF)|_BV(USIPF)|_BV(USIDC)|(0x0E<<USICNT0);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (void prvvReadData (void)) {

  USI_DDR &= ~_BV(USI_SDA); /* SDA en entrée */
  /* Clear tous les flags sauf celui du START et arme l'interruption d'overfow
   * pour se déclencher au bout de 8 impulsions d'horloge (16 fronts) */
  USISR    =  _BV(USIOIF)|_BV(USIPF)|_BV(USIDC);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (void prvvRelease (void)) {

  /* Réinitilise pour l'attente d'une condition de START
     1- Valide l'interruption de START et dévalide celle de fin de trame
     2- Valide le mode I2C
     3- Mémorisation sur front montant de l'horloge SCL */
  USICR    =  _BV(USISIE)|
              _BV(USIWM1)|
              _BV(USICS1);
  /* Clear tous les flags sauf celui du START */
  USISR    =  _BV(USIOIF)|_BV(USIPF)|_BV(USIDC);
}

/* -----------------------------------------------------------------------------
 * Vide les buffers de transmission
 */
__STATIC_ALWAYS_INLINE (void vFlushTxBuffers (void)) {

  ucTwiUsiTxTail = 0;
  ucTwiUsiTxHead = 0;
}

/* -----------------------------------------------------------------------------
 * Vide les buffers de réception
 */
__STATIC_ALWAYS_INLINE (void vFlushRxBuffers (void)) {

  ucTwiUsiRxTail = 0;
  ucTwiUsiRxHead = 0;
}

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
static void
prvvInit (void) {

  USI_DDR  |=  _BV(USI_SCL) | _BV(USI_SDA);
  /* SCL en sortie, c'est normal ?
   * Oui car USI en mode TWI met la ligne SCL à zéro dès qu'une condition de
   * START est détecté, cela permet d'indiquer au maître d'attendre. SCL est
   * relachée dès que USISIF est remis à zéro, cad lorsque le traitement est
   * effectué par la routine USI_START_VECTOR.
   * Si USIWM=1, alors ce mécanisme se produit aussi pour l'interruption
   * USI_OVERFLOW_VECTOR.
   */
  USI_PORT |=  _BV(USI_SCL); /* SCL à l'état haut */
  USI_PORT |=  _BV(USI_SDA); /* SDA à l'état haut */
  USI_DDR  &= ~_BV(USI_SDA); /* SDA en entrée */
  /*
     1- Valide l'interruption de START et dévalide celle de fin de trame
     2- Valide le mode I2C, Wait-sate SCL sur START
     3- Mémorisation sur front montant de l'horloge SCL */
  USICR    =  _BV(USISIE)|
              _BV(USIWM1)|
              _BV(USICS1);
  /* Clear tous les flags */
  USISR    = _BV(USISIF)|_BV(USIOIF)|_BV(USIPF)|_BV(USIDC);
}

/* -----------------------------------------------------------------------------
 * Gestionnaire de détection de START
 * Détecte l'arrivée d'une condition de START et configure le coupleur pour la
 * réception de l'adresse circuit.
 */
ISR(USI_START_VECTOR){

  /* La prochaine étape est la vérification de l'adresse transmise sur le bus */
  ucTwiUsiOverflowState = USI_SLAVE_CHECK_ADDRESS;
  USI_DDR  &= ~_BV(USI_SDA); /* SDA en entrée */

  /*
   * Un START a eu lieu, c-a-d que SDA est passé à 0 alors que SCL est 1
   * Attente du passage de SCL à 0, confirmant le START
   * Sort aussi pour éviter de bloquer si SDA passe à 1 (condition de STOP)
   */
  while ((USI_PIN & _BV(USI_SCL)) & !(USI_PIN & _BV(USI_SDA)))
    ;

  if (USI_PIN & _BV(USI_SDA)) {
    /* SDA = 1, sortie de la boucle d'attente sur STOP */

    /* Retourne en attente START
       1- Valide l'interruption de START et dévalide celle de fin de trame
       2- Valide le mode I2C, Wait-sate SCL sur START
       3- Mémorisation sur front montant de l'horloge SCL */
    USICR    =  _BV(USISIE)|
                _BV(USIWM1)|
                _BV(USICS1);
  }
  else {
    /* SDA = 0 et SCL = 0, sortie de la boucle d'attente sur START */

    /* 1- Valide des interruptions de START (Restart en l'occurence) et
     *    de fin de trame (Overflow de 16 transitions = transmission d'un octet)
     * 2- Configure en mode I2C
     * 3- Shift Register Clock Source = External, positive edge */
    USICR   =   _BV(USISIE)|_BV(USIOIE)|
                _BV(USIWM1)|_BV(USIWM0)|
                _BV(USICS1);
  }

  /* Clear tous les flags et arme l'interruption d'overfow
   * pour se déclencher au bout de 8 impulsions d'horloge (16 fronts) */
  USISR  =    _BV(USISIF)|_BV(USIOIF)|_BV(USIPF)|_BV(USIDC);
}

/* -----------------------------------------------------------------------------
 * Gestionnaire de comptage d'impulsions SCL
 * Cadence toutes les phases de la transmission, invalidé uniquement lorsqu'on
 * est en attente de START.
 */
ISR(USI_OVERFLOW_VECTOR) {

  if (USISR & _BV(USIPF)) {

    /* STOP reçu, re-intialise en attente de START */
    prvvInit();
  }
  else {
    uint8_t ucTxTail;
    uint8_t ucUSIDR = 0;

    switch (ucTwiUsiOverflowState) {

      /* ---------- Address mode ----------
       * Vérification de l'adresse et envoi ACK si correspondance.
       * Si pas de correspondance, retourne en attente START.
       */
      case USI_SLAVE_CHECK_ADDRESS:
        /* Si Appel général ou adresse me correspond */
        if ((USIDR == 0) || ((USIDR & ~0x01) == ucTwiSlaveAddress)) {

          if (USIDR & 0x01) {

            /* Le maître demande une lecture, il faut lui envoyer un octet */
            ucTwiUsiOverflowState = USI_SLAVE_SEND_DATA;
          }
          else {
            /* Le maître demande une écriture, on se prépare à recevoir */
            ucTwiUsiOverflowState = USI_SLAVE_REQUEST_DATA;
          }
          /* Transmission de l'ACK */
          prvvSendAck();
        }
        else {

          /* L'adresse transmise ne me correspond, retourne en attente START */
          prvvRelease();
        }
        break;

      /* ----- Master read data mode ------
       * Vérifie la réponse du maître à l'envoi de la dernière donnée (ACK),
       * transmet un nouvel octet si la maître le demande, sinon retourne en
       * attente START.
       */
      case USI_SLAVE_CHECK_REPLY_FROM_SEND_DATA:
        if (USIDR) {

          /* Le bit ACK est à 1 (NACK), le maître ne veut plus de données */
          prvvRelease();
          return;
        }
        /* Pas de break, c'est normal, le maître veut encore un octet */

      /* Recopie les données à transmettre dans le registre de transmission et
       * les envoies s'il y en a encore ou retourne en attente START si le buffer
       * de transmission est vide.
       */
      case USI_SLAVE_SEND_DATA:

        // Get data from Buffer
        ucTxTail = ucTwiUsiTxTail;
        if (ucTwiUsiTxHead == ucTxTail) {

          /* Le buffer de transmission est vide, appel du gestionnaire de
             transmission utilisateur s'il existe */
          if (pvTwiUsiSlaveTxHandler) {

            pvTwiUsiSlaveTxHandler();
            ucTxTail = ucTwiUsiTxTail;
          }
          /* Toujours pas de données à transmettre ! Retourne en attente START (NACK) */
          if (ucTwiUsiTxHead == ucTxTail) {

            prvvRelease();
            return;
          }
        }
        /* Encore des données à transmettre dans le buffer, on les envoie */
        ucTwiUsiOverflowState = USI_SLAVE_REQUEST_REPLY_FROM_SEND_DATA;
        ucTwiUsiTxTail = (ucTwiUsiTxTail + 1) & TWI_SLAVE_TX_BUFFER_MASK;
        USIDR = ucTwiUsiTxBuffer[ucTwiUsiTxTail];
        prvvSendData();
        break;

      /* Configure pour lire la réponse du maître  (ACK ou NACK ?) */
      case USI_SLAVE_REQUEST_REPLY_FROM_SEND_DATA:
        ucTwiUsiOverflowState = USI_SLAVE_CHECK_REPLY_FROM_SEND_DATA;
        prvvReadAck();
        break;

      /* ----- Master write data mode ------
       * Configure pour lire les données reçues et envoyer un ACK
       *** TODO -------
       * Que ce passe t'il si le buffer de réception est plein ? Ben il faut
       * envoyer un NACK non ?
       * A priori, ici on écrase les données les plus anciennes avec les nouvelles
       * Est-ce judicieux ?
       */
      case USI_SLAVE_REQUEST_DATA:
        ucTwiUsiOverflowState = USI_SLAVE_GET_DATA_AND_SEND_ACK;
        prvvReadData();
        break;

      /* Copie des données reçues dans le buffer et envoi d'un ACK */
      case USI_SLAVE_GET_DATA_AND_SEND_ACK:
        /* Stocke l'octet reçu dans le buffer */
        ucUSIDR = USIDR;
        ucTwiUsiRxHead = (ucTwiUsiRxHead + 1) & TWI_SLAVE_RX_BUFFER_MASK;
        ucTwiUsiRxBuffer[ucTwiUsiRxHead] = ucUSIDR;

        ucTwiUsiOverflowState = USI_SLAVE_REQUEST_DATA;
        prvvSendAck();
        break;
    }
  }
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vTwiUsiSlaveInit(uint8_t ucOwnAddress) {

  vFlushTxBuffers();
  vFlushRxBuffers();

  ucTwiSlaveAddress = ucOwnAddress;
  prvvInit();
}


// -----------------------------------------------------------------------------
void
vTwiUsiSlaveWrite(uint8_t ucData) {
  uint8_t ucHead;

  /* Calcul du pointeur en écriture sur le buffer */
  ucHead = (ucTwiUsiTxHead + 1) & TWI_SLAVE_TX_BUFFER_MASK;
  while (ucHead == ucTwiUsiTxTail)
    ; /* Attente d'une case libre dans le buffer */
  ucTwiUsiTxBuffer[ucHead] = ucData; // Mémorise dans le buffer
  ucTwiUsiTxHead = ucHead; // Mémorise le nouvel index
}

// -----------------------------------------------------------------------------
uint8_t
ucTwiUsiSlaveRead (void) {
  uint8_t ucTail;
  uint8_t ucRxTail;

  ucRxTail = ucTwiUsiRxTail;
  while (ucTwiUsiRxHead == ucRxTail)
    ; /* Attente tant que le buffer est vide */
  /* Calcul du pointeur en lecture sur le buffer */
  ucTail = (ucTwiUsiRxTail + 1) & TWI_SLAVE_RX_BUFFER_MASK;
  ucTwiUsiRxTail = ucTail; // Mémorise le nouvel index
  return ucTwiUsiRxBuffer[ucTail]; // renvoie l'octet lu dans le buffer
}

// -----------------------------------------------------------------------------
bool
xTwiUsiSlaveCharIsReceived (void) {
  uint8_t ucRxTail;

  ucRxTail = ucTwiUsiRxTail;
  // Renvoie false si le buffer est vide.
  return (ucTwiUsiRxHead != ucRxTail);
}

// -----------------------------------------------------------------------------
bool
xTwiUsiSlaveTxBufferIsEmpty (void) {
  uint8_t ucTxTail;

  ucTxTail = ucTwiUsiTxTail;
  // Renvoie true si le buffer est vide.
  return (ucTwiUsiTxHead == ucTxTail);
}

// -----------------------------------------------------------------------------
void
vTwiUsiSlaveRegisterTxHandler (vTwiUsiSlaveHandler pvUxerTxHandler) {

  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {

    pvTwiUsiSlaveTxHandler = pvUxerTxHandler;
  }
}

/* ========================================================================== */
#endif /* TWI_SLAVE_ENABLE defined */

#endif /* AVRIO_TWI_ENABLE defined */
/* ========================================================================== */
