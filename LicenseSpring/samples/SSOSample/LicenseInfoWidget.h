#ifndef LICENSE_INFO_WIDGET_H
#define LICENSE_INFO_WIDGET_H

#pragma once

#include <QWidget>

namespace Ui
{
class LicenseInfoWidget;
}

class LicenseInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LicenseInfoWidget(QWidget *parent = nullptr);
    ~LicenseInfoWidget();

    void loadLicenseInfo();

Q_SIGNALS:
    void licenseRemoved();
    void errorOccurred();
    void updateStatus(QString msg);

private Q_SLOTS:
    void onRemoveLicense();
    void onOpenLicenseFolder();
    void onCheckLicense();
    void onDeactivateLicense();

private:
    Ui::LicenseInfoWidget *ui;
};

#endif // LICENSE_INFO_WIDGET_H
