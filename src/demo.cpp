#include "Jugador.h"
#include "Enemigo.h"
#include "Bala.h"


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

enum EstadoJuego
{
    MENU,
    SELECCION,
    MAPA1,
    MAPA2,
    MAPA3,
    VICTORIA
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

    // CAMBIO 2: carga de textura mapa2
    SDL_Texture* mapa2 =
        cargarTextura(
            "gallery/mapa-2.jpeg",
            renderer
        );

    SDL_Texture* mapa3 =
        cargarTextura(
            "gallery/mapa-3.jpeg",
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

    float enemigo1X = 900;
    float enemigo1Y = 500;
    bool enemigo1Vivo = true;

    float enemigo2X = 1100;
    float enemigo2Y = 500;
    bool enemigo2Vivo = true;

    float enemigo3X = 1300;
    float enemigo3Y = 500;
    bool enemigo3Vivo = true;

    int enemigosEliminados = 0;

    int vidaJefe = 10;

    float jefeX = 950;
    float jefeY = 350;

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
                    (estado == MAPA1 || estado == MAPA2 || estado == MAPA3) &&
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
                else if (estado == MAPA2)
                {
                    if (evento.key.keysym.sym == SDLK_RETURN)
                    {
                        estado = MAPA3;
                    }
                }
                else if (estado == SELECCION)                {
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

            if (enemigo1Vivo && balaActiva)
            {
                if (
                    balaX > enemigo1X &&
                    balaX < enemigo1X + 120 &&
                    balaY > enemigo1Y &&
                    balaY < enemigo1Y + 120
                )
                {
                    enemigo1Vivo = false;
                    balaActiva = false;
                    enemigosEliminados++;
                }
            }

            if (enemigo2Vivo && balaActiva)
            {
                if (
                    balaX > enemigo2X &&
                    balaX < enemigo2X + 120 &&
                    balaY > enemigo2Y &&
                    balaY < enemigo2Y + 120
                )
                {
                    enemigo2Vivo = false;
                    balaActiva = false;
                    enemigosEliminados++;
                }
            }

            if (enemigo3Vivo && balaActiva)
            {
                if (
                    balaX > enemigo3X &&
                    balaX < enemigo3X + 120 &&
                    balaY > enemigo3Y &&
                    balaY < enemigo3Y + 120
                )
                {
                    enemigo3Vivo = false;
                    balaActiva = false;
                    enemigosEliminados++;
                }
            }

            // CAMBIO 3: transicion a MAPA2 al eliminar los 3 enemigos
            if (enemigosEliminados >= 3)
            {
                estado = MAPA2;
            }
        }

        // Lógica de MAPA2: disparo y transición a MAPA3
        if (estado == MAPA2)
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

            if (balaActiva)
            {
                if (mirandoDerecha)
                    balaX += 12;
                else
                    balaX -= 12;

                if (balaX > 1400 || balaX < -50)
                {
                    balaActiva = false;
                }
            }
        }

        // Lógica de MAPA3: jefe
        if (estado == MAPA3)
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

                if (balaX > 1400 || balaX < -50)
                {
                    balaActiva = false;
                }
            }

            // Cambio 6: colisión con el jefe
            if (balaActiva)
            {
                if (
                    balaX > jefeX &&
                    balaX < jefeX + 250 &&
                    balaY > jefeY &&
                    balaY < jefeY + 250
                )
                {
                    vidaJefe--;
                    balaActiva = false;
                }
            }

            // Cambio 7: victoria
            if (vidaJefe <= 0)
            {
                estado = VICTORIA;
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

            if (enemigo1Vivo)
            {
                SDL_Rect enemigo =
                {
                    (int)enemigo1X,
                    (int)enemigo1Y,
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

            if (enemigo2Vivo)
            {
                SDL_Rect enemigo =
                {
                    (int)enemigo2X,
                    (int)enemigo2Y,
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

            if (enemigo3Vivo)
            {
                SDL_Rect enemigo =
                {
                    (int)enemigo3X,
                    (int)enemigo3Y,
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
        }

        // CAMBIO 4: dibujar mapa2
        if (estado == MAPA2)
        {
            SDL_RenderCopy(
                renderer,
                mapa2,
                nullptr,
                nullptr
            );
        }

        // Cambio 5: dibujar mapa3 con jefe
        if (estado == MAPA3)
        {
            SDL_RenderCopy(
                renderer,
                mapa3,
                nullptr,
                nullptr
            );

            SDL_Rect jefe =
            {
                (int)jefeX,
                (int)jefeY,
                250,
                250
            };

            SDL_SetRenderDrawColor(
                renderer,
                0,
                0,
                255,
                255
            );

            SDL_RenderFillRect(
                renderer,
                &jefe
            );
        }

        // Cambio 8: pantalla de victoria
        if (estado == VICTORIA)
        {
            SDL_SetRenderDrawColor(
                renderer,
                0,
                180,
                0,
                255
            );

            SDL_RenderClear(renderer);
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyTexture(menu);
    SDL_DestroyTexture(seleccion);
    SDL_DestroyTexture(mapa1);
    SDL_DestroyTexture(mapa2);       // CAMBIO 5: liberar mapa2
    SDL_DestroyTexture(mapa3);

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