#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

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

    SDL_Surface* fondoSurface =
        IMG_Load("gallery/Menu.png");

    if (!fondoSurface)
    {
        std::cout << IMG_GetError() << std::endl;
        return 1;
    }

    SDL_Texture* fondo =
        SDL_CreateTextureFromSurface(
            renderer,
            fondoSurface
        );

    SDL_FreeSurface(fondoSurface);

    bool ejecutando = true;

    SDL_Event evento;

    while (ejecutando)
    {
        while (SDL_PollEvent(&evento))
        {
            if (evento.type == SDL_QUIT)
                ejecutando = false;

            if (evento.type == SDL_KEYDOWN)
            {
                if (evento.key.keysym.sym == SDLK_RETURN)
                {
                    std::cout
                        << "MENU -> SELECCION\n";
                }
            }
        }

        SDL_RenderClear(renderer);

        SDL_RenderCopy(
            renderer,
            fondo,
            nullptr,
            nullptr
        );

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(fondo);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(ventana);

    IMG_Quit();
    SDL_Quit();

    return 0;
}