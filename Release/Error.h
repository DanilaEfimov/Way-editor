#ifndef ERROR_H
#define ERROR_H

#include<exception>

class error : public std::exception
{
public:
    error();
};

#endif // ERROR_H
