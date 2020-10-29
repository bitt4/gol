#include <SDL2/SDL.h>
#include <cstdlib>
#include <cstring>

class GameOfLife {
private:
    int width, height;
    int cell_width;
    bool* rendered_grid;
    bool* comparison_grid;

    SDL_Color cell_color, background_color;

    int get_nearby_cells(int x, int y);
    void draw_cell(SDL_Renderer* renderer, int x, int y, SDL_Color color);
public:
    GameOfLife(int width, int height, int cell_width, bool* initial_state);
    ~GameOfLife();
    void update();
    void render(SDL_Renderer* renderer);
};
