#include "Gol.hpp"

Gol::Gol(int width, int height, int cell_width, bool* initial_state)
    :width(width),
     height(height),
     cell_width(cell_width)
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

void Gol::update(){

}

void Gol::render(){

}
