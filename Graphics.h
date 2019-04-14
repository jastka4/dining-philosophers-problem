#pragma once

class Graphics {
    int xMax = 0;
    int yMax = 0;

    void draw_table();
    void draw_philosophers();
    void draw_forks();

public:
    void draw_stage();
    void redraw_fork(int color, int x, int y);
};
