#include <SDL2/SDL.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>

#include "Gol.hpp"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

int main(int argc, char *argv[]){

    SDL_Init(SDL_INIT_VIDEO);

    bool initial_state[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0,
                            0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0,
                            0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0,
                            0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0,
                            0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0,
                            0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0,
                            0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    int width = 17;
    int height = 17;
    int cell_size = 50;

    bool randomize_grid = true;    /* This flag will be enabled by command-line arguments */

    /* TODO: return seed of pseudo-rng if certain flag is set */

    if(randomize_grid){
        srand(time(NULL));
        for(int y = 0; y < height; y++){
            for(int x = 0; x < width; x++){
                initial_state[ y * height + x ] = (rand()%2 == 0);
            }
        }
    }

    Gol gol(width, height, cell_size, initial_state);

    SDL_Window *window = SDL_CreateWindow("Game of Life",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          width * cell_size,
                                          height * cell_size,
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

    gol.render(renderer);           /* Render initial state of game */
    SDL_RenderPresent(renderer);
    gol.update();                   /* And update it */

    int last_time = 0;
    int SPEED = 1;                  /* Updates per second */
    bool quit = false;
    SDL_Event e;

    while(!quit){
        while(SDL_PollEvent(&e)){
            switch(e.type){
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym){
                        case SDLK_PLUS:              /* This virtual key does not have corresponding physical key, :(   */
                        case SDLK_EQUALS:            /* When I press `Shift` and `=` keys, it writes `+`                */
                        case SDLK_KP_PLUS:
                            if(SPEED < 1000)         /* upper limit of 1000 updates per second */
                                SPEED += 1;
                            break;
                        case SDLK_KP_MINUS:
                        case SDLK_MINUS:
                            if(SPEED > 1)            /* lower limit of 1 update per second */
                                SPEED -= 1;
                            break;
                    }
            default: {}
            }
        }

        int time_now = SDL_GetTicks();              /* this value will wrap over if the game runs for more than ~49 days    */
        if(time_now > last_time + 1000 / SPEED){
            gol.render(renderer);                   /* Use this instead of sleep(ms), because I want to detect keypresses,  */
            SDL_RenderPresent(renderer);            /* etc. faster. If sleep() function would be used, event handlers would */
            gol.update();                           /* be delayed. This basically updates game every `1000 / SPEED` ms,     */
            last_time = time_now;                   /* but processes events every iteration of the main game loop           */
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
