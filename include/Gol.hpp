#include <SDL2/SDL.h>
#include <cstdlib>
#include <cstring>
#include <vector>

class GameOfLife {
private:
    int m_width, m_height;
    int m_cell_width;
    std::vector<bool> m_rendered_grid;
    std::vector<bool> m_comparison_grid;

    SDL_Color m_cell_color, m_background_color;

    int get_nearby_cells(const int x, const int y) const;
    void draw_cell(SDL_Renderer* renderer, const int x, const int y, const SDL_Color &color) const;
public:
    GameOfLife(const int width, const int height, const int cell_width, const std::vector<bool>& initial_state);
    ~GameOfLife();
    void update();
    void render(SDL_Renderer* renderer) const;
};
