#include "Error.h"
#include "General.h"
#include "mainwindow.h"

Error::Error(const QString message) {
    MainWindow::ErrorReturned->setChecked(true);
    this->ErrorBox = new QMessageBox();
    this->infoText = message;
    this->ErrorBox->setWindowTitle(_ERROR_BOX_);
    this->ErrorBox->setInformativeText(message);
    this->ErrorBox->exec();
}

int Error::Warning(const QString message) {
    MainWindow::WarningReturned->setChecked(true);
    this->ErrorBox = new QMessageBox();
    this->infoText = message;
    this->ErrorBox->setWindowTitle(_ERROR_BOX_);
    this->ErrorBox->setInformativeText(message);
    int _code = this->ErrorBox->exec();
    return _code;
}

Error::~Error() {
    delete this->ErrorBox;
    MainWindow::ErrorReturned->setChecked(false);
    MainWindow::ErrorReturned->setChecked(false);
}
