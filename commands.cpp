#include <unordered_map>
#include "doucment.h"

typedef void (*command_t)(Document &doc);

static void save_text(Document &doc);

std::unordered_map<char, command_t> commands = {
    {'s', save_text},
    };

void command_run(char c, Document &doc)
{
    if (commands.find(c) != commands.end() && commands.at(c) != nullptr)
    {
        const auto command = commands.at(c);
        command(doc);
    }
}

static void save_text(Document &doc)
{
    
}
