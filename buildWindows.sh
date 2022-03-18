# This Build Script is Work in progress
# It includes files (ImGui) that are currently not used in this Project
# The Compiler will raise multible warnings while compliling. Don't worry they are only important for debug purposes.

# setting the Directories
$BASE_DIR=Get-Location 
$BUILD_DIR = "$BASE_DIR\build"
$IMGUI_DIR="$BASE_DIR\libs\imgui"
$LIB_DIR="$BASE_DIR\libs"
$EXE = "$BUILD_DIR\mainWin.exe"

echo "Starting Compilation of $EXE"

# Compiler Flags
$CXXFLAGS="-std=c++11", "-Wall"

# Adding all sources
$SOURCES = "$BASE_DIR\main.cpp", "shader.cpp" #,`
#"$IMGUI_DIR\imgui.cpp", "$IMGUI_DIR\imgui_demo.cpp", "$IMGUI_DIR\imgui_draw.cpp", "$IMGUI_DIR\imgui_tables.cpp", "$IMGUI_DIR\imgui_widgets.cpp", `
#"$IMGUI_DIR\backends\imgui_impl_sdl.cpp", "$IMGUI_DIR\backends\imgui_impl_opengl3.cpp"

# Including all directories which are needed
$INCLUDES = "-I$LIB_DIR\SDL2\x86_64-w64-mingw32\include\SDL2", "-I$LIB_DIR\glew-2.1.0\include\GL", `
"-I$LIB_DIR\glm", "-I$LIB_DIR\freetype-win\include" #, "-I$IMGUI_DIR", "-I$IMGUI_DIR\backends"

# linking the libraries
$LINKER = "-L$LIB_DIR\SDL2\x86_64-w64-mingw32\lib", "-L$LIB_DIR\glew-2.1.0\lib\Release\x64", "-L$LIB_DIR\freetype-win\release_dll\win64"
$LIBS = "-lgdi32", "-limm32", "-lglew32", "-lmingw32", "-lSDL2main", "-lSDL2", "-lglew32", "-lopengl32", "-lWs2_32", `
"-lole32", "-lcomctl32", "-lgdi32", "-lcomdlg32", "-luuid", "-lfreetype"
# REMEMBER: NO Whithespaces inside the ""

# Compilation
g++ $SOURCES $CXXFLAGS $INCLUDES $LINKER $LIBS -o $EXE