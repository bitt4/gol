#include <SDL2/SDL.h>
#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[]){

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Game of Life",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          1280,
                                          720,
                                          SDL_WINDOW_SHOWN);

    if(window == NULL){
        std::cerr << "Window creation failed: " << SDL_GetError() << "\n";
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window,
                                                -1,
                                                SDL_RENDERER_SOFTWARE);

    if(renderer == NULL){
        std::cerr << "Renderer creation failed: " << SDL_GetError() << "\n";
        return EXIT_FAILURE;
    }

    SDL_RenderPresent(renderer);

    SDL_Event e;
    bool quit = false;

    while(!quit){
        if(SDL_WaitEvent(&e)){
            switch(e.type){
            case SDL_QUIT:
                quit = true;
                break;
            default: {}
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
