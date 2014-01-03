AvrIO/Projects README file
$Id$
--------------------------------------------------------------------------------

L'utilisateur peut définir une configuration personnelle en utilisant le modèle
défini dans ce répertoire.

Vous pouvez, dans un premier temps, utiliser le script shell avrio-make
présent dans le répertoire pour créer un nouveau projet, par exemple :
$ ./avrio-make my-project
créé un nouveau projet nommé "my-project" avec la configuration par défaut.

Vous pouvez aussi préciser des valeurs différentes à ses paramètres grâce aux
options de avrio-make:
  -h    Affiche ce message
  -t    type de projet à créer (avrio ou arduino)
        Valeur par défaut: avrio
  -p    modèle du microcontrôleur conforme à ceux indiqués par la commande
        avr-gcc --target-help (paragraphe Known MCU names)
        Valeur par défaut: atmega328p
  -F    fréquence d'horloge du MCU en Hz
        Valeur par défaut: 16000000
  -c    modèle de programmateur conforme à ceux indiqués par la commande
        avrdude -c ?
        Valeur par défaut: uno
  -P    port utilisé par le programmateur (dépend du programmateur utilisé et
        de la plate-forme de développement)
        Valeur par défaut: /dev/ttyACM0
  -b    vitesse de la liaison série (uniquement si -P correspond à un port série)
        Valeur par défaut: Aucune
  -a    Indique que le projet ne se trouve pas dans un sous-répertoire de la
        racine de AvrIO (chemin de projet absolu).
  -B    Carte cible prédéfinie
        Valeur par défaut: Aucune (board.mk et board/ sont créés)

Vous devrez éventuellement vérifier/compléter/modifier les fichiers avrio-board se
trouvant dans le sous-répertoire board en fonction, par exemple, des broches
utilisées pour tel ou tel module (led, button, adc, lcd ...)

La dernière version de la documentation AvrIo peut être consultée sur:
                http://www.epsilonrt.com/avrio/dev
