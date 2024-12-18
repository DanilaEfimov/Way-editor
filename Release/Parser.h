#ifndef PARSER_H
#define PARSER_H

#include"General.h"
#include <iostream>

class Parser
{
private:

public:
    Parser();
    virtual ~Parser();

    uint getType(std::string path) const;
};

#endif // PARSER_H
