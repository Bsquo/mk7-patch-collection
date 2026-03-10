# Mario Kart 7 - Patch collection

This repository contains various examples patches and mods for **Mario Kart 7**.

## List of patches
* **change_chara_wifi** (WIP): Adds a "Character" button in the online VR screen that allows you to change the character in between races.

* **show_timer_in_gp**: Makes the timer be visible during Grand Prix

## Building

Install the 3DS packages from [devkitPro](https://devkitpro.org/wiki/Getting_Started), as well as Python and make, if not already installed.

Use ```make``` in the repository's root to build ```code.ips``` and ```exheader.bin```. For cleaning, use ```make clean```.

You can specify which game version to build the patches for by typing ```make VERSION=XXXX```, where XXXX is one of the following. The title ID is shown between parenthesis:

* ```usa_rev1```: Americas (v1.1, prepatched) (`0004000000030800`)
* ```eur_rev0```: Europe (v1.0) (`0004000000030700`) (WIP)
* ```eur_rev2```: Europe (v1.2) (`0004000000030700`) (WIP)

## Installation
### On 3DS Console with Luma3DS
Open the 3DS's SD card in your computer. Then, navigate to ```\luma\titles\title ID```, and copy the files ```code.ips```, ```exheader.bin``` generated in this repository's root into that folder. 

Power on your 3DS while holding the SELECT key to launch the Luma3DS menu. Make sure that ```Enable game patching``` is turned on, then press START to save and launch the home menu. You only have to do this once.

### On Citra
In the Citra window, right-click on the installed copy of Mario Kart 7, and choose ```Open Mods Location``` (alternatively you can go to ```File->Open Citra Folder``` and then open up ```\load\mods\title ID```.) Copy the files ```code.ips```, ```exheader.bin``` generated in this repository's root into that folder.

## Credits
This project is based on bits and pieces of other 3DS modding and reverse engineering projects:
* [oot3d_practice_menu](https://github.com/HylianFreddy/oot3d_practice_menu) ([gamestebled](https://github.com/gamestabled), [lioncash](https://github.com/lioncash), [HylianFreddy](https://github.com/HylianFreddy) and [gymnast86](https://github.com/carlwernicke86)): This project was used as the primary base for this repository - more specifically, **HylianFreddy's fork**, as it includes additional code inside `patch.py` to automatically generate certain parts of the final `exheader.bin`. [MM3DR](https://github.com/PhlexPlexico/mm3dr) uses a similar build system, which in turn is based on [MM3D Restoration Project](https://github.com/leoetlino/project-restoration) (which uses the [Magikoopa](https://github.com/RicBent/Magikoopa) software).

* [Mario Kart 7 Reverse Engineering Team](https://github.com/mk7re): For their repositories, used here as a base for libraries. Including (but not limited to):
    - [OpenEAD](https://github.com/open-ead): [sead](https://github.com/open-ead/sead), [nnheaders](https://github.com/open-ead/nnheaders)
    - [3DS Decompilation](https://github.com/3dsdecomp): [LibMessageStudio](https://github.com/3dsdecomp/LibMessageStudio)
    - [Anto726](https://github.com/Anto726): game research
    - [PabloMK7](https://github.com/PabloMK7): game research
    - [Marioiscool246](https://github.com/Marioiscool246): `Kart::NetData` research
    - _tZ: `System::RootSystem`, its nested classes, and much more

* [devkitPro](https://github.com/devkitpro): For the `devkitARM` toolchain.

* [Magikoopa-NSMB2-Examples](https://github.com/RicBent/Magikoopa-NSMB2-Examples) ([RicBent](https://github.com/RicBent/Magikoopa-NSMB2-Examples)) and [nsmb2-patch-collection](https://github.com/Gimzie/nsmb2-patch-collection) ([Gimzie](https://github.com/Gimzie)): Being the idea inspiration for this repository.
