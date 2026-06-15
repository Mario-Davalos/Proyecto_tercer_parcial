#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

enum EstadoJuego
{
    MENU,
    SELECCION,
    MAPA1
};

SDL_Texture* cargarTextura(
    const char* ruta,
    SDL_Renderer* renderer)
{
    SDL_Surface* surface = IMG_Load(ruta);

    if (!surface)
    {
        std::cout << "Error cargando: "
                  << ruta << std::endl;

        std::cout << IMG_GetError()
                  << std::endl;

        return nullptr;
    }

    SDL_Texture* textura =
        SDL_CreateTextureFromSurface(
            renderer,
            surface
        );

    SDL_FreeSurface(surface);

    return textura;
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    IMG_Init(
        IMG_INIT_PNG |
        IMG_INIT_JPG
    );

    SDL_Window* ventana =
        SDL_CreateWindow(
            "MANDATO FINAL",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            1376,
            768,
            SDL_WINDOW_SHOWN
        );

    SDL_Renderer* renderer =
        SDL_CreateRenderer(
            ventana,
            -1,
            SDL_RENDERER_ACCELERATED
        );

    SDL_Texture* menu =
        cargarTextura(
            "gallery/Menu.png",
            renderer
        );

    SDL_Texture* seleccion =
        cargarTextura(
            "gallery/Seleccion de personajes.png",
            renderer
        );

    SDL_Texture* mapa1 =
        cargarTextura(
            "gallery/mapa-1.jpeg",
            renderer
        );

    SDL_Texture* millonario =
        cargarTextura(
            "gallery/Millonario.png",
            renderer
        );

    SDL_Texture* buchona =
        cargarTextura(
            "gallery/Buchona.png",
            renderer
        );

    SDL_Texture* alucin =
        cargarTextura(
            "gallery/Alucin.png",
            renderer
        );

    SDL_Texture* peluchon =
        cargarTextura(
            "gallery/Peluchon.png",
            renderer
        );

    EstadoJuego estado = MENU;

    int personajeSeleccionado = 0;

    float jugadorX = 200;
    float jugadorY = 500;

    float velocidadY = 0;

    bool enSuelo = true;

    bool mirandoDerecha = true;

    float balaX = -100;
    float balaY = -100;

    bool balaActiva = false;

    float enemigoX = 1000;
    float enemigoY = 500;

    bool enemigoVivo = true;

    bool ejecutando = true;

    SDL_Event evento;

    while (ejecutando)
    {
        while (SDL_PollEvent(&evento))
        {
            if (evento.type == SDL_QUIT)
            {
                ejecutando = false;
            }

            if (evento.type == SDL_KEYDOWN)
            {
                if (
                    estado == MAPA1 &&
                    evento.key.keysym.sym == SDLK_SPACE &&
                    !balaActiva
                )
                {
                    balaActiva = true;
                    balaX = jugadorX + 80;
                    balaY = jugadorY + 50;
                }

                if (estado == MENU)
                {
                    if (evento.key.keysym.sym == SDLK_RETURN)
                    {
                        estado = SELECCION;
                    }
                }
                else if (estado == SELECCION)
                {
                    if (evento.key.keysym.sym == SDLK_RIGHT)
                    {
                        personajeSeleccionado++;

                        if (personajeSeleccionado > 3)
                            personajeSeleccionado = 0;
                    }

                    if (evento.key.keysym.sym == SDLK_LEFT)
                    {
                        personajeSeleccionado--;

                        if (personajeSeleccionado < 0)
                            personajeSeleccionado = 3;
                    }

                    if (evento.key.keysym.sym == SDLK_RETURN)
                    {
                        estado = MAPA1;
                    }
                }
            }
        }

        const Uint8* teclado =
            SDL_GetKeyboardState(NULL);

        if (estado == MAPA1)
        {
            if (teclado[SDL_SCANCODE_A])
            {
                jugadorX -= 6;
                mirandoDerecha = false;
            }

            if (teclado[SDL_SCANCODE_D])
            {
                jugadorX += 6;
                mirandoDerecha = true;
            }

            if (
                teclado[SDL_SCANCODE_W] &&
                enSuelo
            )
            {
                velocidadY = -15;
                enSuelo = false;
            }

            velocidadY += 0.6f;

            jugadorY += velocidadY;

            if (jugadorY >= 500)
            {
                jugadorY = 500;
                velocidadY = 0;
                enSuelo = true;
            }

            if (balaActiva)
            {
                if (mirandoDerecha)
                    balaX += 12;
                else
                    balaX -= 12;

                if (
                    balaX > 1400 ||
                    balaX < -50
                )
                {
                    balaActiva = false;
                }
            }

            if (
                enemigoVivo &&
                balaActiva
            )
            {
                if (
                    balaX > enemigoX &&
                    balaX < enemigoX + 120 &&
                    balaY > enemigoY &&
                    balaY < enemigoY + 120
                )
                {
                    enemigoVivo = false;
                    balaActiva = false;
                }
            }
        }

        SDL_RenderClear(renderer);

        if (estado == MENU)
        {
            SDL_RenderCopy(
                renderer,
                menu,
                nullptr,
                nullptr
            );
        }

        if (estado == SELECCION)
        {
            SDL_RenderCopy(
                renderer,
                seleccion,
                nullptr,
                nullptr
            );

            SDL_Rect selector =
            {
                190 + personajeSeleccionado * 300,
                550,
                120,
                120
            };

            SDL_SetRenderDrawColor(
                renderer,
                255,
                0,
                0,
                255
            );

            SDL_RenderDrawRect(
                renderer,
                &selector
            );
        }

        if (estado == MAPA1)
        {
            SDL_RenderCopy(
                renderer,
                mapa1,
                nullptr,
                nullptr
            );

            SDL_Texture* personajeActual =
                millonario;

            if (personajeSeleccionado == 1)
                personajeActual = buchona;

            if (personajeSeleccionado == 2)
                personajeActual = alucin;

            if (personajeSeleccionado == 3)
                personajeActual = peluchon;

            SDL_Rect jugador =
            {
                (int)jugadorX,
                (int)jugadorY,
                120,
                120
            };

            SDL_RenderCopy(
                renderer,
                personajeActual,
                nullptr,
                &jugador
            );

            if (enemigoVivo)
            {
                SDL_Rect enemigo =
                {
                    (int)enemigoX,
                    (int)enemigoY,
                    120,
                    120
                };

                SDL_SetRenderDrawColor(
                    renderer,
                    255,
                    0,
                    0,
                    255
                );

                SDL_RenderFillRect(
                    renderer,
                    &enemigo
                );
            }

            if (balaActiva)
            {
                SDL_Rect bala =
                {
                    (int)balaX,
                    (int)balaY,
                    20,
                    10
                };

                SDL_SetRenderDrawColor(
                    renderer,
                    255,
                    255,
                    0,
                    255
                );

                SDL_RenderFillRect(
                    renderer,
                    &bala
                );
            }
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyTexture(menu);
    SDL_DestroyTexture(seleccion);
    SDL_DestroyTexture(mapa1);

    SDL_DestroyTexture(millonario);
    SDL_DestroyTexture(buchona);
    SDL_DestroyTexture(alucin);
    SDL_DestroyTexture(peluchon);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(ventana);

    IMG_Quit();
    SDL_Quit();

    return 0;
}