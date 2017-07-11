Tutoriel

Ce document a pour objectif de faciliter la prise en main d'AvrIO.

## Installation des logiciels

Pour utiliser AvrIO sous Windows, il faut disposer d'un environnement de
développement correctement configuré. C'est un peu plus compliqué que sous
linux mais il nous faut :

* le compilateur avr-gcc et ses outils (on parle de toolchain)
* les outils de développement unix (make, git ...) et un ligne de commande unix (bash)
* un éditeur de texte IDE.

Pour faire simple, ATMEL a développé AvrStudio qui est un logiciel gratuit
tout à fait compatible avec AvrIO. Son gros avantage est de fournir une
fonction mise au point (debug) capable de gérer les débuggers les plus courants
(dont AVR Dragon). Son principal inconvénient est sa lourdeur, raison pour
laquelle on pourra aussi utiliser des logiciels plus léger comme CodeLite ou
même Geany.

_Remarque_: ATMEL a été racheté par Microchip, ceci aura peut-être des
conséquences sur l'avenir de AvrStudio....

**Si vous ne disposez pas de ces logiciels sur votre ordinateur**, il faut les
télécharger et les installer.

Si les logiciels sont déjà installés vous pouvez passer au paragraphe **Configuration de l'environnement**.

Pour procéder à l'installation, il faut avoir un compte avec les droits
`Administrateur`, le mieux est d'avoir un compte personnel non partagé avec d'autres personnes qui développent avec AvrIO.

### Téléchargements

