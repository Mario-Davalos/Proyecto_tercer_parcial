#include "Bala.h"

Bala::Bala()
{
    x = -100;
    y = -100;

    activa = false;

    direccion = 1;
}

void Bala::disparar(
    float inicioX,
    float inicioY,
    int dir
)
{
    if (!activa)
    {
        activa = true;

        x = inicioX;
        y = inicioY;

        direccion = dir;
    }
}

void Bala::actualizar()
{
    if (!activa)
        return;

    x += 12 * direccion;

    if (x > 1500 || x < -100)
    {
        activa = false;
    }
}

SDL_Rect Bala::obtenerRect()
{
    SDL_Rect rect;

    rect.x = (int)x;
    rect.y = (int)y;
    rect.w = 20;
    rect.h = 10;

    return rect;
}