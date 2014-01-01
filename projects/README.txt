AvrIO/Projects README file
$Id$
--------------------------------------------------------------------------------

L'utilisateur peut définir une configuration personnelle en utilisant le modèle
défini dans ce répertoire.

Vous pouvez, dans un premier temps, utiliser le script shell avrio-new-project
présent dans le répertoire pour créer un nouveau projet, par exemple :
$ avrio-new-project my-project
créé un nouveau projet nommé "my-project" avec la configuration par défaut
suivante correspondant à une carte ARDUINO UNO Rev. 3 :
  * MCU   = atmega328p
  * F_CPU = 16000000
  * AVRDUDE_PROGRAMMER = arduino
  * AVRDUDE_PORT = /dev/ttyACM0

Vous pouvez aussi préciser des valeurs différentes à ses paramètres grâce aux
options de avrio-new-project:
  * -p : modèle du microcontrôleur conforme à ceux indiqués par la commande
         avr-gcc --target-help (paragraphe Known MCU names)
  * -F : fréquence d'horloge du MCU en Hz
  * -c : modèle de programmateur conforme à ceux indiqués par la commande
         avrdude -c ?
  * -P : port utilisé par le programmateur (dépend du programmateur utilisé et
         de la plate-forme de développement)
  * -b : vitesse de la liaison série (uniquement si -P correspond à un port série)

Vous devrez aussi vérifier/compléter/modifier les fichiers avrio-board se
trouvant dans le sous-répertoire board en fonction, par exemple, des broches
utilisées pour tel ou tel module (led, button, adc, lcd ...)

