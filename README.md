<p align="center">
  <img src="https://github.com/LolBaum/BlockGame/blob/master/BlockGame_Logo.png"/>
</p>

# BlockGame
Prototype of a Voxelgame with C++ and OpenGL.
## Disclaimer
*This Project is still under construction and might not work stable.
The Licence (and Licences of used Dependecies) will be added soon*
___
### Dependencies
glew, glm, SDL2, freetype
___
# Instaltion 
## Linux
### Ubuntu
following dependencies need to be installed: 
| dependency | installation (apt-get) |
| ----------- | ----------- |
| GLEW: | `sudo apt-get install libglew-dev` |
| glm: | `sudo apt-get install libglm-dev` |
| SDL2: | `sudo apt-get install libsdl2-dev` |
| freetype: | `sudo apt-get install libfreetype6-dev` |

**Building**<br>
To build the Project (with g++), simply execute the `buildLinux` bash script.<br>
If the compilation was sucessfull the executable game will be at `build/mainLinux`.<br>
To run the game, the corresponding dynamic librarys of the dependencies are necessaray (the commands above should install these correctly)

## Windows *(Still Work in Progress)*
following dependencies need to be installed: 
| dependency <br>(development Version) | download link |
| ----------- | ----------- |
| GLEW: | `http://glew.sourceforge.net/` |
| glm: | `https://glm.g-truc.net/0.9.9/` |
| SDL2: | `https://libsdl.org/download-2.0.php` |
| freetype: | `https://freetype.org/download.html` |

**Building**<br>
The `buildWindows.sh` script can be used to build the Game for Windows. All Dependecies must be located in the `libs\` directory.
It's important that all folders have the same names as in the script.
<br>
The file `.vscode\tasks.json` can be used in VSCode for the compilation. <br>
To run the game, the DLLs of the dependencies are necessaray (`SDL2.dll`, `glew32.dll`, `freetype.dll`). For windows they are aleady locted in the build directory.

## MacOS *(Work in Progress)*
_coming soon_

**Building**<br>
_coming soon_



___
The development of this Project has started around 20.09.2021.