#ifndef DEACTIVATIONDIALOG_H
#define DEACTIVATIONDIALOG_H

#include <QDialog>
#include <LicenseSpring/LicenseManager.h>

namespace Ui
{
class DeactivationDialog;
}

class DeactivationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeactivationDialog(
        LicenseSpring::LicenseManager::ptr_t licenseManager, QWidget *parent = nullptr);
    ~DeactivationDialog();

private slots:
    void on_getCodeButton_clicked();

    void on_deactivateButton_clicked();

private:
    Ui::DeactivationDialog *ui;
    LicenseSpring::LicenseManager::ptr_t licenseManager;
};

#endif // DEACTIVATIONDIALOG_H
