#include <iostream>
#include <ncurses.h>
#include <ostream>
#include <fstream>
#include <array>
#include <list>
#include <unordered_map>

#define MAX_ROWS 9999
#define LINE_NUM_WIDTH 4
#define STARTING_COL (LINE_NUM_WIDTH + 1)
#define MAX_COLS 140

typedef enum
{
    LINENUMSWIN,
    TEXTWIN,
    TOTAL_WINDOWS
} windows_t;
struct
{
    std::list<char> textdata;
    size_t num_lines;
    std::string filename;
    std::array<WINDOW *, TOTAL_WINDOWS> windows;
} win_cb;

void move_left(const WINDOW *win)
{
    const int x = getcurx(win);

    if (x > STARTING_COL)
    {
        move(getcury(win), x - 1);
    }
}

void init_app(void)
{
    win_cb.num_lines = 0;
    initscr();
    keypad(stdscr, TRUE);
    cbreak();
    noecho();
    nonl();
}

WINDOW *init_linenum_win(size_t linenums)
{
    int height, width;
    getmaxyx(stdscr, height, width);
    WINDOW *linenum_win = newwin(height, width, 0, 0);
    refresh();
    for (size_t i = 0; i < linenums; i++)
    {
        mvwprintw(linenum_win, i, 0, "%ld", i + 1);
    }
    wrefresh(linenum_win);
    return linenum_win;
}
WINDOW *init_text_win(const char *filepath)
{
    int height, width;
    getmaxyx(stdscr, height, width);
    WINDOW *textwin = newwin(height, width, 0, STARTING_COL);

    refresh();

    if (filepath == nullptr)
    {
        return textwin;
    }
    std::ifstream file = std::ifstream(filepath);

    char ch;
    while (file.get(ch))
    {
        if (ch == '\n')
        {
            win_cb.num_lines++;
        }

        win_cb.textdata.push_back(ch);
        wprintw(textwin, "%c", ch);
    }
    wmove(textwin, 0, 0);
    wrefresh(textwin);

    return textwin;
}
int main(int argc, char **argv)
{

    init_app();

    if (argc >= 2)
    {
        win_cb.filename = std::string(argv[1]);
        win_cb.windows[TEXTWIN] = init_text_win(argv[1]);
    }
    else
    {
        win_cb.windows[TEXTWIN] = init_text_win(NULL);
    }

    

    // win_cb.windows[LINENUMSWIN] = init_linenum_win(win_cb.num_lines);

    int key;
    while ((key = getch()) != KEY_F(1))
    {
        switch (key)
        {
        case KEY_BACKSPACE:
            move_left(win_cb.windows[TEXTWIN]);
            break;

        case KEY_SAVE:
            printw("SAVING");
            break;

        case KEY_LEFT:
            move_left(win_cb.windows[TEXTWIN]);
            break;

        case KEY_RIGHT:
        {
            const int x = getcurx(win_cb.windows[TEXTWIN]);

            if (x < MAX_COLS)
            {
                move(getcury(win_cb.windows[TEXTWIN]), x + 1);
            }
        }

        break;
        case KEY_UP:
        {
            const int y = getcury(win_cb.windows[TEXTWIN]);

            if (y > 0)
            {
                move(y - 1, getcurx(win_cb.windows[TEXTWIN]));
            }
        }
        break;
        case KEY_DOWN:
        {
            if (getcury(win_cb.windows[TEXTWIN]) < win_cb.num_lines)
            {
                move(getcury(win_cb.windows[TEXTWIN]) + 1, getcurx(win_cb.windows[TEXTWIN]));
            }
        }
        break;

        case '\r':
        case KEY_ENTER:
        {

            const int next_line = getcury(win_cb.windows[TEXTWIN]) + 1;
            mvprintw(next_line, 0, "%d", next_line);
            move(next_line, STARTING_COL);

            if (next_line > win_cb.num_lines)
            {
                win_cb.num_lines = next_line;
            }
        }
        break;
        default:

            printw("%c", key);
            break;
        }
        refresh();
    }

    endwin();

    return 0;
}