* Télécharger la dernière version de `AvrStudio` (qui inclut avr-gcc et ses
outils, make et évidement l'IDE) sur :

    <http://www.microchip.com/development-tools/atmel-studio-7>

* Télécharger la dernière version de`Git for Windows` qui inclut git, bash et
les utilitaires unix (sed, awk ...) sur :

    <https://git-scm.com/download/win>

Choisir la version correspondant à votre version de Windows
(Git-X.XX.X.X-64-bit.exe pour un système 64 bits ...)

### Installation

#### AvrStudio 7

Le mieux est d'avoir une version AvrStudio supérieure ou égale à 7.0.1188

**Remarque importante**

Si vous choisissez la version 7.0.1188, il est possible que l'installation
bloque à la mise à jour `KB3033929 Windows Update`, dans ce cas utiliser la
commande ci-dessous pour lancer l'installation:

<pre class="fragment">
  as-installer-7.0.1188-full.exe INSTALLDIR=C:\Info\Atmel\Studio /SystemValidationErrorAsWarning
</pre>

Pour utiliser la toolchain avec `bash` il ne faut pas installer dans `Program Files`
mais dans un chemin sans espace (`C:\Info` par exemple) :

<img src="https://raw.githubusercontent.com/epsilonrt/avrio/master/doc/images/as7-1.png" alt="Installation AvrStudio 7">

**ATTENTION** Il faut respecter la consigne ci-dessus !

Lors de l'installation :

* Sélectionner uniquement l'architecture `AVR 8-bit MCU`
* Confirmer l'installation des drivers Atmel et Microchip


#### Git for Windows

Il est préférable de ne pas installer dans `Program Files`
mais dans un chemin sans espace (`C:\Info` par exemple) :

<img src="https://raw.githubusercontent.com/epsilonrt/avrio/master/doc/images/git-1.png" alt="Installation Git for Windows">

Lors de l'installation valider les choix par défaut sauf :

* cocher la case `Use a TrueType font in all console windows` pour la sélection des composants,
* choisir `checkout as-is, commit as-is` pour la configuration des fins de lignes,
* cocher la case `Enable symbolic links` pour les options supplémentaires (extra)

#### CodeLite et AvrDUDE

En complément des programmes précédents, il peut être intéressant d'installer
quelques éléments supplémentaires:

* Un IDE plus léger que AvrStudio: CodeLite
* Le programme `avrdude` permettant de programmer les microcontrôleurs ATMEL à partir
de la ligne de commande `bash`, de CodeLite et même de Geany.
* La librairie `libusb-win32-devel-filter` qui permet d'utiliser les programmateurs
USB (Avr Dragon, Avr ICE ...) en dehors de AvrStudio.

On commence par le téléchargement :

* Télécharger la dernière version de CodeLite sur <https://downloads.codelite.org>
on choisira la version `Weekly Build` 64bits en conformité avec notre version
de Windows.
* Télécharger la version modifiée `epsilonrt` de AvrDUDE sur
<http://www.epsilonrt.fr/files/avrdude-6.3-mingw32-epsilonrt.zip> (cette version
prend en charge les derniers modèles AVR8 (mega328PB, tiny841 ...)
* Télécharger la dernière version de `libusb-win32-devel-filter` sur
<https://sourceforge.net/projects/libusb-win32/files/libusb-win32-releases>

Puis on passe à l'installation:

* Décompresser et installer CodeLite avec les options par défaut
* Décompresser le fichier `avrdude-6.3-mingw32-epsilonrt.zip` et copier les
fichiers dans dossier `C:\Info\Atmel\Studio\7.0\shellutils`
* Installer `libusb-win32-devel-filter` avec les options par défaut

Il faut enfin configurer `libusb-win32-devel-filter` pour lui ajouter le
programmateur qu'on utilise:

* Connecter le programmateur que vous utilisez habituellement (Avr Dragon ici)
* Lancer le `LibUSB Filter Wizard` puis choisir `Install a device filter`
* Choisir votre programmateur dans la liste et cliquer sur `Install`

<img src="https://raw.githubusercontent.com/epsilonrt/avrio/master/doc/images/libusb-1.png" alt="Installation d'un filtre LibUSB pour un AVR Dragon">

## Configuration de l'environnement

Pour utiliser AvrIO, il faut ajouter le chemin vers la toolchain et ses utilitaires dans le `PATH` système, pour ce faire :

* cliquer droit sur `Ordinateur`, puis `Propriétés`
* cliquer sur `Propriétés système avancées`
* cliquer sur `Variables d'environnement`
* cliquer sur la variable `Path` dans la liste des `Variables système`
* cliquer sur le bouton `Modifier` et ajouter le chemin de la toolchain et de
ses utilitaires à la fin du `Path` (il faut ajouter un point virgule avant
chaque chemin ajouté). Si vous avez installé dans `C:\Info` comme conseillé ci-dessus, il faut ajouter
le texte ci-dessous à la fin du `Path`:

<pre class="fragment">
        ;C:\Info\Atmel\Studio\7.0\shellutils;C:\Info\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin
</pre>

Puis, il faut ajouter des variables d'environnement, pour ce faire :

* cliquer droit sur `Ordinateur`, puis `Propriétés`
* cliquer sur `Propriétés système avancées`
* cliquer sur `Variables d'environnement`
* cliquer sur le bouton `Nouvelle...` sous la liste des `Variables utilisateur pour...`
* compléter `Nom de la variable` : **HOME**
* compléter `Valeur de la variable` : **%USERPROFILE%** puis cliquer sur `Ok`
* ajouter une autre variable utilisateur **AVRIO_ROOT** avec la valeur
**%USERPROFILE%/src/avrio**

Voilà ce que cela devrait afficher :

<img src="https://raw.githubusercontent.com/epsilonrt/avrio/master/doc/images/env-4.png" alt="Configuration environnement">

Il reste à vérifier que tout fonctionne. Lancer Git Bash et exécuter les
commandes ci-dessous (les commandes à taper sont à droite des `$` qui ne doivent pas être tapés ! `EtudiantSN` désigne l'identifiant de
l'utilisateur en cours):

<pre class="fragment">
  EtudiantSN@OBIWAN MINGW64 ~
  $ pwd
  /c/Users/EtudiantSN

  EtudiantSN@OBIWAN MINGW64 ~
  $ echo "$HOME-$AVRIO_ROOT"
  /c/Users/EtudiantSN-C:\Users\EtudiantSN/src/avrio

  EtudiantSN@OBIWAN MINGW64 ~
  $ avr-gcc --version
  avr-gcc.exe (AVR_8_bit_GNU_Toolchain_3.6.0_1734) 5.4.0
  Copyright (C) 2015 Free Software Foundation, Inc.
  This is free software; see the source for copying conditions.  There is NO
  warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
</pre>


Pour la prise en charge des caractères accentués dans Git bash, il faut configurer
la fenêtre Git Bash:

* cliquer droit sur la barre de titre de la fenêtre, puis `Options...`
* sélectionner l'item `Text` et choisir `Locale` **fr_FR**, **UTF-8** pour `Character Set`
* Appliquer et enregistrer.

## Clonage et installation de AvrIO

L'installation de AvrIO se fait à partir de Git Bash. Lancer Git Bash et
exécuter les commandes ci-dessous:

* On commence par créer un répertoire par: `mkdir ~/src`
* puis on y descend: `cd ~/src`
* puis on clone le dépôt: `git clone http://github.com/epsilonrt/avrio.git`
* puis on y descend: `cd avrio`
* enfin on installe AvrIO: `make install` (pas de sudo !)

Voilà la fin de la procédure:

<pre class="fragment">
  EtudiantSN@OBIWAN MINGW64 ~/src
  $ git clone http://github.com/epsilonrt/avrio.git
  Cloning into 'avrio'...
  warning: redirecting to https://github.com/epsilonrt/avrio.git/
  remote: Counting objects: 22430, done.
  remote: Compressing objects: 100% (33/33), done.
  remote: Total 22430 (delta 8), reused 0 (delta 0), pack-reused 22397
  Receiving objects: 100% (22430/22430), 119.27 MiB | 943.00 KiB/s, done.
  Resolving deltas: 100% (14482/14482), done.
  Checking out files: 100% (2304/2304), done.

  EtudiantSN@OBIWAN MINGW64 ~/src
  $ cd avrio

  EtudiantSN@OBIWAN MINGW64 ~/src/avrio (master)
  $ make install
  C:/Info/Atmel/Studio/7.0/shellutils/make -w -C util/bin install
  make[1]: Entering directory 'C:/Users/EtudiantSN/src/avrio/util/bin'
  [UNINSTALL] utils from /usr/local
  [INSTALL] utils in /usr/local
  make[1]: Leaving directory 'C:/Users/EtudiantSN/src/avrio/util/bin'
  C:/Info/Atmel/Studio/7.0/shellutils/make -w -C util/bin install
  make[1]: Entering directory 'C:/Users/EtudiantSN/src/avrio/util/bin'
  [UNINSTALL] utils from /usr/local
  [INSTALL] utils in /usr/local
  make[1]: Leaving directory 'C:/Users/EtudiantSN/src/avrio/util/bin'
  Installed for MINGW64
</pre>

## Utilisation de AvrIO

AvrIO peut être utilisé avec n'importe quel IDE prenant en charge les fichiers
`Makefile` (AvrStudio 7, CodeLite, Geany, Eclipse ...).

### Création d'un nouveau projet

Pour l'instant, la **création** d'un nouveau projet se fait exclusivement par
la ligne de commande Git `bash`

La commande à utiliser est `avrio-prj`, on peut avoir de l'aide sur cette
commande à l'aide de son option `-h`:

<pre class="fragment">
  EtudiantSN@OBIWAN MINGW64 ~/src/avrio (master)
  $ avrio-prj -h
  usage: avrio-prj [Options] [project_name]

  Ce script permet de créer un nouveau projet pour AvrIo.

    project_name  nom du projet à créer. Si le nom de projet n'est pas fourni,
    c'est le nom du répertoire courant qui est utilisé.
    Sans l'option -s, seul un fichier projet CodeLite/AvrStudio est créé.

  OPTIONS:
    -h  Affiche ce message
    -f  Force l'écrasement de fichiers existants.
    -g  Ajoute les fichiers créés au dépôt git (si projet git)
    -s  Créé aussi un fichier Makefile et un fichier source C (ou C++)
    -4  Le fichier projet AvrStudio est pour la version 4 (Version 7 par défaut)
    -b MYPROJECT      Création d'une nouvelle carte MYPROJECT (nom en majuscule sans espace)
    -p partno         micro-contrôleur de la nouvelle carte (atmega328p par défaut)
    -F freq           fréquence oscillateur  de la nouvelle carte (16000000 par défaut)
    -c programmer-id  programmateur de la nouvelle carte (dragon_isp par défaut)
    -P port           port de programmation de la nouvelle carte (usb par défaut)
    -L liste les cartes connues par AvrIO
</pre>

#### Projet pour une carte Arduino

On peut maintenant tester la création d'un nouveau projet AvrIO et sa
compilation:

* on crée un nouveau répertoire pour notre projet dans `src` par:
`mkdir ~/src/monprojet` (Pas d'espaces !!!)
* puis on y descend: `cd ~/src/monprojet`
* puis on crée le projet: `avrio-prj -s monprojet`
* une commande `ls` nous permet de voir les fichiers créés:


<pre class="fragment">
        EtudiantSN@OBIWAN MINGW64 ~/src/monprojet
        $ ls
        Makefile  monprojet.c  monprojet.cproj  monprojet.project
</pre>


* `Makefile` est le fichier qui permet de compiler
* `monprojet.c` est le fichier source à modifier qui contient pour l'instant
le code pour faire clignoter une led
* `monprojet.cproj` est le fichier projet pour AvrStduio 7
* `monprojet.project` est le fichier projet pour Codelite

On peut le compiler directement sur la ligne de commande avec `make`:

<pre class="fragment">
  EtudiantSN@OBIWAN MINGW64 ~/src/monprojet
  $ make
  avr-gcc (AVR_8_bit_GNU_Toolchain_3.6.0_1734) 5.4.0
  Copyright (C) 2015 Free Software Foundation, Inc.
  This is free software; see the source for copying conditions.  There is NO
  warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

  [CC]  monprojet.c
  [LINK]  monprojet.elf
  [FLASH]  monprojet.hex
  [EEPROM]  monprojet.eep
  [LINK]  atmega328p/monprojet.elf
  [LISTING]  atmega328p/monprojet.lss
  [SYMBOL]  atmega328p/monprojet.sym
  C:\Info\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-nm.exe: atmega328p/monprojet.elf: no symbols

  [SIZE]
  AVR Memory Usage
  ----------------
  Device: atmega328p

  Program:     160 bytes (0.5% Full)
  (.text + .data + .bootloader)

  Data:          0 bytes (0.0% Full)
  (.data + .bss + .noinit)
</pre>

Si on a une carte Arduino connectée (la carte Arduino étant vue comme un port COM, il n'est pas nécessaire d'avoir LibUSB installé mais il faut AvrDUDE !), on peut la programmer directement à partir de la ligne de commande avec
`make program`:

<pre class="fragment">
  $ make program
  [CC]  monprojet.c
  [LINK]  monprojet.elf
  [FLASH]  monprojet.hex
  [EEPROM]  monprojet.eep
  avrdude -p atmega328p -P COM4 -c arduino    -U flash:w:monprojet.hex

  avrdude: AVR device initialized and ready to accept instructions

  Reading | ################################################## | 100% 0.00s

  avrdude: Device signature = 0x1e950f (probably m328p)
  avrdude: NOTE: "flash" memory has been specified, an erase cycle will be performed
           To disable this feature, specify the -D option.
  avrdude: erasing chip
  avrdude: reading input file "monprojet.hex"
  avrdude: input file monprojet.hex auto detected as Intel Hex
  avrdude: writing flash (160 bytes):

  Writing | ################################################## | 100% 0.05s

  avrdude: 160 bytes of flash written
  avrdude: verifying flash memory against monprojet.hex:
  avrdude: load data flash data from input file monprojet.hex:
  avrdude: input file monprojet.hex auto detected as Intel Hex
  avrdude: input file monprojet.hex contains 160 bytes
  avrdude: reading on-chip flash data:

  Reading | ################################################## | 100% 0.04s

  avrdude: verifying ...
  avrdude: 160 bytes of flash verified

  avrdude done.  Thank you.
</pre>

Si cela ne fonctionne pas, c'est qu'il faut modifier le port série utilisé par
AvrDUDE pour communiquer avec la carte Arduino. Pour cela ouvrir le fichier
`Makefile`, dé-commenter et modifier la ligne `AVRDUDE_PORT = COM4` pour qu'elle
corresponde au port COM de la carte Arduino.

Il existe plusieurs autres cibles à fournir à make, par exemple:

* `make rebuild` permet de recompiler complètement le projet
* `make clean` permet de nettoyer le projet
* `make distclean` permet de nettoyer complètement le projet (pour le remettre
dans son état initial).

#### Utilisation sous CodeLite

Comme on l'a vu précédemment, un projet AvrIO créé un fichier .project permettant
de l'utiliser avec CodeLite:

* Lancer CodeLite, si la configuration se lance laisser tous les choix par défaut
* Créer un nouvel espace de travail C++ (Workspace) à l'aide du menu Workspace,
choisir le dossier ~/src et lui donner un nom (MonWorkspace ?)

<img src="https://raw.githubusercontent.com/epsilonrt/avrio/master/doc/images/codelite-1.png" alt="Création workspace codelite">

* Cliquer droit sur `MonWorkspace` puis `Add an Existing Project` et ajouter
le projet `monprojet.project`

<img src="https://raw.githubusercontent.com/epsilonrt/avrio/master/doc/images/codelite-2.png" alt="Ajout projet codelite">

* Le bouton de la barre d'outils ci-dessous permet d'effectuer toutes les
opérations:

<img src="https://raw.githubusercontent.com/epsilonrt/avrio/master/doc/images/codelite-3.png" alt="Actions AvrIO">

#### Utilisation sous AvrStudio 7

Comme on l'a vu précédement, un projet AvrIO créé un fichier .cproj permettant
de l'utiliser avec AvrStudio 7:

* Lancer AvrStudio 7 (et attendre ...)

La notion de Workspace existe aussi sous AvrStudio 7, cela s'appelle une
**solution**, par contre, une solution est automatiquement crée si on ouvre un
projet isolé, donc pas besoin de la créer manuellement:

* Ouvrir le projet `monprojet.cproj` à l'aide du menu `File`

<img src="https://raw.githubusercontent.com/epsilonrt/avrio/master/doc/images/as7-6.png" alt="Ouverture projet As7">

* Le bouton de la barre d'outils ci-dessous permet de compiler le projet:

<img src="https://raw.githubusercontent.com/epsilonrt/avrio/master/doc/images/as7-7.png" alt="Compilation As7">

Il n'est pas possible d'effectuer un debuggage sous AvrStudio 7 avec une carte
Arduino (sauf à écraser son firmware), nous verrons donc cette fonctionnalité
plus bas avec un projet pour une carte personnalisée.

#### Projet pour une autre carte prise en charge par AvrIO

Si votre carte n'est pas une Arduino, AvrIO connait un certain nombre de cartes
commercialisées par ATMEL, OLIMEX... Une liste des cartes supportées est
données par `avrio-prj -L`:

<pre class="fragment">
  EtudiantSN@OBIWAN MINGW64 ~/src/avrio (master)
  $ avrio-prj -L

  STK200
  USBKEY
  BUTTERFLY
  DVK90CAN1
  UNO
  ETHERNUT1
  AVRUSBSTK
  AVRUSBSTK_M32U2
  AVRCAN
  IDWARF_NODE
  IDWARF_BOX
  MAVRIX
  MAVRIX_RELOADED
  SOLARIX
  DMNET_HUB
  DMNET_STK200
  DMNET_SERVER_IHM
  TOUERIS_IHM
  GIFAM_TESTER
  GIFAM_CHIP
  USBKEY_MSL
  DVK90CAN1_XBEE
  XPLBEE
  MTO_TEMPLATE
  SOLARPI_HUM
  SOLARPI_LUM
  SOLARPI_PRESS
  SOLARPI_ETEMP
  SOLARPI_ITEMP
  SOLARPI_TX
  SOLARPI_RX
  XNET_NODE
  CHIPIO_XNET
  CHIPIO_SOLARPI
  APRS_TRACKER
  APRS_SHIELD
  P1_HUM
  P6_LUM
  P8_PRESS
  P9_TEMPE
  P10_TEMPI
</pre>

Une description des cartes disponibles est présente dans le fichier avrio/boards.txt :

<pre class="fragment">
  $ cat boards.txt
  #############################################################################
  #               List of target boards supported by AVRIO                    #
  #############################################################################
  ------------- fabriqué par ATMEL
  STK200            STK200 Starter Kit
                    http://www.atmel.com/dyn/resources/prod_documents/doc1107.pdf
  USBKEY            AT90USBKey - AT90USB Evaluation Kit
                    http://www.atmel.com/dyn/resources/prod_documents/doc7627.pdf
  BUTTERFLY         AVR Butterfly Evaluation Kit
                    http://www.atmel.com/dyn/resources/prod_documents/doc4271.pdf
  DVK90CAN1         DVK90CAN1 - AT90CAN128 Development Kit
                    http://www.atmel.com/dyn/resources/prod_documents/doc4381.pdf
  DVK90CAN1_XBEE    DVK90CAN1 - AT90CAN128 Development Kit
                    Equipé d'une carte d'extension XBee sur connecteur EXT_CON1
                    http://www.atmel.com/dyn/resources/prod_documents/doc4381.pdf
  ------------- fabriqué par ARDUINO
  UNO               Arduino UNO
                    http://arduino.cc/en/Main/arduinoBoardUno
  ------------- fabriqué par EGNITE
  ETHERNUT1         Ethernut 1.3
                    http://www.ethernut.de/pdf/enhwm13e.pdf
  ------------- fabriqué par OLIMEX
  AVRUSBSTK         AVR-USB-STK development board
                    http://www.olimex.com/dev/pdf/AVR/AVR-USB-STK.pdf
  AVRUSBSTK_M32U2   AVR-USB-STK development board modified with ATmega32U2 MCU
                    http://www.olimex.com/dev/pdf/AVR/AVR-USB-STK.pdf
  AVRCAN            AVR-CAN development board
                    http://www.olimex.com/dev/pdf/AVR/AVR-CAN.pdf
  ------------- fabriqué par chip45
  IDWARF_NODE       iDwaRF-328 V1.2 board
                    http://download.chip45.com/iDwaRF-328_V1.2_infosheet.pdf
  IDWARF_BOX        iDwaRF-BOX V1.2 board
                    http://download.chip45.com/iDwaRF-BOX_V1.2_infosheet.pdf
  ------------- fabriqué par epsilonRT
  MAVRIX            Carte mère du robot mAVRix
  MAVRIX_RELOADED   Carte mère du robot mAVRix équipée de l'extension RELOADED.
  SOLARIX           Carte mère du projet Solarix.
  DMNET_HUB         Hub du projet DomoNET
  DMNET_STK200      Emulation de DomoNET sur une carte STK200
  DMNET_SERVER_IHM  Carte IHM du serveur DomoNET
  TOUERIS_IHM       Carte IHM du projet Toueris
  GIFAM_TESTER      Carte du testeur GIFAM
  GIFAM_CHIP        Circuit contrôleur GIFAM
  APRS_TRACKER      Carte tracker APRS du projet SolarPi 2014
  APRS_SHIELD       Shield Arduino tracker APRS du projet SolarPi
  P1_HUM            Carte capteur humidité du projet SolarPi 2014
  P6_LUM            Carte capteur luminosité du projet SolarPi 2014
  P8_PRESS          Carte capteur pression du projet SolarPi 2014
  P9_TEMPE          Carte capteur température extérieure du projet SolarPi 2014
  P10_TEMPI         Carte capteur température intérieure du projet SolarPi 2014
  SOLARPI_HUM       Carte capteur humidité du projet SolarPi 2016
  SOLARPI_LUM       Carte capteur luminosité du projet SolarPi 2016
  SOLARPI_PRESS     Carte capteur pression du projet SolarPi 2016
  SOLARPI_ETEMP     Carte capteur température extérieure du projet SolarPi 2016
  SOLARPI_ITEMP     Carte capteur température intérieure du projet SolarPi 2016
  SOLARPI_TX        Carte transmission nacelle du projet SolarPi 2016
  SOLARPI_RX        Carte réception station-sol (Shield Arduino) du projet SolarPi 2016
  CHIPIO_SOLARPI    Circuit ChipIo du serveur nacelle du projet SolarPi 2016
  XNET_NODE         Carte Node du système domotique XNet
  CHIPIO_XNET       Circuit ChipIo du serveur XNet
  XPLBEE            Carte xPLBee
  MTO_TEMPLATE      Carte modèle pour station météo du projet domogik-plant 2017

  ------------- manufactured by user.
    The user can use a personal configuration using avrio-make without -B option.
    In this case, you must change the board.mk file and the module
    files used by the target in the folder avrio/projects/template/board.
</pre>

On peut modifier la carte cible en modifiant la variable `BOARD` dans le `Makefile`
avec l'une des valeurs données par

#### Projet pour une carte personnalisée

Le gros avantage d'AvrIO est de pouvoir être adapté facilement à une carte
personnalisée. Pour cette partie, nous allons considérer que nous avons une
carte personnalisée avec les spécifications suivantes:

* microcontrôleur attiny841
* fréquence d'horloge 8MHz
* une led active à l'état bas branchée sur PA3
* un bouton poussoir actif à l'état bas branchée sur PA0
* une liaison série asynchrone (UART) connectée à TXD0 (PA1) et RXD0 (PA2)

Nous utiliserons un Avr Dragon pour la programmation et le debug.

Nous décidons de baptiser notre carte MYBOARD. La première chose à faire est de créer un répertoire dans ~/src:

<pre class="fragment">
  mkdir ~/src/myboard
  cd ~/src/myboard
</pre>

Puis on crée le projet:

<pre class="fragment">
  avrio-prj -s -b MYBOARD -p attiny841 -F 8000000 myboard
</pre>

Un `ls` nous permet de voir les fichiers créés:

<pre class="fragment">
  ls
  board/  board.mk  Makefile  myboard.atsln  myboard.c  myboard.cproj  myboard.project  myboard.workspace  src/  test/
</pre>

Nous avons déjà présenté les fichiers Makefile, .c, .cproj et .project. Voici
une description succincte des nouveaux:

* `myboard.workspace` fichier workspace pour CodeLite
* `myboard.atsln` fichier solution pour AvrStudio 7
* `board.mk` fichier de description de la carte personnalisée. C'est dans ce
fichier que l'on trouve la configuration du modèle de microcontrôleur, de la
fréquence d'horloge et du programmateur utilisé.
* `board/` dossier où l'on trouve les fichiers de description des fonctions de
la carte, c'est dans ce dossier qu'on va pouvoir décrire leds, boutons poussoirs,
liaisons série, i2c, 1-wire ... Chaque fonction est associée à une fichier
`avrio-board-XXXX.h` (XXXX décriant la fonction). Ce dossier contient un exemple
de fichier pour chaque fonction prise en charge par AvrIO afin de servir de
modèle.
* `test/` dossier où l'on mettra les programmes de test unitaires de la carte, à
l'initial, il n'y a qu'un test: celui des leds.
* `src/` dossier où l'on mettra les fichiers sources spécifiques à notre carte.

La première chose à faire est de modifier les fichiers `avrio-board-XXXX.h`
associés aux fonctions prévues sur la carte: leds, boutons poussoirs et liaison
série.

Premièrement, nous allons ouvrir le fichier solution avec AvrStudio 7 afin de
pouvoir modifier les fichiers puis nous verrons comment on programme et on débogue.

Une fois AvrStudio 7 lancé et la solution ouverte, nous allons nous occuper de
la fonction led.

##### Configuration des leds

Voilà la partie intéressante de `avrio-board-led.h` qu'il va falloir modifier:

<pre class="fragment">
  /* constants ================================================================ */
  #define LED_QUANTITY  1
  #define LED_LED1 _BV(5)
  #define LED_ALL_LEDS ( LED_LED1 )
  #define LED_NO_LED (0)
  #define LED_DBG LED_LED1

  /* types ==================================================================== */
  typedef uint8_t xLedMask;

  /* inline public functions ================================================== */
  // ------------------------------------------------------------------------------
  static inline void
  vLedClear (xLedMask xMask) {

    PORTB &= ~(xMask & LED_ALL_LEDS);
  }

  // ------------------------------------------------------------------------------
  static inline void
  vLedInit (void) {

    DDRB |= LED_ALL_LEDS;
    vLedClear (LED_ALL_LEDS);
  }

  // ------------------------------------------------------------------------------
  static inline void
  vLedSet (xLedMask xMask) {

    PORTB |= (xMask & LED_ALL_LEDS);
  }

  // ------------------------------------------------------------------------------
  static inline void
  vLedToggle (xLedMask xMask) {

    PORTB ^= (xMask & LED_ALL_LEDS);
  }
</pre>

Nous avons quelques `#define` à modifier et les fonctions `inline` à modifier
pour faire en sorte de pouvoir commander la seule led que nous avons qui est
connecté sur `PA3` et qui est active à l'état bas.

Une fois adapté, le fichier est le suivant:

<pre class="fragment">
  /* constants ================================================================ */
  #define LED_QUANTITY  1
  #define LED_LED1 _BV(3)
  #define LED_ALL_LEDS ( LED_LED1 )
  #define LED_NO_LED (0)
  #define LED_DBG LED_LED1

  /* types ==================================================================== */
  typedef uint8_t xLedMask;

  /* inline public functions ================================================== */
  // ------------------------------------------------------------------------------
  static inline void
  vLedClear (xLedMask xMask) {

    PORTA |= (xMask & LED_ALL_LEDS);
  }

  // ------------------------------------------------------------------------------
  static inline void
  vLedInit (void) {

    DDRA |= LED_ALL_LEDS;
    vLedClear (LED_ALL_LEDS);
  }

  // ------------------------------------------------------------------------------
  static inline void
  vLedSet (xLedMask xMask) {

    PORTA &= ~(xMask & LED_ALL_LEDS);
  }

  // ------------------------------------------------------------------------------
  static inline void
  vLedToggle (xLedMask xMask) {

    PORTA ^= (xMask & LED_ALL_LEDS);
  }
</pre>

Comme on peut le voir, les modifications ont consisté à remplacer `_BV(5)` par `_BV(3)`,
`PORTB` par `PORTA`, `DDRB` par `DDRA` et à inverser le niveau logique associé aux
fonctions `vLedClear()` et `vLedSet()`.

Une fois la modification effectuée, nous pouvons réaliser le test unitaire led.
Il faut mettre la carte cible sous tension et y connecter un AVR Dragon sur son
connecteur ISP.

* dans le `Solution Explorer` cliquer droit sur `test_led` puis `Properties`,
dans l'onglet `Device` changer **ATmega1284P** en **ATtiny841**
* faire de même pour le projet `myboard` (**ATmega328P** en **ATtiny841**)
* dans le `Solution Explorer` cliquer droit sur `test_led` puis `Set as Startup Project`
* dans l'onglet `Tool` sélectionner votre AVR Dragon et choisir l'interface `debugWire`
* compiler le projet `test_led` puis télécharger le programme dans la cible à
l'aide de la commande `Start Without Debugging`.

Si le microcontrôleur n'est pas en mode debugWire (mais en mode ISP), AvrStudio
va vous demander une confirmation et vous demander de débrancher la liaison
ISP, de couper l'alimentation de la carte et de rebrancher la liaison.

Si tout c'est bien passé, votre led clignote !

Si l'on souhaite observer le fonctionnement du programme en mode pas à pas, il
suffit d'utiliser la commande `Start Debugging and Break` puis:

* La touche `F10` permet d'exécuter la prochaine instruction ou la prochaine
fonction,
* `F11` permet de descendre dans une fonction pour la déboguer,
* `F9` permet de poser un point d'arrêt sur la ligne du curseur
* `F5` permet de lancer le programme jusqu'au prochain point d'arrêt
* On peut stopper le mode debug en utilisant `Stop Debugging`, le programme
continuera à tourner en mode 'autonome'.

On procédera ainsi pour chaque fonction, le dossier `avrio/board/epsilonrt/mto/template`
contient les fichiers `avrio-board-button.h` et `avrio-board-tc.h` (liaison série)
correspondant à notre carte personnalisée.
