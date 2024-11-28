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
    virtual ~Error();

    inline QString what();
    int code() const;
};

#endif // ERROR_H
