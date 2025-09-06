#pragma once

#include <qwidget.h>

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

private slots:
    void on_productDetailsButton_clicked();

    void on_listHardwareKeysButton_clicked();

    void on_reconfigureButton_clicked();

    void on_activateKeyButton_clicked();

    void on_activateUserButton_clicked();

    void on_userEdit_textChanged(const QString &arg1);

    void on_getTrialButton_clicked();

    void on_createRequestKeyButton_clicked();

    void on_createRequestUserButton_clicked();

    void on_openPageButton_clicked();

    void on_activateOfflineButton_clicked();

    void on_customerEdit_textChanged(const QString &arg1);

signals:
    void licenseActivated();

private:
    Ui::ActivationWindow *ui;
};
