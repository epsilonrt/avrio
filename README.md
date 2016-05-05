# AvrIo
*Librairie et outils pour ATMEL AVR*
---
Copyright 2010-2016 (c), epsilonRT

AvrIO est un projet Open Source sous 
[CeCILL Free Software License Version 2.1](http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.html)
dont l'objectif est d'offrir une bibliothèque C pour les microcontrôleurs AVR 
d'Atmel. AvrIO est développé en C et en assembleur. 
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
* fabriquées et conçue par moi ou mes étudiants :
    - Carte mère du robot mAVRix
    - Cartes du projet DomoNET : Hub, Ihm du serveur, Capteur et Télécommande
    - Cartes du projet Toueris : Ihm et RTC
    - Cartes des mini-projets de BTS Systèmes Électroniques Session 2011 (Projet 2 à 7)

AvrIO complète la bibliothèque C standard 
[avr-libc](http://www.nongnu.org/avr-libc/) dans plusieurs domaines:

* Le multitâche en fournissant par exemple un ordonnanceur collaboratif, des 
  dispositifs de synchronisation (sémaphores, mutex) ou de communication 
  (tube FIFO) ...
* Les entrées-sorties comme, par exemple, les afficheurs LCD, les claviers, 
  les terminaux ...
* Les réseaux : CAN, I2C, ModBUS ...
* La robotique comme, par exemple, la commande PWM des moteurs DC, 
  les encodeurs, les filtres d'asservissement PID ...
* Le traitement du signal avec le calcul de transformée de Fourier 
  rapide (FFT) ...

Vous pouvez récupérer la toute dernière version de développement grâce à git :\n

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
