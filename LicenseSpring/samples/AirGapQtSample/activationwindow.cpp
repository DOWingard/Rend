#include "activationwindow.h"
#include "productdetailsdialog.h"
#include "ui_activationwindow.h"
#include <LicenseSpring/LicenseHandler.h>
#include <qdesktopservices.h>
#include <qfiledialog.h>
#include <qmessagebox.h>

using namespace LicenseSpring;

ActivationWindow::ActivationWindow(QWidget *parent) : QWidget(parent), ui(new Ui::ActivationWindow)
{
    ui->setupUi(this);
}

ActivationWindow::~ActivationWindow() { delete ui; }

void ActivationWindow::on_productDetailsButton_clicked()
{
    ProductDetailsDialog wdg(licenseManager);
    wdg.exec();
}

void ActivationWindow::on_getCodeButton_clicked()
{
    auto key = ui->licenseKeyEdit->text();
    auto initCode = ui->initCodeEdit->text();
    if (!key.isEmpty() && !initCode.isEmpty())
    {
        try
        {
            ui->activationCodeEdit->setText(QString(
                licenseManager->getAirGapActivationCode(initCode.toStdString(), key.toStdString())
                    .c_str()));
            ui->hwidEdit->setText(
                QString(licenseManager->currentConfig()->getHardwareID().c_str()));
        }
        catch (const LicenseSpringException &ex)
        {
            QMessageBox::critical(0, QString("Error"), QString(ex.what()), QMessageBox::Ok);
            return;
        }
    }
    else
        QMessageBox::warning(
            0, QString("Warning"), QString("Type key for activation"), QMessageBox::Ok);
}

void ActivationWindow::on_activateButton_clicked()
{
    auto key = ui->licenseKeyEdit->text();
    auto policy = ui->policyIdEdit->text();
    auto signature = ui->confirmationCodeEdit->text();
    if (policy.isEmpty() || signature.isEmpty())
        QMessageBox::warning(0, QString("Warning"),
            QString("Type policy and signature for activation"), QMessageBox::Ok);
    else
    {
        try
        {
            auto path = ui->policyPathEdit->text();
            if (path.isEmpty())
                QMessageBox::warning(0, QString("Warning"),
                    QString("Type path to the policy file for activation"), QMessageBox::Ok);
            licenseManager->activateAirGapLicense(
                signature.toStdString(), path.toStdWString(), key.toStdString(), policy.toUInt());
            if (licenseManager->getCurrentLicense()->isValid())
                emit licenseActivated();
        }
        catch (const LicenseSpringException &ex)
        {
            QMessageBox::critical(0, QString("Error"), QString(ex.what()), QMessageBox::Ok);
        }
    }
}

void ActivationWindow::on_openButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this, tr("Open File"), "", tr("License Files *.lic All Files *.*"));
    if (fileName.isEmpty())
        return;
    ui->policyPathEdit->setText(fileName);
}

void ActivationWindow::on_openPageButton_clicked()
{
    QDesktopServices::openUrl(QUrl("https://saas.licensespring.com/air-gap/"));
}
