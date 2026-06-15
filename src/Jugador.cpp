#include "Jugador.h"

Jugador::Jugador()
{
    x = 200;
    y = 500;

    velocidadY = 0;

    enSuelo = true;
}

void Jugador::moverIzquierda()
{
    x -= 6;
}

void Jugador::moverDerecha()
{
    x += 6;
}

void Jugador::saltar()
{
    if (enSuelo)
    {
        velocidadY = -15;
        enSuelo = false;
    }
}

void Jugador::actualizar()
{
    velocidadY += 0.6f;

    y += velocidadY;

    if (y >= 500)
    {
        y = 500;
        velocidadY = 0;
        enSuelo = true;
    }
}

SDL_Rect Jugador::obtenerRect()
{
    SDL_Rect rect;

    rect.x = (int)x;
    rect.y = (int)y;
    rect.w = 120;
    rect.h = 120;

    return rect;
}