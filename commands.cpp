#include <unordered_map>

typedef void (*command_t)(void);
std::unordered_map<char, command_t> commands = {
    {'s',  nullptr},
    {'i', }
};



void command_run(char c)
{

}