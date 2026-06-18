#pragma once

#include <SDL2/SDL.h>

class Enemigo
{
public:
    float x;
    float y;

    bool vivo;

    Enemigo(
        float posX,
        float posY
    );

    SDL_Rect obtenerRect();
};