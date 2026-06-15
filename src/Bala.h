#ifndef BALA_H
#define BALA_H

#include <SDL2/SDL.h>

class Bala
{
public:

    float x;
    float y;

    bool activa;

    int direccion;

    Bala();

    void disparar(
        float inicioX,
        float inicioY,
        int dir
    );

    void actualizar();

    SDL_Rect obtenerRect();
};

#endif