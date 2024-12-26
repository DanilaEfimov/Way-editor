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
    MainWindow::ErrorReturned->setChecked(false);
}

Error::Error(const QString message, bool isWarning) {
    MainWindow::WarningReturned->setChecked(true);
    this->ErrorBox = new QMessageBox();
    this->infoText = message;
    this->ErrorBox->setWindowTitle(_WARNING_BOX_);
    this->ErrorBox->setInformativeText(message);
    this->ErrorBox->exec();
    MainWindow::WarningReturned->setChecked(false);
}

Error::~Error() {
    delete this->ErrorBox;
    MainWindow::ErrorReturned->setChecked(false);
    MainWindow::ErrorReturned->setChecked(false);
}
