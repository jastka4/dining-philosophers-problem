#include <ncurses.h>

int xMax = 0;
int yMax = 0;

void draw_stage() {
    attron(COLOR_PAIR(1));
    mvprintw(5, 17, "      ,.,");
    mvprintw(6, 17,"     ((())");
    mvprintw(7, 17,"    (|*_*|)");
    mvprintw(8, 17,"     c\\=/c");
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

    mvprintw(10, 22,".....");
    mvprintw(11, 18, "_d^^^^^^^^^b_");
    mvprintw(12, 15, ".d''           ``b.");
    mvprintw(13, 13, ".p'                 `q.");
    mvprintw(14, 12,".d'                   `b.");
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

    attron(COLOR_PAIR(4));
    mvprintw(13, 40, "  ,.,");
    mvprintw(14, 40, " (~ ~)");
    mvprintw(15, 40, "q:0 0:p");
    mvprintw(16, 40, " ((_))");
    mvprintw(17, 40, "  'u'");
    attroff(COLOR_PAIR(4));

    attron(COLOR_PAIR(5));
    mvprintw(23, 35, " ('¯`)");
    mvprintw(24, 35, " )X X(");
    mvprintw(25, 35, "( (_) )");
    mvprintw(26, 35, " `'-'`");
    attroff(COLOR_PAIR(5));

    // draw forks
    attron(COLOR_PAIR(1));
    mvprintw(13, 17, "--<=");
    attroff(COLOR_PAIR(1));

    attron(COLOR_PAIR(2));
    mvprintw(18, 14, "--<=");
    attroff(COLOR_PAIR(2));

    attron(COLOR_PAIR(3));
    mvprintw(22, 23, "--<=");
    attroff(COLOR_PAIR(3));

    attron(COLOR_PAIR(4));
    mvprintw(13, 28, "--<=");
    attroff(COLOR_PAIR(4));

    attron(COLOR_PAIR(5));
    mvprintw(18, 31, "--<=");
    attroff(COLOR_PAIR(5));
}

int main () {
    initscr();		//ncurses start
    keypad(stdscr, TRUE);
    noecho();		//pressed characters do not print on the screen
    cbreak();		//pressed keys are immediately returned by getch() instead of waiting until new line is read
    //get screen size
    getmaxyx(stdscr, yMax, xMax);

    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(4, COLOR_RED, COLOR_BLACK);
    init_pair(5, COLOR_GREEN, COLOR_BLACK);

    clear();
    draw_stage();
    refresh();
    getch();
    endwin();
    return 0;
}