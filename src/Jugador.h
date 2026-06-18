#pragma once

#include <SDL2/SDL.h>

class Jugador
{
public:
    float x;
    float y;

    float velocidadY;

    bool enSuelo;

    Jugador();

    void moverIzquierda();
    void moverDerecha();
    void saltar();
    void actualizar();

    SDL_Rect obtenerRect();
};