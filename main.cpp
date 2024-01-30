#include <iostream>
#include <ncurses.h>

void move_left(const WINDOW *win)
{
    const int x = getcurx(win);

    if (x > 0)
    {
        move(getcury(win), x - 1);
    }
}


int main(int argc, char **argv)
{
    WINDOW *win = initscr();

    keypad(stdscr, TRUE);

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

        case KEY_LEFT:
            move_left(win);
            break;

        case KEY_RIGHT:
            move(getcury(win), getcurx(win) + 1);
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
        case '\n':
            move(getcury(win) + 1, getcurx(win));
    
        break;
        default:
            break;
        }
        refresh();
    }

    endwin();

    return 0;
}
