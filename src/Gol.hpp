#include <SDL2/SDL.h>
#include <cstdlib>
#include <cstring>

class Gol {
private:
    int width, height;
    int cell_width;
    bool* first_grid;
    bool* second_grid;

    int get_nearby_cells(int x, int y);
public:
    Gol(int width, int height, int cell_width, bool* initial_state);
    ~Gol();
    void update();
    void render();
};
