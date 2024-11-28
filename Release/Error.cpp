#include "Error.h"
#include "General.h"

Error::Error(const QString message) {
    this->ErrorBox = new QMessageBox();
    this->infoText = message;
    this->ErrorBox->setWindowTitle(_ERROR_BOX_);
    this->ErrorBox->setInformativeText(message);
    this->ErrorBox->exec();
}

Error::~Error() {
    delete this->ErrorBox;
}

QString Error::what() {
    return infoText;
}

int Error::code() const
{

}
