#include <SDL2/SDL.h>
#include <iostream>
#include <cstdlib>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

void sleep(int miliseconds){
#ifdef _WIN32
    Sleep(miliseconds);
#else
    usleep(miliseconds * 1000);
#endif
}

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
    const int SPEED = 60;    /* Updates per second */

    while(!quit){
        if(SDL_WaitEvent(&e)){
            switch(e.type){
            case SDL_QUIT:
                quit = true;
                break;
            default: {}
            }
        }
        // gol.update();    // Not implemented
        // gol.render();    // Not implemented
        SDL_RenderPresent(renderer);
        sleep(1000 / SPEED);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
