#ifndef ERROR_H
#define ERROR_H

#include<QMessageBox>
#include<QString>
#include<exception>

class Error : private std::exception
{
private:
    QMessageBox* ErrorBox;
    QString infoText;
public:
    Error(const QString message);
    int Warning(const QString message);
    virtual ~Error();
};

#endif // ERROR_H
