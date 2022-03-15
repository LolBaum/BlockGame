$BASE_DIR=Get-Location  #"E:\Programmieren\_GameDev\Ourcraft"
$IMGUI_DIR="E:\Programmieren\_GameDev\Ourcraft\libs\imgui"

$LIB_DIR="E:\Programmieren\_GameDev\Ourcraft\libs"

$CXXFLAGS="-std=c++11", "-Wall", "-I$IMGUI_DIR", "-I$IMGUI_DIR\backends"

$SOURCES = "$BASE_DIR\main.cpp", "shader.cpp",`
"$IMGUI_DIR\imgui.cpp", "$IMGUI_DIR\imgui_demo.cpp", "$IMGUI_DIR\imgui_draw.cpp", "$IMGUI_DIR\imgui_tables.cpp", "$IMGUI_DIR\imgui_widgets.cpp", `
"$IMGUI_DIR\backends\imgui_impl_sdl.cpp", "$IMGUI_DIR\backends\imgui_impl_opengl3.cpp"

$INCLUDES = "-I$LIB_DIR\SDL2\x86_64-w64-mingw32\include\SDL2", "-I$LIB_DIR\glew-2.1.0\include\GL", `
"-I$LIB_DIR\glm", "-I$LIB_DIR\freetype-win\include"

$LINKER = "-L$LIB_DIR\SDL2\x86_64-w64-mingw32\lib", "-L$LIB_DIR\glew-2.1.0\lib\Release\x64", "-L$LIB_DIR\freetype-win\release_dll\win64"

$LIBS = "-lgdi32", "-limm32", "-lglew32", "-lmingw32", "-lSDL2main", "-lSDL2", "-lglew32", "-lopengl32", "-lWs2_32", `
"-lole32", "-lcomctl32", "-lgdi32", "-lcomdlg32", "-luuid", "-lfreetype"

# REMEMBER: NO Whithespaces inside the ""


$BUILD_DIR = "$BASE_DIR\build"
$EXE = "$BUILD_DIR\build_win_sh.exe"


g++ $SOURCES $CXXFLAGS $INCLUDES $LINKER $LIBS -o $EXE