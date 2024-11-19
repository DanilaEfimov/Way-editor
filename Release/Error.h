#ifndef ERROR_H
#define ERROR_H

#include<QMessageBox>
#include<QString>

class Error
{
private:
    QMessageBox* ErrorBox;
    QString infoText;
public:
    Error(const QString message);
    virtual ~Error();

    QString what() const;
    int code() const;
};

#endif // ERROR_H
