#pragma once

#include <qwidget.h>
#include <LicenseSpring/LicenseManager.h>

QT_BEGIN_NAMESPACE
namespace Ui
{
class ActivationWindow;
}
QT_END_NAMESPACE

class ActivationWindow : public QWidget
{
    Q_OBJECT

public:
    ActivationWindow(QWidget *parent = nullptr);
    ~ActivationWindow();
    void setManager(LicenseSpring::LicenseManager::ptr_t manager) { licenseManager = manager; }

private slots:
    void on_productDetailsButton_clicked();

    void on_openPageButton_clicked();

    void on_getCodeButton_clicked();

    void on_activateButton_clicked();

    void on_openButton_clicked();

signals:
    void licenseActivated();

private:
    Ui::ActivationWindow *ui;
    LicenseSpring::LicenseManager::ptr_t licenseManager;
};
