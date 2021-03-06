#define MAX(a, b) (a) > (b) ? (a) : (b)

void get_grid_dimensions_from_file_stream(std::ifstream &file_stream, int &grid_width_dest, int &grid_height_dest){
    int max_line_width = 0;
    int current_line_length = 0;
    grid_height_dest = 1;

    char c;
    while(file_stream.get(c)){
        if(c == '\n' && file_stream.peek() != std::ifstream::traits_type::eof()){
            grid_height_dest++;
            max_line_width = MAX(max_line_width, current_line_length);
            current_line_length = 0;
        }
        else {
            current_line_length++;
        }
    }

    grid_width_dest = max_line_width;
}

void fill_grid(std::ifstream &file_stream, std::vector<bool> &destination_grid, const int grid_width){
    int grid_y = 0;
    int current_line_length = 0;

    char c;
    while(file_stream.get(c)){
        bool is_living_cell;
        if(c == '\n'){
            grid_y++;
            current_line_length = 0;
        }
        else {
            if(c == '0' || c == ' '){
                is_living_cell = false;
            }
            else {
                is_living_cell = true;
            }
            destination_grid[grid_y * grid_width + current_line_length] = is_living_cell;
            current_line_length++;
        }
    }
}

void load_file(const char* filename, std::vector<bool> &destination_grid, int &grid_width_dest, int &grid_height_dest){
    /* For now handles simple files */
    std::ifstream grid_file(filename);

    int grid_width, grid_height;

    if(grid_file.is_open()){
        /* Check if the file is empty, http://www.cplusplus.com/reference/ios/ios/eof/ */
        if(grid_file.peek() == std::ifstream::traits_type::eof()){
            fprintf(stderr, "File '%s' is empty.\n", filename);
            exit(1);
        }
        else {
            get_grid_dimensions_from_file_stream(grid_file, grid_width, grid_height);

            /* allocate memory for grid */
            int grid_size = grid_width * grid_height;
            destination_grid.resize(grid_size);

            grid_file.clear();    /* Make file stream readable again from the beginning */
            grid_file.seekg(0, grid_file.beg);

            /* fill the grid */
            fill_grid(grid_file, destination_grid, grid_width);
        }
    }
    else {
        fprintf(stderr, "Can't open file '%s'.\n", filename);
        exit(1);
    }

    grid_width_dest = grid_width;
    grid_height_dest = grid_height;
    grid_file.close();
}
