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

int Gol::get_nearby_cells(int x, int y){
    /* Not implemented so for now */ return 0;
}

void Gol::draw_cell(SDL_Renderer* renderer, int x, int y, SDL_Color color){
    SDL_Rect cell_rectangle;
    cell_rectangle.x = x;
    cell_rectangle.y = y;
    cell_rectangle.w = this->cell_width;
    cell_rectangle.h = this->cell_width;

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, cell_rectangle);
}

void Gol::update(){
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
