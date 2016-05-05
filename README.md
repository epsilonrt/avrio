# AvrIo

*Bibliothèque C modulaire pour ATMEL AVR*

---
Copyright 2010-2016 (c), epsilonRT

<a href="http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.html">
  <img src="https://raw.githubusercontent.com/epsilonrt/gxPL/master/doc/images/osi.png" alt="osi.png" align="right" valign="top">
</a>

AvrIO est un projet Open Source sous
[CeCILL Free Software License Version 2.1](http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.html)
dont l'objectif est d'offrir une bibliothèque C pour les microcontrôleurs AVR
d'Atmel.<br>
AvrIO est développé en C et en assembleur.<br>
Il utilise la chaîne de développement GNU GCC disponible sur 
la plupart des plateformes : Windows, Linux et autres Unix like, MacOS...

**Pourquoi AvrIO ?**

Lorsque l'on souhaite developper un projet utilisant un microcontrôleur, on se
heurte rapidement à la problématique de choisir un système de prototypage.
Il existe un grand nombre de cartes prototypes dont la plus connue dans le monde
ATMEL est sans nul doute ARDUINO.

Certes cette solution est séduisante car elle
fournit une solution "Tout-en-Un" à l'amateur soucieux d'arriver rapidement à 
une solution en DIY.

Le problème se complique lorsqu'on souhaite passer son projet à une étape 
un peu plus professionnelle, voir industrielle en developpant une carte spécifique.

On a alors 2 choix possibles:

1. respecter l'architecture matérielle d'une carte ARDUINO et utiliser la 
  librairie ARDUINO, ce qui est loin d'être souple, ou
2. modifier la librairie ARDUINO pour l'adapter à la nouvelle carte, ce qui est
  loin d'être simple (car la librairie ARDUINO n'est pas prévue pour cela).
  
Si on ajoute à cela:

1. un fort couplage entre l'[IDE](https://fr.wikipedia.org/wiki/Environnement_de_d%C3%A9veloppement) 
  ARDUINO et sa librairie (on est plus ou moins obliger d'utiliser l'IDE prévue...)
