#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Error SDL\n";
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG)))
    {
        std::cout << "Error SDL_image\n";
        return 1;
    }

    SDL_Window* ventana = SDL_CreateWindow(
        "MANDATO FINAL",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1376,
        768,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(
        ventana,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    SDL_Surface* surface = IMG_Load("gallery/Menu.png");

    if (!surface)
    {
        std::cout << "No se pudo cargar Menu.png\n";
        std::cout << IMG_GetError() << std::endl;
        return 1;
    }

    SDL_Texture* menuTexture =
        SDL_CreateTextureFromSurface(
            renderer,
            surface
        );

    SDL_FreeSurface(surface);

    bool ejecutando = true;
    SDL_Event evento;

    while (ejecutando)
    {
        while (SDL_PollEvent(&evento))
        {
            if (evento.type == SDL_QUIT)
                ejecutando = false;
        }

        SDL_RenderClear(renderer);

        SDL_RenderCopy(
            renderer,
            menuTexture,
            nullptr,
            nullptr
        );

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(menuTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(ventana);

    IMG_Quit();
    SDL_Quit();

    return 0;
}