#include <iostream>
#include <ncurses.h>
#include <ostream>
#include <fstream>
#include <array>
#include <list>
#include <unordered_map>
#include "commands.h"
#include "doucment.h"

#define MAX_ROWS 9999
#define LINE_NUM_WIDTH 4
#define STARTING_COL (LINE_NUM_WIDTH + 1)
#define MAX_COLS 140

#define ESC_DELAY 100 // 100ms
#define KEY_ESC_ALT 27

typedef enum
{
    LINENUMSWIN,
    TEXTWIN,
    TOTAL_WINDOWS
} windows_t;

typedef enum
{
    MODE_CMD,
    MODE_INSERT,
} win_mode_t;

struct
{
    std::list<char> textdata;
    size_t num_lines;
    std::string filename;
    WINDOW *linenum_win;
    WINDOW *textwin;
    win_mode_t mode;
} wincb;


std::array<WINDOW *, TOTAL_WINDOWS> windows;


void move_left(WINDOW *win)
{
    const int x = getcurx(win);
    wmove(win, getcury(win), x - 1);
    
}

void init_app(void)
{
    wincb.num_lines = 0;
    wincb.mode = MODE_CMD;
    initscr();
    keypad(stdscr, TRUE);
    cbreak();
    noecho();
    nonl();
    set_escdelay(100);
}

WINDOW *init_linenum_win(size_t linenums)
{
    int height, width;
    getmaxyx(stdscr, height, width);
    WINDOW *linenum_win = newwin(height, width, 0, 0);
    wmove(linenum_win, 0, 0);
    refresh();
    for (size_t i = 0; i <= linenums; i++)
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
    wmove(textwin, 0, 0);
    refresh();

    if (filepath == nullptr)
    {
        return textwin;
    }
    std::ifstream file = std::ifstream(filepath);

    char ch;
    while (file.get(ch))
    {
        if (ch == '\n' || ch == '\r')
        {
            wincb.num_lines++;
        }

        wincb.textdata.push_back(ch);
        wprintw(textwin, "%c", ch);
    }
    wmove(textwin, 0, 0);
    wrefresh(textwin);

    return textwin;
}
void render_text()
{
    int ypos, xpos;
    getyx(wincb.textwin, ypos, xpos);

    werase(wincb.textwin);
    wmove(wincb.textwin, 0, 0);

    for(char c : wincb.textdata)
    {
        wprintw(wincb.textwin, "%c", c);
    }
    
    wmove(wincb.textwin, ypos, xpos);
    wrefresh(wincb.textwin);

}
int main(int argc, char **argv)
{

    init_app();
    Document doc;

    if (argc >= 2)
    {
        wincb.filename = std::string(argv[1]);
        wincb.textwin = init_text_win(argv[1]);
        windows[TEXTWIN] = wincb.textwin;
    }
    else
    {
        wincb.textwin = init_text_win(NULL);
        windows[TEXTWIN] = wincb.textwin;

    }
    

    wincb.linenum_win = init_linenum_win(wincb.num_lines);
    render_text();

    int key;
    while ((key = getch()) != KEY_F(1))
    {
        switch (key)
        {
        case KEY_ESC_ALT:
            wincb.mode = MODE_CMD;

            break;
        case KEY_DC:
        case KEY_BACKSPACE:
        case 127:
        case '\b':
            move_left(wincb.textwin);
            break;

        case KEY_LEFT:
            move_left(wincb.textwin);
            break;

        case KEY_RIGHT:
        {
            const int x = getcurx(wincb.textwin);
            
            if (x < getmaxx(wincb.textwin) - 1)
            {
                wmove(wincb.textwin, getcury(wincb.textwin), x + 1);
            }
        }

        break;
        case KEY_UP:
        {
            const int y = getcury(wincb.textwin);

            if (y > 0)
            {
                wmove(wincb.textwin, y - 1, getcurx(wincb.textwin));
            }
        }
        break;
        case KEY_DOWN:
        {
            if (getcury(wincb.textwin) < wincb.num_lines)
            {
                wmove(wincb.textwin, getcury(wincb.textwin) + 1, getcurx(wincb.textwin));
            }
        }
        break;

        case '\r':
        case KEY_ENTER:
        {

            const int next_line = getcury(wincb.textwin) + 1;
            mvwprintw(wincb.linenum_win, next_line, 0, "%d", next_line);
            wmove(wincb.textwin, next_line, STARTING_COL);
            wincb.textdata.push_back('\n');
            if (next_line > wincb.num_lines)
            {
                wincb.num_lines = next_line;
            }
        }
        break;
        default:
            if(wincb.mode == MODE_INSERT)
            {
                int y,x;
                getyx(wincb.textwin, y, x);
                wincb.textdata.push_back(key);
              
                wmove(wincb.textwin, y, x + 1);
                // wprintw(wincb.textwin, "%c", key);
            }
            else if(key == 'i'){
                wincb.mode = MODE_INSERT;
            }
            else{
                command_run(key, doc);
            }
            render_text();
            break;
        }

        for(auto win : windows)
        {
            wrefresh(win);
        }
        // refresh();
    }

    endwin();

    return 0;
}
