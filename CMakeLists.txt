# CMakeLists.txt loosely based on https://github.com/nmwsharp/polyscope/blob/v1.3.0/examples/demo-app/CMakeLists.txt

cmake_minimum_required(VERSION 3.14)

project(BlockGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

# GLEW
FetchContent_Declare(
        glew
        GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
        GIT_TAG        origin/master
)
FetchContent_GetProperties(glew)
if(NOT glew_POPULATED)
    FetchContent_Populate(glew)
    add_subdirectory(${glew_SOURCE_DIR} ${glew_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# GLM
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm
        GIT_TAG        origin/master
)

# SDL2
FetchContent_Declare(
        SDL2
        GIT_REPOSITORY  https://github.com/libsdl-org/SDL
        GIT_TAG         release-2.0.20
        GIT_PROGRESS    TRUE
)

FetchContent_MakeAvailable(glew glm SDL2)

list(POP_BACK CMAKE_MESSAGE_INDENT)


# Add project sources dependencies
include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
        ${glew_SOURCE_DIR}/include/GL
        "${PROJECT_SOURCE_DIR}/libs/include"
)

file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/include/*.h"
        "${PROJECT_SOURCE_DIR}/include/*.hpp"
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/*.c"
        )

IF (WIN32)
    set(SDL_LIBRARIES ${SDL_LIBRARIES} SDL2-static)
ELSE()
    set(SDL_LIBRARIES ${SDL_LIBRARIES} SDL2)
ENDIF()

# Create an executable
add_executable(BlockGame ${all_SRCS})

get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()

# Link to dependencies
target_link_libraries(BlockGame libglew_static glm::glm ${SDL_LIBRARIES})
