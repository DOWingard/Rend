#pragma once

#include <qdialog.h>
#include <LicenseSpring/LicenseManager.h>

namespace Ui
{
class ProductDetailsDialog;
}

class ProductDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProductDetailsDialog(
        LicenseSpring::LicenseManager::ptr_t licenseManager, QWidget *parent = nullptr);
    ~ProductDetailsDialog();

private:
    Ui::ProductDetailsDialog *ui;
};
