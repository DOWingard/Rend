#include "devicedatadialog.h"
#include "ui_devicedatadialog.h"
#include <qmessagebox.h>

DeviceDataDialog::DeviceDataDialog(QWidget *parent) : QDialog(parent), ui(new Ui::DeviceDataDialog)
{
    ui->setupUi(this);
}

DeviceDataDialog::~DeviceDataDialog() { delete ui; }

void DeviceDataDialog::on_buttonBox_accepted()
{
    auto name = ui->nameEdit->text();
    auto value = ui->valueEdit->text();
    if (!name.isEmpty() && !value.isEmpty())
    {
        auto license = licenseManager->getCurrentLicense();
        try
        {
            license->addDeviceVariable(name.toStdString(), value.toStdString());
            license->sendDeviceVariables();
        }
        catch (LicenseSpring::LicenseSpringException ex)
        {
            QMessageBox::critical(0, QString("Error"), QString(ex.what()), QMessageBox::Ok);
            return;
        }
    }
}
