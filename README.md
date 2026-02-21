<p align="center">
  <img src="https://github.com/LolBaum/BlockGame/blob/2d32680f58500a500f32f70ae24b23bb65a4ab83/GitHub/BlockGame_Logo.png"/>
</p>

# BlockGame
Prototype of a Voxelgame with C++ and OpenGL.

Table of Contents
-----------------

1. [Controls](#controls) <!--- 2. [Paths](#paths) -->
3. [Configuration File](#config-file)
4. [Building](#building)
5. [Screenshots](#screenshots)

Controls
--------
### Gameplay
| Button             | Action          |
|--------------------|-----------------|
| Move mouse         | Look around     |
| W, A, S, D         | Move            |
| Space              | Jump            |
| Left mouse button  | Remove block    |
| Right mouse button | Place block     |
| 0-9                | Select item     |
    |

### Utilities & Debugging

| Button | Action                  |
|--------|-------------------------| 
| F1     | Take Screenshot         |
| Z      | Render Wireframe        |
| ESC    | Quit                    |
| Y      | Toggle UI text          |
| G      | Toggle visual debugging |





<!---
Paths
-----
-->

Config File
-----------
- location: `build/config.json`
- This file is created by closing BlockGame for the first time.


Building
--------

## building with Cmake
BlockGame can be build using the `CMakeLists.txt` file (tested in CLion).

Make sure the executable will be placed into `build` directory. 
Alternatively the working directory can be specified to the `build` directory.

This build process might take longer than the alternative approaches.

## Alternative Linux
following dependencies need to be installed: 
```
sudo apt-get install libglew-dev libglm-dev libsdl2-dev
``` 

### Compiling
To build the Project (with g++), simply execute the `buildLinux` bash script.<br>
If the compilation was sucessfull the executable game will be at `build/mainLinux`.<br>

## Alternative Windows
The necessary dependencies `glew, glm, SDL2` are already included in this repository (`libs` directory).


### buildWindows.sh
The `buildWindows.sh` script can be used to build the Game for Windows. All Dependecies must be located in the `libs\` directory.
It's important that all folders have the same names as in the script.
<br>

### build_with_python.py
The `python build_with_python.py` can be used to speed up the compilation if the Project is compiled multiple times.
All compiled sources are cached in `.python_build/filehash.txt` and will only be recompiled if necessary.

## MacOS 
_not tested_

Screenshots
-----------

<p align="center">
  <img src="https://github.com/LolBaum/BlockGame/blob/759c4edf9a6b1e978c0427af788dfb3654848490/GitHub/coolScreenshots/screenshot_178.png"/>
</p>

<p align="center">
  <img src="https://github.com/LolBaum/BlockGame/blob/759c4edf9a6b1e978c0427af788dfb3654848490/GitHub/coolScreenshots/screenshot_199_test.png"/>
</p>

___

<p align="center">
    <i>
    The development of this Project has started around 20.09.2021.
    </i>
</p>

<p align="center">
  <img src="https://github.com/LolBaum/BlockGame/blob/bdb9729edd7e09680489464cc1e8383afcf3309e/GitHub/gitBlockGameSocial.png" style="width:200px;"/>
</p>
