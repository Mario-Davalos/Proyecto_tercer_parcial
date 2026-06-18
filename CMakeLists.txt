cmake_minimum_required(VERSION 3.16)
project(MandatoFinal)
set(CMAKE_CXX_STANDARD 17)
include_directories(
    C:/vcpkg/installed/x64-mingw-dynamic/include
)
link_directories(
    C:/vcpkg/installed/x64-mingw-dynamic/lib
)
add_executable(MandatoFinal
    src/demo.cpp
    src/Jugador.cpp
    src/Enemigo.cpp
    src/Bala.cpp
)
target_link_libraries(MandatoFinal
    SDL2main
    SDL2
    SDL2_image
)
