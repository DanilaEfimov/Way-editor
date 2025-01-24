#include "Error.h"
#include "General.h"
#include "mainwindow.h"

Error::Error(const QString message) {
    MainWindow::ErrorReturned->setCheckable(true);
    MainWindow::ErrorReturned->setChecked(true);
    this->ErrorBox = new QMessageBox();
    this->infoText = message;
    this->ErrorBox->setWindowTitle(_ERROR_BOX_);
    this->ErrorBox->setInformativeText(message);
    this->ErrorBox->setIcon(QMessageBox::Information);
    this->ErrorBox->exec();
}

Error::Error(const QString message, bool isWarning) {
    MainWindow::WarningReturned->setCheckable(true);
    MainWindow::WarningReturned->setChecked(true);
    this->ErrorBox = new QMessageBox();
    this->infoText = message;
    this->ErrorBox->setWindowTitle(_WARNING_BOX_);
    this->ErrorBox->setInformativeText(message);
    this->ErrorBox->setIcon(QMessageBox::Warning);
    this->ErrorBox->exec();
}

Error::~Error() {
    delete this->ErrorBox;
    MainWindow::ErrorReturned->setChecked(false);
    MainWindow::WarningReturned->setChecked(false);
     MainWindow::ErrorReturned->setCheckable(false);
     MainWindow::WarningReturned->setCheckable(false);
}
