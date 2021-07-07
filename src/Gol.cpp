#include "../include/Gol.hpp"

GameOfLife::GameOfLife(const int width, const int height, const int cell_width, const std::vector<bool>& initial_state)
    :m_width { width },
     m_height { height },
     m_cell_width { cell_width }
{
    m_cell_color.r = 255;
    m_cell_color.g = 0;
    m_cell_color.b = 0;
    m_cell_color.a = 255;

    m_background_color.r = 0;
    m_background_color.g = 0;
    m_background_color.b = 0;
    m_background_color.a = 255;

    int size_of_grid = width * height;

    m_rendered_grid = initial_state;
    m_comparison_grid.resize(size_of_grid);
}

GameOfLife::~GameOfLife()
{
}

inline int mod(const int a, const int b){
    int res = a % b;
    return res >= 0 ? res : res + b;
}

int GameOfLife::get_nearby_cells(const int x, const int y) const {
    int nearby_cells = 0;

    for(int ry = -1; ry < 2; ry++){
        for(int rx = -1; rx < 2; rx++){        /* Use modulo to ensure that negative coordinates will warp over, e.g. y coord */
            if(!(rx == 0 && ry == 0)){         /* with value -2 will wrap over to -2 + height of grid (e.g. 4 for height = 6) */
                bool cell = m_comparison_grid[ mod(y + ry, m_height) * m_width + mod(x + rx, m_width) ];
                if(cell /* is alive */)
                    nearby_cells++;
            }
        }
    }

    return nearby_cells;
}

void GameOfLife::draw_cell(SDL_Renderer* renderer, const int x, const int y, const SDL_Color &color) const {
    SDL_Rect cell_rectangle;
    cell_rectangle.x = x * m_cell_width;
    cell_rectangle.y = y * m_cell_width;
    cell_rectangle.w = m_cell_width;
    cell_rectangle.h = m_cell_width;

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &cell_rectangle);
}

void GameOfLife::update(){
    m_comparison_grid = m_rendered_grid;

    for(int y = 0; y < m_height; y++){
        for(int x = 0; x < m_width; x++){
            int nearby_cells = get_nearby_cells(x, y);
            int current_cell_coords = y * m_width + x;
            bool previous_cell = m_comparison_grid[current_cell_coords];
            auto&& current_cell = m_rendered_grid[current_cell_coords];

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

void GameOfLife::render(SDL_Renderer* renderer) const {
    for(int y = 0; y < m_height; y++){
        for(int x = 0; x < m_width; x++){
            int current_position = y * m_width + x;
            bool cell_before = m_comparison_grid[current_position];
            bool cell_now = m_rendered_grid[current_position];

            /* Check if the cell changed its state*/
            if(cell_before != cell_now){
                SDL_Color color = m_background_color;
                if(cell_now /* is alive */){
                    color = m_cell_color;
                }
                draw_cell(renderer, x, y, color);
            }
        }
    }
}
