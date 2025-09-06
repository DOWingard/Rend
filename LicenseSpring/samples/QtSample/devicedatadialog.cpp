#include "devicedatadialog.h"
#include "ui_devicedatadialog.h"
#include <LicenseSpring/LicenseHandler.h>
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
        auto &lh = LicenseSpring::LicenseHandler::instance();
        lh.addDeviceVariable(name.toStdString(), value.toStdString());
        lh.sendDeviceVariables();
        if (lh.wasError())
        {
            QMessageBox::critical(
                0, QString("Error"), QString(lh.getLastErrorMsg().c_str()), QMessageBox::Ok);
            return;
        }
    }
}
