#include <SDL2/SDL.h>

class Gol {
private:
    int width, height;
    bool* first_grid, second_grid;
public:
    Gol(int width, int height, bool* initial_state);
    ~Gol();
    void update();
    void render();
};
