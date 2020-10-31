#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <random>
#include <getopt.h>

#include "Gol.hpp"

long int parse_option_as_number(const char* arg, int max_length, bool allow_zero = false);

void load_file(const char* filename, bool *&destination_grid);

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
    int SPEED = 1;                  /* Updates per second */

    static struct option long_options[] = {
                                           {"random", no_argument, NULL, 'r'},
                                           {"width", required_argument, NULL, 'w'},
                                           {"height", required_argument, NULL, 'h'},
                                           {"seed", optional_argument, NULL, 's'},
                                           {"cell-size", required_argument, NULL, 'c'},
                                           {"speed", required_argument, NULL, 'v'},         /* v as in velocity */
                                           {"help", no_argument, NULL, '?'},
                                           {NULL, 0, NULL, 0}
    };

    int c;

    /* Parse command-line arguments, NOT YET COMPLETE */
    while ((c = getopt_long(argc, argv, "rw:h:sc:v:?", long_options, NULL)) != -1) {
        switch (c)
            {
            case 'r':
                randomize_grid = true;
                break;
            case 'w':
                width = parse_option_as_number(optarg, 4);        /* implicit cast from long to int */
                break;
            case 'h':
                height = parse_option_as_number(optarg, 4);       /* implicit cast from long to int */
                break;
            case 's':
                if(optarg){
                    seed = (unsigned)parse_option_as_number(optarg, 20);
                    randomize_grid = true;                        /* randomize the grid, since the seed for prng was provided */
                }
                else
                    return_seed = true;
                break;
            case 'c':
                cell_size = parse_option_as_number(optarg, 4);    /* implicit cast from long to int */
                break;
            case 'v':
                SPEED = parse_option_as_number(optarg, 4, true);  /* implicit cast from long to int */
                break;
            case '?':
                /* display help, not fully implemented yet */
                fprintf(stderr,
                        "%s: Use -h or --help to display options.\n", argv[0]);
                exit(EXIT_FAILURE);
            default: {}
            }
    }

    int grid_size = width * height;
    bool *initial_state = (bool*)calloc(grid_size, sizeof(bool));

    if(randomize_grid){
        if(seed == 0)
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

    GameOfLife gol(width, height, cell_size, initial_state);

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

    /* Set window icon */           /* I think we don't need a function for getting full path since it's used only once */
    SDL_Surface *icon = SDL_LoadBMP((std::string(SDL_GetBasePath()) + "assets/icon.bmp").c_str());
    SDL_SetWindowIcon(window, icon);
    SDL_FreeSurface(icon);

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

    free(initial_state);

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
    else if(value <= 0){       /* I don't think I want to allow negative values in any of the command-line arguments           */
        fprintf(stderr, "Invalid argument: %s is too small.\n", arg);
        exit(1);
    }

    int length;
    if(value == 0)
        length = 1;
    else
        length = (int)log10(value) + 1;    /* calculate the length of a number */

    if(length > max_length){
        fprintf(stderr, "Invalid argument: %s is too big\n", arg);
        exit(1);
    }

    return value;
}

void load_file(const char* filename, bool *&destination_grid){
    /* For now handles simple files */
    std::ifstream grid_file(filename);

    int grid_width, grid_height = 1;

    if(grid_file.is_open()){
        /* Check if the file is empty, http://www.cplusplus.com/reference/ios/ios/eof/ */
        if(grid_file.peek() == std::ifstream::traits_type::eof()){
            fprintf(stderr, "File '%s' is empty.\n", filename);
            exit(1);
        }
        else {
            int max_line_width = 0;
            int current_line_length = 0
            
            char c;
            while(grid_file.get(c)){
                if(c == '\n' && grid_file.peek() != std::ifstream::traits_type::eof()){
                    grid_height++;
                    max_line_width = MAX(max_line_width, current_line_length);
                    current_line_length = 0;
                }
                else {
                    current_line_length++;
                }
            }

            grid_width = max_line_width;

            /* allocate memory for grid */
            int grid_size = grid_width * grid_height;
            destination_grid = (bool*)calloc(grid_size, sizeof(bool));

            /* fill the grid */
            grid_file.clear();    /* Read file again from the beginning */
            grid_file.seekg(0, grid_file.beg);

            int grid_y = 0;
            current_line_length = 0;

            while(grid_file.get(c)){
                bool cell;
                if(c == '\n' && grid_file.peek() != std::ifstream::traits_type::eof()){
                    grid_y++;
                    current_line_length = 0;
                }
                else {
                    if(c == '0' || c == ' '){
                        cell = false;
                    }
                    else {
                        cell = true;
                    }
                    destination_grid[grid_y * grid_width + current_line_length] = cell;
                    current_line_length++;
                }
            }
        }
    }
    else {
        fprintf(stderr, "Can't open file '%s'.\n", filename);
        exit(1);
    }
}