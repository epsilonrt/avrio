   Copyright © 2009-2011 epsilonRT. All rights reserved.
   $Id$
================================================================================

Pour compiler certains projets, des sources supplémentaires sont nécessaires.
Pour de multiples raisons, les codes sources de ces projets ne sont pas inclus
dans la hiérarchie de avrio.

1- LUFA
-------
LUFA est un projet qui permet de gérer USB avec les microcontroleurs AVR
équipés de ce type de liaison (ATMEGA16U4 ...)
Pour compiler les projets utilisant LUFA, la dernière version de LUFA doit être
téléchargée et décompressée dans ce répertoire (avrio/src).
Le répertoire de décompression doit être renommé en avrio/src/lufa.

Version actuellement utilisée: LUFA101122

Lien vers le site et pour le téléchargement de LUFA:

                  http://www.fourwalledcubicle.com

2-FREEMODBUS
------------
FreeModBus est un projet qui permet de gérer ModBus avec les microcontroleurs
AVR équipés de liaison série (UART ou USART). Lien vers le site:
                  http://freemodbus.berlios.de

Pour compiler les projets utilisant FreeModBus, la dernière version de
FreeModBus doit être téléchargée et décompressée dans ce répertoire (avrio/src).
Le répertoire de décompression doit être renommé en avrio/src/freemodbus.

Version actuellement utilisée: freemodbus-v1.5.0

Lien pour le téléchargement de FreeModBus:
        https://developer.berlios.de/project/showfiles.php?group_id=6120

2-MODBUS MASTER
---------------
MBMaster est un projet qui permet d'implémenter un maître ModBus avec les 
microcontroleurs AVR équipés de liaison série (UART ou USART). 
Lien vers le site:
                  http://www.embedded-solutions.at/

Pour compiler les projets utilisant MBMaster, la dernière version de
MBMaster doit être achatée et décompressée dans ce répertoire (avrio/src).
Le répertoire de décompression doit être renommé en avrio/src/mbmaster.

Version actuellement prise en charge: mbmaster-v2.7.0

4-FAFS
------
FatFS est un projet permettant de gérer des partitions FAT12/16/32 sur des
microcontrôleurs "légers". Sur AVR, il intègre les couches bas-niveau 
permettant de gérer des partions sur:
- Cartes MMC/SD sur liaison SPI (Module natif ou BitBang)
- Cartes CFC (Compact Flash)
- Disques ATA (IDE)

Une fois téléchargé et dézippé :
- renommer le dossier src de FatFs en : fatfs 
- recopier le dossier fatfs dans le dossier src de AvrIO
- recopier ffconf.h de src/fatfs vers le dossier de votre application (demo/fatfs ?)
  et modifier la configuration comme bon vous semble (dans votre dossier !)
- renommer en src/fatfs/ffconf.h en ffconf_template.h (afin de pouvoir récupérer
  une copie "propre" de ce fichier et d'éviter de le modifier).

Version actuellement utilisée: FatFs R0.09

Lien pour le téléchargement de FatFs:
        http://elm-chan.org/fsw/ff/00index_e.html

================================================================================
