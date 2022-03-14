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

**Building**
To build the Project (with g++), simply execute the `buildLinux` bash script.
If the compilation was sucessfull the executable game will be at `build/mainLinux`.
To run the game, the corresponding dynamic librarys of the dependencies are necessaray (the commands above should install these correctly)

## Windows (Still Work in Progress)
following dependencies need to be installed: 
| dependency <br>(development Version) | download link |
| ----------- | ----------- |
| GLEW: | `http://glew.sourceforge.net/` |
| glm: | `https://glm.g-truc.net/0.9.9/` |
| SDL2: | `https://libsdl.org/download-2.0.php` |
| freetype: | `https://freetype.org/download.html` |

**Building**

*Further Information will be added soon.*
The file `.vscode/tasks.json` can be used in VSCode for the compilation.
To run the game, the DLLs of the dependencies are necessaray (`SDL2.dll`, `glew32.dll`, `freetype.dll`). For windows they are aleady locted in the build directory.


___
The development of this Project has started around 20.09.2021.