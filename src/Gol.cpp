#include "Gol.hpp"

Gol::Gol(int width, int height, int cell_width, bool* initial_state)
    :width(width),
     height(height),
     cell_width(cell_width),
     cell_color({.r = 255, .g = 0, .b = 0, .a = 255}),
     background_color({.r = 0, .g = 0, .b = 0, .a = 255})
{
    int size_of_grid = width * height;

    this->first_grid = (bool*)calloc(size_of_grid, sizeof(bool));
    this->second_grid = (bool*)calloc(size_of_grid, sizeof(bool));

    memcpy(first_grid, initial_state, size_of_grid);
}

Gol::~Gol(){
    free(this->first_grid);
    free(this->second_grid);
}

inline int mod(int a, int b){
    int res = a % b;
    return res >= 0 ? res : res + b;
}

int Gol::get_nearby_cells(int x, int y){
    int nearby_cells = 0;

    for(int ry = -1; ry < 2; ry++){
        for(int rx = -1; rx < 2; rx++){
            if(!(rx == 0 && ry == 0)){
                if(this->second_grid[ mod(y + ry, this->height) * this->height + mod(x + rx, this->width) ])
                    nearby_cells++;
            }
        }
    }

    return nearby_cells;
}

void Gol::draw_cell(SDL_Renderer* renderer, int x, int y, SDL_Color color){
    SDL_Rect cell_rectangle;
    cell_rectangle.x = x * this->cell_width;
    cell_rectangle.y = y * this->cell_width;
    cell_rectangle.w = this->cell_width;
    cell_rectangle.h = this->cell_width;

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &cell_rectangle);
}

void Gol::update(){
    int size_of_grid = this->width * this->height;
    memcpy(second_grid, first_grid, size_of_grid);

    for(int y = 0; y < this->height; y++){
        for(int x = 0; x < this->width; x++){
            int nearby_cells = get_nearby_cells(x, y);
            int current_cell_coords = y * this->height + x;

            if(this->second_grid[current_cell_coords]            /* If the current cell is alive */
               && nearby_cells >= 2                              /* and if it has 2 or 3 living neighbour cells */
               && nearby_cells <= 3)
            {
                this->first_grid[current_cell_coords] = true;    /* it will live on to next generation */
            }

            else if(!this->second_grid[current_cell_coords]           /* If the current cell is dead */
               && nearby_cells == 3)                             /* and it has exactly 3 living neighbour cells */
            {
                this->first_grid[current_cell_coords] = true;    /* it will become living cell */
            }

            else {
                this->first_grid[current_cell_coords] = false;   /* Any other cell dies */
            }
        }
    }
}

void Gol::render(SDL_Renderer* renderer){
    for(int y = 0; y < this->height; y++){
        for(int x = 0; x < this->width; x++){
            if(this->first_grid[y * this->height + x]){
                draw_cell(renderer, x, y, this->cell_color);
            }
            else {
                draw_cell(renderer, x, y, this->background_color);
            }
        }
    }
}
