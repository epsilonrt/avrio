AvrIO/Projects README file
$Id$
--------------------------------------------------------------------------------

L'utilisateur peut définir une configuration personnelle en utilisant le modèle
défini dans ce répertoire.

Vous pouvez, dans un premier temps, utiliser le script shell avrio-make
présent dans le répertoire pour créer un nouveau projet, par exemple :
$ avrio-make my-project
créé un nouveau projet nommé "my-project" avec la configuration par défaut.

Vous pouvez aussi préciser des valeurs différentes à ses paramètres grâce aux
options de avrio-make:
  -h    Affiche ce message
  -t    type de projet à créer (avrio ou arduino)
        Valeur par défaut: $DEFAULT_TYPE
  -B    Carte cible utilisée pour le projet (BOARD)
        Les valeurs possibles sont énumérées par la commande
        $(basename $0) -B ?
        Les valeurs les plus courantes sont : UNO, STK200, USBKEY ...
        Valeur par défaut: Aucune ce qui correspond à une carte cible personnalisée
        (board.mk et board/ sont créés)
  -c    modèle de programmateur conforme à ceux indiqués par la commande
        avrdude -c ?
        Valeur par défaut: $DEFAULT_AVRDUDE_PROGRAMMER
  -P    port utilisé par le programmateur (dépend du programmateur utilisé et
        de la plate-forme de développement)
        Valeur par défaut: $DEFAULT_AVRDUDE_PORT
  -b    vitesse de la liaison série (uniquement si -P correspond à un port série)
        Valeur par défaut: Aucune
  -p    modèle du microcontrôleur conforme à ceux indiqués par la commande
        avr-gcc --target-help (paragraphe Known MCU names)
        Cette option est prise en compte uniquement si -B n'est pas utilisé.
        Valeur par défaut: $DEFAULT_MCU
  -F    fréquence d'horloge du MCU en Hz
        Valeur par défaut: $DEFAULT_FCPU
        Cette option est prise en compte uniquement si -B n'est pas utilisé.
  -a    Indique que le projet ne se trouve pas dans un sous-répertoire de la
        racine de AvrIO (chemin de projet absolu).

Vous devrez éventuellement vérifier/compléter/modifier les fichiers avrio-board se
trouvant dans le sous-répertoire board en fonction, par exemple, des broches
utilisées pour tel ou tel module (led, button, adc, lcd ...)

La dernière version de la documentation AvrIo peut être consultée sur:
                http://www.epsilonrt.com/avrio/dev
