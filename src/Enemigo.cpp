#include "Enemigo.h"

Enemigo::Enemigo(
    float posX,
    float posY
)
{
    x = posX;
    y = posY;

    vivo = true;
}

SDL_Rect Enemigo::obtenerRect()
{
    SDL_Rect rect;

    rect.x = (int)x;
    rect.y = (int)y;
    rect.w = 120;
    rect.h = 120;

    return rect;
}