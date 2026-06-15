#include <SDL2/SDL.h>
#include <iostream>

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Error al iniciar SDL\n";
        return 1;
    }

    SDL_Window* ventana = SDL_CreateWindow(
        "MANDATO FINAL",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1280,
        720,
        SDL_WINDOW_SHOWN
    );

    if (!ventana)
    {
        std::cout << "Error al crear ventana\n";
        return 1;
    }

    SDL_Renderer* renderer =
        SDL_CreateRenderer(
            ventana,
            -1,
            SDL_RENDERER_ACCELERATED
        );

    bool ejecutando = true;
    SDL_Event evento;

    float jugadorX = 100;
    float jugadorY = 500;

    float velocidadX = 0;
    float velocidadY = 0;

    bool enSuelo = true;

    while (ejecutando)
    {
        while (SDL_PollEvent(&evento))
        {
            if (evento.type == SDL_QUIT)
                ejecutando = false;
        }

        const Uint8* teclado =
            SDL_GetKeyboardState(NULL);

        velocidadX = 0;

        if (teclado[SDL_SCANCODE_A])
            velocidadX = -5;

        if (teclado[SDL_SCANCODE_D])
            velocidadX = 5;

        if (teclado[SDL_SCANCODE_W] && enSuelo)
        {
            velocidadY = -12;
            enSuelo = false;
        }

        velocidadY += 0.5f;

        jugadorX += velocidadX;
        jugadorY += velocidadY;

        if (jugadorY >= 500)
        {
            jugadorY = 500;
            velocidadY = 0;
            enSuelo = true;
        }

        SDL_SetRenderDrawColor(
            renderer,
            20,
            20,
            30,
            255
        );

        SDL_RenderClear(renderer);

        SDL_Rect suelo =
        {
            0,
            600,
            1280,
            120
        };

        SDL_SetRenderDrawColor(
            renderer,
            0,
            180,
            0,
            255
        );

        SDL_RenderFillRect(
            renderer,
            &suelo
        );

        SDL_Rect jugador =
        {
            (int)jugadorX,
            (int)jugadorY,
            50,
            80
        };

        SDL_SetRenderDrawColor(
            renderer,
            0,
            180,
            255,
            255
        );

        SDL_RenderFillRect(
            renderer,
            &jugador
        );

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(ventana);
    SDL_Quit();

    return 0;
}