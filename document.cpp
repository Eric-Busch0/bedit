#include "doucment.h"
#include <string>
#include <fstream>

Document::Document()
{
    this->filename = std::string("");
    this->numlines = 0;
}
Document::Document(std::string filename) : filename(filename)
{
}

Document::~Document()
{
}

bool Document::write_out(void)
{
    if (this->filename.empty())
    {
        return false;
    }

    std::ofstream outfile(this->filename);

    if (!outfile.is_open())
    {
        return false;
    }

    for (auto line : this->textdata)
    {
        for (char c : line)
        {
            outfile << c;
        }
    }

    outfile.close();


    return true;
}