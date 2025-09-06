#include "deactivationdialog.h"
#include "ui_deactivationdialog.h"
#include <qmessagebox.h>

using namespace LicenseSpring;

DeactivationDialog::DeactivationDialog(LicenseManager::ptr_t manager, QWidget *parent)
    : QDialog(parent), ui(new Ui::DeactivationDialog), licenseManager(manager)
{
    ui->setupUi(this);
    ui->licenseKeyEdit->setText(licenseManager->getCurrentLicense()->key().c_str());
}

DeactivationDialog::~DeactivationDialog() { delete ui; }

void DeactivationDialog::on_getCodeButton_clicked()
{
    auto key = licenseManager->getCurrentLicense()->key();
    auto initCode = ui->initCodeEdit->text();
    auto license = licenseManager->getCurrentLicense();
    if (!key.empty() && !initCode.isEmpty())
    {
        try
        {
            ui->activationCodeEdit->setText(
                QString(license->getAirGapDeactivationCode(initCode.toStdString()).c_str()));
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
            0, QString("Warning"), QString("Type key for deactivation"), QMessageBox::Ok);
}

void DeactivationDialog::on_deactivateButton_clicked()
{
    auto key = licenseManager->getCurrentLicense()->key();
    auto confirmationCode = ui->confirmationCodeEdit->text();
    auto license = licenseManager->getCurrentLicense();
    if (confirmationCode.isEmpty())
        QMessageBox::warning(0, QString("Warning"),
            QString("Type confirmation code for deactivation"), QMessageBox::Ok);
    else
    {
        try
        {
            license->deactivateAirGap(confirmationCode.toStdString());
            close();
        }
        catch (AirGapActivationException ex)
        {
            QMessageBox::critical(0, QString("Error"), QString(ex.what()), QMessageBox::Ok);
        }
    }
}
