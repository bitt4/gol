#include <SDL2/SDL.h>
#include <iostream>
#include <cstdlib>

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
    double SPEED = 1;    /* Updates per second */
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
                            if(SPEED < 996)
                                SPEED += 5;
                            SDL_Log("%lf", SPEED);
                            break;
                        case SDLK_KP_MINUS:
                        case SDLK_MINUS:
                            if(SPEED > 5)
                                SPEED -= 5;
                            SDL_Log("%lf", SPEED);
                            break;
                        case SDLK_ASTERISK:              /* Again, no physical key :(   */
                        case SDLK_8:                     /* So I'll use the `8` key     */
                        case SDLK_KP_MULTIPLY:
                            if(SPEED < 500)
                                SPEED *= 2;
                            SDL_Log("%lf", SPEED);
                            break;
                        case SDLK_SLASH:
                        case SDLK_KP_DIVIDE:
                            if(SPEED > 0.05)
                                SPEED /= 2;
                            SDL_Log("%lf", SPEED);
                            break;
                    }
            default: {}
            }
        }

        int time_now = SDL_GetTicks();              /* this value will wrap over if the game runs more than ~49 days        */
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
