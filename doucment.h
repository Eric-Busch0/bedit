#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <stddef.h>
#include <vector>
#include <string>

class Document
{
private:
    /* data */
    size_t numlines;
    std::string filename;
    std::vector<std::vector<char>> textdata;
public:
    Document(/* args */);
    Document(std::string filename);
    ~Document();
    bool write_out(void);

};





#endif