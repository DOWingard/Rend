#pragma once

#include <qdialog.h>

namespace Ui
{
class DeviceDataDialog;
}

class DeviceDataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeviceDataDialog(QWidget *parent = nullptr);
    ~DeviceDataDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DeviceDataDialog *ui;
};
