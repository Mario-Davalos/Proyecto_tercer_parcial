cmake_minimum_required(VERSION 3.16)

project(MandatoFinal)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)

pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)

add_executable(MandatoFinal
    src/demo.cpp
    src/Jugador.cpp
    src/Enemigo.cpp
)

target_include_directories(MandatoFinal PRIVATE
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
)

target_link_libraries(MandatoFinal
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
)