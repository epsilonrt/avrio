/**
 * @file avrio-cfg-afsk.h
 * @brief Fichier de configuration du module modem AFSK1200
 * @author Francesco Sacchi <batt@develer.com>
 *          @copyright 2009 GNU General Public License version 2
 *          See the notice below.
 * @author Pascal JEAN <pjean@btssn.net>
 *          @copyright 2014 GNU Lesser General Public License version 3
 *          <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20120519 - Initial version from BeRTOS
 * -----------------------------------------------------------------------------
 * Bertos is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * As a special exception, you may use this file as part of a free software
 * library without restriction.  Specifically, if other files instantiate
 * templates or use macros or inline functions from this file, or you compile
 * this file and link it with other files to produce an executable, this
 * file does not by itself cause the resulting executable to be covered by
 * the GNU General Public License.  This exception does not however
 * invalidate any other reasons why the executable file might be covered by
 * the GNU General Public License.
 *
 * Copyright 2009 Develer S.r.l. (http://www.develer.com/)
 */
#ifndef _AVRIO_CFG_AFSK_H_
#define _AVRIO_CFG_AFSK_H_

/*
 * La configuration du module est effectuée dans le fichier avrio-cfg-afsk.h
 * Ce fichier doit se trouver dans le chemin de recherche des fichiers en-tête
 * (répertoire du projet ou dossier board normalement). \n
 */

/*
 * Constante à utiliser pour la configuration du module dans le fichier
 * avrio-cfg-afsk.h grâce à la macro CONFIG_AFSK_FILTER
 */
#define AFSK_BUTTERWORTH  0
#define AFSK_CHEBYSHEV    1

/*
 * Configuration du filtre numérique à utiliser par le module
 */
#define CONFIG_AFSK_FILTER AFSK_CHEBYSHEV

/*
 * Configuration de la taille du buffer de réception utilisé par le module
 */
#define CONFIG_AFSK_RX_BUFLEN 32

/*
 * Configuration de la taille du buffer d'émission utilisé par le module
 */
#define CONFIG_AFSK_TX_BUFLEN 32

/*
 * Confguration de la fréquence d'échantillonage du convertisseur numérique-
 * analogique en Hz.
 */
#define CONFIG_AFSK_DAC_SAMPLERATE 9600

/*
 * Configuration du timeout de réception en ms (-1 pour le désactiver).
 */
#define CONFIG_AFSK_RXTIMEOUT -1

/*
 * Configuration de la longueur du préambule en ms avant de débuter la
 * transmission de la trame. Le nombre arrondi de flags HDLC est ajouté avant
 * la trame afin de permettre la synchronisation correcte du récepteur.
 */
#define CONFIG_AFSK_PREAMBLE_LEN 300UL

/*
 * Configuration de la longueur de la queue de trame en ms avant de terminer la
 * transmission de la trame. Le nombre arrondi de flags HDLC est ajouté après
 * la trame afin de permettre la synchronisation correcte du récepteur.
 */
#define CONFIG_AFSK_TRAILER_LEN 50UL

/* ========================================================================== */
#endif /* _AVRIO_CFG_AFSK_H_ */