2. L'absence de prise en compte de l'utilisation d'un 
  [débogueur](https://fr.wikipedia.org/wiki/D%C3%A9bogueur) comme 
  [AVR Dragon](http://www.atmel.com/tools/AVRDRAGON.aspx) ou
  [JTAG ICE](http://www.atmel.com/tools/JTAGICE3.aspx),

cela fait beaucoup dans un contexte professionnel...

AvrIO fournit une solution élégante permettant de gommer les différences entre
le code mis au point lors de la phase de prototypage sur une carte du type 
d'ARDUINO (DVK, USBKEY, STK ...) et la phase de production sur la carte finale.
Il reprend en cela les avantages de la 
[conception orienté "Objet"](https://fr.wikipedia.org/wiki/Programmation_orient%C3%A9e_objet)

En clair, aucune ligne de code de l'application mise au point lors du prototypage
n'est modifiée pour la solution finale.

Cela est rendu possible en effectuant une séparation stricte entre le code et
la description matérielle. Cette description est réaliséé grâce à des fichiers
d'entête dans le dossier **board**. Le développeur aura donc uniquement à créer
un nouveau dossier **board** pour sa nouvelle carte en utilisant comme modèles 
les nombreux exemples du dossier 
[board](https://github.com/epsilonrt/avrio/tree/master/board) d'AvrIO.

AvrIO n'est pas lié à un IDE spécifique, on peut l'utiliser en ligne de 
commande (**make**, **make program**, **make debug** ...), avec 
[Geany](https://www.geany.org/), 
[AvrStudio 4](http://www.atmel.com/tools/studioarchive.aspx), 
[CodeLite](http://www.codelite.org/), [Eclipse](https://eclipse.org/)... 

Il suffit que l'IDE en question ai prévu l'intégration de projet utilisant un 
Makefile.

Pour ma part, j'ai une préférence pour [CodeLite](http://www.codelite.org/), 
qui est solution multiplateforme et puissante. [CodeLite](http://www.codelite.org/)
me permet sous Linux un débogage via [GDB](https://www.gnu.org/software/gdb/) 
avec des fonctionnalités très proches d'AVR Studio 4 sous Windows... Mais il 
ne s'agit là que d'un avis personnel.

AvrIO est développé :

* de façon modulaire, c'est à dire, qu'il est découpé en modules. 
  Chaque module peut être validé ou non, **configuré et adapté facilement** 
  grâce à des fichiers séparés du code source. Les dépendances entre modules 
  sont limitées au stricte nécessaire.
* de façon à réduire l'occupation mémoire que ce soit RAM ou FLASH permettant 
  son utilisation sur les modèles les plus économiques de la famille AVR (tiny).
* de façon à intégrer d'autres projets Open Source comme par exemple 
  [freemodbus](http://freemodbus.berlios.de/), 
  [Arduino](https://github.com/arduino/Arduino) ou 
  [AvrX](http://www.epsilonrt.com/avrio/group__avrx__module.html) 
  sans modification de leur licence.

AvrIO complète la bibliothèque C standard 
[avr-libc](http://www.nongnu.org/avr-libc/) dans plusieurs domaines:

* Les entrées-sorties:
    - les broches [numériques](http://www.epsilonrt.com/avrio/group__dpin__module.html) et 
      [analogiques](http://www.epsilonrt.com/avrio/group__adc__module.html),
    - les [leds](http://www.epsilonrt.com/avrio/group__led__module.html), 
    - les [leds RGB](http://www.epsilonrt.com/avrio/group__ledrgb__module.html), 
    - les [boutons poussoirs](http://www.epsilonrt.com/avrio/group__button__module.html), 
    - les [micro-switchs](http://www.epsilonrt.com/avrio/group__switch__module.html), 
    - les [relais bistable](http://www.epsilonrt.com/avrio/group__bisrelay__module.html), 
    - les [afficheurs LCD](http://www.epsilonrt.com/avrio/group__lcd__module.html), 
    - les [claviers](http://www.epsilonrt.com/avrio/group__keyb__module.html), 
    - les [compteurs](http://www.epsilonrt.com/avrio/group__counter__module.html), 
    - les [interruptions](http://www.epsilonrt.com/avrio/group__irq__module.html), 
    - les [RTC](http://www.epsilonrt.com/avrio/group__rtc__module.html),
    - les [cartes mémoires SD](http://www.epsilonrt.com/avrio/group__mmc__module.html)...
* Les capteurs analogiques:
    - [linéaires et non linéaires](http://www.epsilonrt.com/avrio/group__adc__sensor__module.html)
    - [CTN](http://www.epsilonrt.com/avrio/group__ntc__module.html), 
    - [MQ135](http://www.epsilonrt.com/avrio/group__mq135__module.html), 
    - [TEMT6000](http://www.epsilonrt.com/avrio/group__temt__module.html), 
* Les capteurs numériques: 
    - [HIH6130](http://www.epsilonrt.com/avrio/group__hih6130__module.html), 
    - [HSC](http://www.epsilonrt.com/avrio/group__hsc__module.html), 
    - [DS1621](http://www.epsilonrt.com/avrio/group__ds1621__module.html), 
    - [TSL230](http://www.epsilonrt.com/avrio/group__tsl230__module.html) ...
* Les bus [I2C](http://www.epsilonrt.com/avrio/group__twi__group.html) 
  et [SPI](http://www.epsilonrt.com/avrio/group__spi__module.html)
* Les réseaux: 
    - Liaisons série
      [matérielles](http://www.epsilonrt.com/avrio/group__serial__module.html) avec
      gestion du RS485 et [logicielles](file:///home/pascal/src/avrio/doc/html/group__serial__sw__module.html), 
    - [XBee](http://www.epsilonrt.com/avrio/group__avrio__xbee.html) série 1 (IEEE802.15.4) et série 2 (Zigbee), 
    - [Ax25](http://www.epsilonrt.com/avrio/group__ax25__module.html), 
    - [AFSK](http://www.epsilonrt.com/avrio/group__afsk__module.html) ...
* La robotique:  
    - la commande PWM des [moteurs DC](http://www.epsilonrt.com/avrio/group__bdcm__module.html), 
    - les [servo-moteurs RC](http://www.epsilonrt.com/avrio/group__servo__module.html), 
    - les [encodeurs incrémentaux](http://www.epsilonrt.com/avrio/group__encoder__module.html),  
    - les [accéléromètres](http://www.epsilonrt.com/avrio/group__acc3d__module.html), 
    - les [compas magnétiques](http://www.epsilonrt.com/avrio/group__comp3d__module.html), 
    - les [gyroscopes](http://www.epsilonrt.com/avrio/group__gyro3d__module.html), 
    - les [centrales intertielles](http://www.epsilonrt.com/avrio/group__imu__module.html) ...
* Le traitement du signal avec 
    - les filtres d'asservissement [PID](http://www.epsilonrt.com/avrio/group__pid__module.html), 
    - les filtres de [Kalman](http://www.epsilonrt.com/avrio/group__kalman__module.html),
    - l'algorithme [DCM](http://www.epsilonrt.com/avrio/group__dcm__module.html) "Direction Cosine Matrix" ...
* Le multitâche en fournissant par exemple un 
  [ordonnanceur collaboratif](http://www.epsilonrt.com/avrio/group__task__module.html), 
  des dispositifs de synchronisation ([sémaphores](http://www.epsilonrt.com/avrio/group__semaphore__module.html), 
  [mutex](http://www.epsilonrt.com/avrio/group__mutex__module.html)) 
  ou de communication (tube [FIFO](http://www.epsilonrt.com/avrio/group__queue__module.html)), 
  les [listes doublement chaînées](http://www.epsilonrt.com/avrio/group__avrio__dlist.html),
  les [vecteurs](http://www.epsilonrt.com/avrio/group__avrio__vector.html) ...

AvrIO est pré-configurée pour un grand nombre de cartes cibles industrielles 
mais peut être adapté facilement à une carte cible personnelle :

* fabriquées par ATMEL
    - [STK200 Starter Kit](http://www.atmel.com/dyn/resources/prod_documents/doc1107.pdf)
    - [AT90USB Evaluation Kit](http://www.atmel.com/dyn/resources/prod_documents/doc7627.pdf)
    - [AVR Butterfly Evaluation Kit](http://www.atmel.com/dyn/resources/prod_documents/doc4271.pdf)
    - [DVK90CAN1 - AT90CAN128 Development Kit](http://www.atmel.com/dyn/resources/prod_documents/doc4381.pdf)
* fabriquées par OLIMEX
    - [AVR-USB-STK development board](http://www.olimex.com/dev/pdf/AVR/AVR-USB-STK.pdf)
    - [AVR-CAN development board](http://www.olimex.com/dev/pdf/AVR/AVR-CAN.pdf)
* fabriquées par EGNITE
    - [Ethernut 1.3](http://www.ethernut.de/pdf/enhwm13e.pdf)

Vous pouvez récupérer la toute dernière version de développement grâce à git :

    $ git clone http://github.com/epsilonrt/avrio.git

##Installation rapide

    git clone http://github.com/epsilonrt/avrio.git
    cd avrio
    sudo make install
    
    make set-profile
    # Remove AVRIO_ROOT=... in /home/pascal/.profile
    # AVRIO_ROOT=/home/pascal/src/avrio was added
    #  in /home/pascal/.profile
    # You must log out for this to take effect.
