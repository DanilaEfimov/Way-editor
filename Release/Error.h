#ifndef ERROR_H
#define ERROR_H

#include<QMessageBox>
#include<QString>
#include<exception>

class Error : private std::exception
{
private:
    static QMessageBox* ErrorBox;
    static QString infoText;
public:
    Error(const QString message);
    Error(const QString message, bool isWarning);
    virtual ~Error();
};

#endif // ERROR_H
