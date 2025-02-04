#include "Error.h"
#include "General.h"
#include "mainwindow.h"

QMessageBox* Error::ErrorBox = nullptr;
QString Error::infoText = "";

Error::Error(const QString message) {
    MainWindow::ErrorReturned->setCheckable(true);
    MainWindow::ErrorReturned->setChecked(true);
    ErrorBox = new QMessageBox();
    infoText = message;
    ErrorBox->setWindowTitle(_ERROR_BOX_);
    ErrorBox->setInformativeText(message);
    ErrorBox->setIcon(QMessageBox::Information);
    //ErrorBox->setStyleSheet("color: #F00F0F; background-color: #F0F0F0;");
    ErrorBox->exec();
}

Error::Error(const QString message, bool isWarning) {
    MainWindow::WarningReturned->setCheckable(true);
    MainWindow::WarningReturned->setChecked(true);
    ErrorBox = new QMessageBox();
    infoText = message;
    ErrorBox->setWindowTitle(_WARNING_BOX_);
    ErrorBox->setInformativeText(message);
    ErrorBox->setIcon(QMessageBox::Warning);
    //ErrorBox->setStyleSheet("color: #FFFFFF; background-color: #FF90AF;");
    ErrorBox->exec();
}

Error::~Error() {
    delete ErrorBox;
    MainWindow::ErrorReturned->setChecked(false);
    MainWindow::WarningReturned->setChecked(false);
    MainWindow::ErrorReturned->setCheckable(false);
    MainWindow::WarningReturned->setCheckable(false);
}
