#include <iostream>
#include <ncurses.h>

#define MAX_ROWS 9999
#define LINE_NUM_WIDTH 4
#define STARTING_COL (LINE_NUM_WIDTH + 1)
#define MAX_COLS 140

struct
{
    size_t num_lines;

} win_cb;

void move_left(const WINDOW *win)
{
    const int x = getcurx(win);

    if (x > STARTING_COL)
    {
        move(getcury(win), x - 1);
    }
}

WINDOW *init_app()
{
    WINDOW *win = initscr();
    win_cb.num_lines = 0;

    mvprintw(0, 0, "0");
    move(getcury(win), STARTING_COL);

    keypad(stdscr, TRUE);

    return win;
}

int main(int argc, char **argv)
{

    WINDOW *win = init_app();

    int key;
    while ((key = getch()) != KEY_F(1))
    {
        switch (key)
        {
        case KEY_BACKSPACE:
        {
            move_left(win);
            break;
        }
        case KEY_SAVE:
            break;

        case KEY_LEFT:
            move_left(win);
            break;

        case KEY_RIGHT:
        {
            const int x = getcurx(win);

            if (x < MAX_COLS)
            {
                move(getcury(win), x + 1);
            }
        }

        break;
        case KEY_UP:
        {
            const int y = getcury(win);

            if (y > 0)
            {
                move(y - 1, getcurx(win));
            }
        }
        break;
        case KEY_DOWN:
        {
            if (getcury(win) < win_cb.num_lines)
            {
                move(getcury(win) + 1, getcurx(win));
            }
        }
        break;
        case '\n':
        {
            const int next_line = getcury(win) + 1;
            mvprintw(next_line, 0, "%d", next_line);
            move(next_line, STARTING_COL);

            if (next_line > win_cb.num_lines)
            {
                win_cb.num_lines = next_line;
            }
        }
        break;
        default:
            break;
        }
        refresh();
    }

    endwin();

    return 0;
}
