#include <SDL2/SDL.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <random>
#include <getopt.h>

#include "Gol.hpp"

int parse_option(const char* arg, int max_length);

int main(int argc, char *argv[]){

    SDL_Init(SDL_INIT_VIDEO);

    /*
    bool initial_state[400] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    pulsar
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
    */

    /* Default options */
    bool randomize_grid = false;
    bool return_seed = false;
    time_t seed = 0;
    int width = 17;
    int height = 17;
    int cell_size = 50;

    static struct option long_options[] = {
                                           {"random", no_argument, NULL, 'r'},
                                           {"width", required_argument, NULL, 'w'},
                                           {"height", required_argument, NULL, 'h'},
                                           {"seed", no_argument, NULL, 's'},
                                           {"cs", required_argument, NULL, 'c'},
                                           {"help", no_argument, NULL, '?'},
                                           {NULL, 0, NULL, 0}
    };

    int c;

    /* Parse command-line arguments, NOT YET COMPLETE */
    while ((c = getopt_long(argc, argv, "rw:h:sc:?", long_options, NULL)) != -1) {
        switch (c)
            {
            case 'r':
                randomize_grid = true;
                break;
            case 'w':
                width = parse_option(optarg, 4);
                break;
            case 'h':
                height = parse_option(optarg, 4);
                break;
            case 's':
                return_seed = true;
                break;
            case 'c':
                cell_size = parse_option(optarg, 3);
                break;
            case '?':
                /* display help, not fully implemented yet */
                fprintf(stderr,
                        "%s: Use -h or --help to display options.\n", argv[0]);
                exit(EXIT_FAILURE);
            default: {}
            }
    }

    /* TODO: Check for minimal width and height of the window
     */

    /* TODO: Allocate memory for `initial_state` after getting width and height from command-line args
     */

    int grid_size = width * height;
    bool *initial_state = (bool*)calloc(grid_size, sizeof(bool));

    if(randomize_grid){
        seed = time(NULL);
        std::mt19937 generator(seed);

        for(int i = 0; i < grid_size; i++){
            initial_state[i] = generator()%2;
        }
    }
    else {    /* For now make every other cell alive */
        for(int i = 0; i < grid_size; i++){
            initial_state[i] = i%2;
        }
    }

    if(return_seed)
        fprintf(stdout, "Seed: %ld\n", seed);

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
                            if(SPEED > 0)            /* lower limit of 0 updates per second, the game stops here */
                                SPEED -= 1;
                            break;
                        case SDLK_SPACE:             /* If the SPEED was not equal to 0, set it to 0 */
                            SPEED = !SPEED;          /* If the SPEED was equal to 0, set it to 1     */
                            break;
                    }
            default: {}
            }
        }

        int time_now = SDL_GetTicks();              /* this value will wrap over if the game runs for more than ~49 days      */
        if(SPEED != 0 && time_now > last_time + 1000 / SPEED){
            gol.render(renderer);                   /* Use this instead of sleep(ms), because I want to detect keypresses and */
            SDL_RenderPresent(renderer);            /* events faster. If sleep() function would be used, event handlers would */
            gol.update();                           /* be delayed. This basically updates game every `1000 / SPEED` ms, but   */
            last_time = time_now;                   /* processes events every iteration of the main game loop                 */
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

int parse_option(const char* arg, int max_length){
    int value = atoi(arg);    /* I used atoi, because it returns 0 when the text is not a number, and I can easily check that */

    if(value <= 0){           /* I don't think I want to allow negative values in any of the command-line arguments           */
        fprintf(stderr, "Invalid argument: %s is either not a number or is too small\n", arg);
        exit(1);
    }

    int length = (int)log10(value) + 1;    /* calculate the length of a number */

    if(length > max_length){
        fprintf(stderr, "Invalid argument: %s is too big\n", arg);
        exit(1);
    }

    return value;
}
