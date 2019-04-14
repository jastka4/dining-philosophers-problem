#include "Graphics.h"
#include <ncurses.h>

void Graphics::draw_table() {
    mvprintw(10, 22, ".....");
    mvprintw(11, 18, "_d^^^^^^^^^b_");
    mvprintw(12, 15, ".d''           ``b.");
    mvprintw(13, 13, ".p'                 `q.");
    mvprintw(14, 12, ".d'                   `b.");
    mvprintw(15, 11, ".d'                     `b.");
    mvprintw(16, 11, "::                       ::");
    mvprintw(17, 11, "::                       ::");
    mvprintw(18, 11, "::                       ::");
    mvprintw(19, 11, "`p.                     .q'");
    mvprintw(20, 12, "`p.                   .q'");
    mvprintw(21, 13, "`b.                 .d'");
    mvprintw(22, 15, "`q..          ..p'");
    mvprintw(23, 18, "^q........p^");
    mvprintw(24, 22, "''''");
}

void Graphics::draw_philosophers() {
    attron(COLOR_PAIR(1));
    mvprintw(5, 17, "      ,.,");
    mvprintw(6, 17, "     ((())");
    mvprintw(7, 17, "    (|*_*|)");
    mvprintw(8, 17, "     c\\=/c");
    attroff(COLOR_PAIR(1));

    attron(COLOR_PAIR(2));
    mvprintw(13, 2, "  .-.");
    mvprintw(14, 2, " (~ ~)");
    mvprintw(15, 2, " :o o:");
    mvprintw(16, 2, "(((_)))");
    mvprintw(17, 2, "  '-'");
    attroff(COLOR_PAIR(2));

    attron(COLOR_PAIR(3));
    mvprintw(23, 4, "      ,.,");
    mvprintw(24, 4, "    ((~\"~))");
    mvprintw(25, 4, "   '(|o_o|)'");
    mvprintw(26, 4, "   ,..\\=/..,");
    attroff(COLOR_PAIR(3));

    attron(COLOR_PAIR(4));
    mvprintw(23, 35, " ('¯`)");
    mvprintw(24, 35, " )X X(");
    mvprintw(25, 35, "( (_) )");
    mvprintw(26, 35, " `'-'`");
    attroff(COLOR_PAIR(4));

    attron(COLOR_PAIR(5));
    mvprintw(13, 40, "  ,.,");
    mvprintw(14, 40, " (~ ~)");
    mvprintw(15, 40, "q:0 0:p");
    mvprintw(16, 40, " ((_))");
    mvprintw(17, 40, "  'u'");
    attroff(COLOR_PAIR(5));
}

void Graphics::draw_forks() {
    mvprintw(13, 17, "--<=");
    mvprintw(18, 14, "--<=");
    mvprintw(22, 23, "--<=");
    mvprintw(13, 28, "--<=");
    mvprintw(18, 31, "--<=");
}

void Graphics::redraw_fork(const int color, const int x, const int y) {
    attron(COLOR_PAIR(color));
    mvprintw(x, y, "--<=");
    attroff(COLOR_PAIR(color));
}

void Graphics::draw_stage() {
    draw_table();
    draw_philosophers();
    draw_forks();
}
