#include <iostream>
#include <ncurses.h>
#include <ostream>
#include <fstream>
#include <array>
#include <list>

#define MAX_ROWS 9999
#define LINE_NUM_WIDTH 4
#define STARTING_COL (LINE_NUM_WIDTH + 1)
#define MAX_COLS 140

struct
{
    std::list<char> textdata;
    size_t num_lines;
    std::string filename;
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
void init_window(const WINDOW *win , std::fstream &file)
{  

    const int window_height = getmaxy(win);

    std::string line;
    
    while(std::getline(file, line) && getcury(win) < window_height)
    {
        printw(line.c_str());
        move(getcury(win) + 1, getcurx(win));
    }
    
}
int main(int argc, char **argv)
{
    
    std::ifstream file;

    if (argc >= 2)
    {
        file.open(argv[1]);
        win_cb.filename = std::string(argv[1]);
        while (!file.eof())
        {
            win_cb.textdata.push_back(file.get());
        }

    }
    else
    {
        win_cb.filename = std::string("temp.txt");
        file.open("temp.txt");
    }

    WINDOW *win = init_app();

    for(auto ch : win_cb.textdata)
    {

        printw("%c", ch);
        
        if(ch == '\n')
        {
            const int current_line = getcury(win);

            printw("%d", current_line);
            move(current_line, STARTING_COL);
        }
    
    }




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
            printw("SAVING");
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

    file.close();

    return 0;
}
