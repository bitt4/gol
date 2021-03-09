#include "Gol.hpp"

GameOfLife::GameOfLife(int width, int height, int cell_width, bool*& initial_state)
    :width(width),
     height(height),
     cell_width(cell_width)
{
    this->cell_color.r = 255;
    this->cell_color.g = 0;
    this->cell_color.b = 0;
    this->cell_color.a = 255;

    this->background_color.r = 0;
    this->background_color.g = 0;
    this->background_color.b = 0;
    this->background_color.a = 255;

    int size_of_grid = width * height;

    this->rendered_grid = initial_state;
    this->comparison_grid = new bool[size_of_grid]();
}

GameOfLife::~GameOfLife(){
    delete[] this->comparison_grid;
}

inline int mod(int a, int b){
    int res = a % b;
    return res >= 0 ? res : res + b;
}

int GameOfLife::get_nearby_cells(int x, int y){
    int nearby_cells = 0;

    for(int ry = -1; ry < 2; ry++){
        for(int rx = -1; rx < 2; rx++){        /* Use modulo to ensure that negative coordinates will warp over, e.g. y coord */
            if(!(rx == 0 && ry == 0)){         /* with value -2 will wrap over to -2 + height of grid (e.g. 4 for height = 6) */
                bool cell = this->comparison_grid[ mod(y + ry, this->height) * this->width + mod(x + rx, this->width) ];
                if(cell /* is alive */)
                    nearby_cells++;
            }
        }
    }

    return nearby_cells;
}

void GameOfLife::draw_cell(SDL_Renderer* renderer, int x, int y, SDL_Color color){
    SDL_Rect cell_rectangle;
    cell_rectangle.x = x * this->cell_width;
    cell_rectangle.y = y * this->cell_width;
    cell_rectangle.w = this->cell_width;
    cell_rectangle.h = this->cell_width;

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &cell_rectangle);
}

void GameOfLife::update(){
    int size_of_grid = this->width * this->height;
    memcpy(comparison_grid, rendered_grid, size_of_grid);

    for(int y = 0; y < this->height; y++){
        for(int x = 0; x < this->width; x++){
            int nearby_cells = get_nearby_cells(x, y);
            int current_cell_coords = y * this->width + x;
            bool previous_cell = this->comparison_grid[current_cell_coords];
            bool &current_cell = this->rendered_grid[current_cell_coords];

            if(previous_cell && nearby_cells >= 2 && nearby_cells <= 3){
                current_cell = true;
            } else if(!previous_cell && nearby_cells == 3){
                current_cell = true;
            } else {
                current_cell = false;
            }
        }
    }
}

void GameOfLife::render(SDL_Renderer* renderer){
    for(int y = 0; y < this->height; y++){
        for(int x = 0; x < this->width; x++){
            bool cell_before = this->comparison_grid[y * this->width + x];
            bool cell_now = this->rendered_grid[y * this->width + x];

            /* Check if the cell changed its state*/
            if(cell_before != cell_now){
                SDL_Color color = this->background_color;
                if(cell_now /* is alive */){
                    color = this->cell_color;
                }
                draw_cell(renderer, x, y, color);
            }
        }
    }
}
