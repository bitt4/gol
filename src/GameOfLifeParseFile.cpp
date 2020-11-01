void load_file(const char* filename, bool *&destination_grid){
    /* For now handles simple files */
    std::ifstream grid_file(filename);

    int grid_width, grid_height = 1;

    if(grid_file.is_open()){
        /* Check if the file is empty, http://www.cplusplus.com/reference/ios/ios/eof/ */
        if(grid_file.peek() == std::ifstream::traits_type::eof()){
            fprintf(stderr, "File '%s' is empty.\n", filename);
            exit(1);
        }
        else {
            int max_line_width = 0;
            int current_line_length = 0;

            char c;
            while(grid_file.get(c)){
                if(c == '\n' && grid_file.peek() != std::ifstream::traits_type::eof()){
                    grid_height++;
                    max_line_width = MAX(max_line_width, current_line_length);
                    current_line_length = 0;
                }
                else {
                    current_line_length++;
                }
            }

            grid_width = max_line_width;

            /* allocate memory for grid */
            int grid_size = grid_width * grid_height;
            destination_grid = (bool*)calloc(grid_size, sizeof(bool));

            /* fill the grid */
            grid_file.clear();    /* Read file again from the beginning */
            grid_file.seekg(0, grid_file.beg);

            int grid_y = 0;
            current_line_length = 0;

            while(grid_file.get(c)){
                bool cell;
                if(c == '\n'){
                    grid_y++;
                    current_line_length = 0;
                }
                else {
                    if(c == '0' || c == ' '){
                        cell = false;
                    }
                    else {
                        cell = true;
                    }
                    destination_grid[grid_y * grid_width + current_line_length] = cell;
                    current_line_length++;
                }
            }
        }
    }
    else {
        fprintf(stderr, "Can't open file '%s'.\n", filename);
        exit(1);
    }
}
