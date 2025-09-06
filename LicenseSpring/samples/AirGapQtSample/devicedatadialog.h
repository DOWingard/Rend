#pragma once

#include <qdialog.h>
#include <LicenseSpring/LicenseManager.h>

namespace Ui
{
class DeviceDataDialog;
}

class DeviceDataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeviceDataDialog(QWidget *parent = nullptr);
    void setManager(LicenseSpring::LicenseManager::ptr_t manager) { licenseManager = manager; }
    ~DeviceDataDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DeviceDataDialog *ui;
    LicenseSpring::LicenseManager::ptr_t licenseManager;
};
