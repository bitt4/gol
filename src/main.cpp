#include <SDL2/SDL.h>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <random>
#include <getopt.h>

#include "Gol.hpp"

#define MAX(a, b) ((a)>(b)?(a):(b))

#include "GameOfLifeParseFile.cpp"

void print_help();
long int parse_option_as_number(const char* arg, int max_length, bool allow_zero = false);
void load_random_grid(bool *&destination_grid, int width, int height, time_t &seed);

int main(int argc, char *argv[]){

    SDL_Init(SDL_INIT_VIDEO);

    /* Default options */
    bool return_seed = false;
    bool file_specified = false;
    std::string filename;
    time_t seed = -1;
    int width = 32;
    int height = 32;
    int cell_size = 20;
    int speed = 1;                  /* Updates per second */

    static struct option long_options[] = {
                                           {"width", required_argument, NULL, 'w'},
                                           {"height", required_argument, NULL, 'h'},
                                           {"cell-size", required_argument, NULL, 'c'},
                                           {"seed", optional_argument, NULL, 's'},
                                           {"speed", required_argument, NULL, 'v'},         /* v as in velocity */
                                           {"help", no_argument, NULL, 'H'},
                                           {NULL, 0, NULL, 0}
    };

    int c;

    while ((c = getopt_long(argc, argv, "w:h:sc:v:H", long_options, NULL)) != -1) {
        switch (c)
            {
            case 'w':
                width = static_cast<int>(parse_option_as_number(optarg, 4));
                break;
            case 'h':
                height = static_cast<int>(parse_option_as_number(optarg, 4));
                break;
            case 's':
                if(optarg){
                    seed = (unsigned)parse_option_as_number(optarg, 20, true);
                }
                else
                    return_seed = true;
                break;
            case 'c':
                cell_size = static_cast<int>(parse_option_as_number(optarg, 4));
                break;
            case 'v':
                speed = static_cast<int>(parse_option_as_number(optarg, 4, true));
                break;
            case 'H':
                print_help();
                exit(EXIT_FAILURE);
            case '?':
                fprintf(stderr,                                                    /* Yeah, upprcase 'H' is a bit uncommon */
                        "%s: Use -H or --help to display options.\n", argv[0]);    /* for the help command, but lowercase  */
                exit(EXIT_FAILURE);                                                /* 'h' is used for 'height' argument    */
            default: {}
            }
    }

    /* parse non-option arguments */
    for (int i = optind; i < argc; i++){
        file_specified = true;
        filename = argv[i];
        break;    /* ignore remaining args */
    }

    bool *initial_state;

    if(file_specified){
        load_file(filename.c_str(), initial_state, width, height);
    }
    else {
        load_random_grid(initial_state, width, height, seed);

        if(return_seed)
            fprintf(stdout, "Seed: %ld\n", seed);
    }

    GameOfLife gol(width, height, cell_size, initial_state);

    SDL_Window *window = SDL_CreateWindow("Game of Life",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          width * cell_size,
                                          height * cell_size,
                                          SDL_WINDOW_SHOWN);

    if(window == NULL){
        fprintf(stderr, "Window creation failed: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    /* Set window icon */           /* I think we don't need a function for getting full path since it's used only once */
    SDL_Surface *icon = SDL_LoadBMP((std::string(SDL_GetBasePath()) + "assets/icon.bmp").c_str());
    SDL_SetWindowIcon(window, icon);
    SDL_FreeSurface(icon);

    SDL_Renderer *renderer = SDL_CreateRenderer(window,
                                                -1,
                                                SDL_RENDERER_SOFTWARE);

    if(renderer == NULL){
        fprintf(stderr, "Renderer creation failed: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    gol.render(renderer);           /* Render initial state of game */
    SDL_RenderPresent(renderer);
    gol.update();                   /* And update it */

    int last_time = 0;
    bool paused = (speed == 0);
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
                            if(speed < 1000)         /* upper limit of 1000 updates per second */
                                speed += 1;
                            break;
                        case SDLK_KP_MINUS:
                        case SDLK_MINUS:
                            if(speed > 0)            /* lower limit of 0 updates per second, the game stops here */
                                speed -= 1;
                            break;
                        case SDLK_SPACE:
                            if(speed == 0){
                                speed = 1;
                                paused = false;
                            }
                            else
                                paused = !paused;
                            break;
                    }
            default: {}
            }
        }

        int time_now = SDL_GetTicks();              /* this value will wrap over if the game runs for more than ~49 days      */
        if(!paused && speed != 0 && time_now > last_time + 1000 / speed){
            gol.render(renderer);                   /* Use this instead of sleep(ms), because I want to detect keypresses and */
            SDL_RenderPresent(renderer);            /* events faster. If sleep() function would be used, event handlers would */
            gol.update();                           /* be delayed. This basically updates game every `1000 / speed` ms, but   */
            last_time = time_now;                   /* processes events every iteration of the main game loop                 */
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

long int parse_option_as_number(const char* arg, int max_length, bool allow_zero){
    char *endptr; /* Used by strtol */
    long int value = strtol(arg, &endptr, 10);
    /* When `allow_zero` is set to true, `value` will be 0 if parsed argument is not a valid number */

    if(allow_zero){
        if(value < 0){
            fprintf(stderr, "Invalid argument: %s is too small.\n", arg);
            exit(1);
        }
    }
    else if(value <= 0){       /* I don't think I want to allow negative values in any of the command-line arguments */
        fprintf(stderr, "Invalid argument: %s is too small.\n", arg);
        exit(1);
    }

    int length;
    if(value == 0)
        length = 1;
    else
        length = static_cast<int>(log10(value)) + 1;    /* calculate the length of a number */

    if(length > max_length){
        fprintf(stderr, "Invalid argument: %s is too big\n", arg);
        exit(1);
    }

    return value;
}

void print_help(){
    fprintf(stdout,
            "Usage: gol [OPTIONS] ... [FILE] ...\n"
            "FILE represents template for initial grid\n"
            "\n"
            "Options:\n"
            "  -w, --width=NUM       Width of grid in cells\n"
            "  -h, --height=NUM      Height of grid in cells\n"
            "  -c, --cell-size=NUM   Size of cell in pixels\n"
            "  -s, --seed=[NUM]      Set seed for random number generator if specified,\n"
            "                        otherwise display seed for generated grid\n"
            "  -v, --speed=NUM       How many times per second will the grid be updated\n"
            "  -H, --help            Display this message\n"
            "\n"
            "Example:\n"
            "  gol -w 100 -h 100 -c 5 -s -v 30      Generate a grid 100 cells wide, 100 cells\n"
            "                                       tall, cells will have 5px, print seed\n"
            "                                       from rng, and the grid will update\n"
            "                                       30 times per second\n"
            "\n"
            "Controls:\n"
            "  SPACE                 Pause/Resume the game\n"
            "  '+'/'=' key           Increase speed\n"
            "  '-' key               Decrease speed\n"
            "\n"
            "Grid FILE:\n"
            "  0 or ' ' specifies a dead cell, any other character specifies a living cell.\n"
            "  Width and height of the grid are extracted from grid file. Length of the\n"
            "  longest line will become width of the grid. Height of the grid is specified\n"
            "  by the number of lines in the file. If there's a trailing newline at the end\n"
            "  of the file, it won't be counted.\n"
            "  Width and height specified by command-line arguments are overriden by the width\n"
            "  and height of the FILE.\n"
            "Example:\n\n"
            "    000000\n"
            "    000100\n"
            "    010100\n"
            "    001100\n"
            "    000000\n"
            "    000000\n");
}

void load_random_grid(bool *&destination_grid, int width, int height, time_t &seed){
    int grid_size = width * height;
    destination_grid = new bool[grid_size]();

    if(seed == -1)    /* If the seed isn't specified in command-line arguments */
        seed = time(NULL);    /* set seed for rng */

    std::mt19937 generator(seed);

    for(int i = 0; i < grid_size; i++)
        destination_grid[i] = generator()%2;
}
