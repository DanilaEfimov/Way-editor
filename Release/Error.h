#ifndef ERROR_H
#define ERROR_H

#include<QMessageBox>
#include<QString>
#include<exception>

class Error : public std::exception
{
public:
    Error(const QString message);
    virtual ~Error();

    virtual void show() const;
};

#endif // ERROR_H
