#pragma once

#include <qwidget.h>

namespace Ui
{
class LicenseWindow;
}

class LicenseWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LicenseWindow(QWidget *parent = nullptr);
    ~LicenseWindow();

private slots:
    void on_deactivateLicenseButton_clicked();

    void on_checkLicenseButton_clicked();

    void on_productDetailsButton_clicked();

    void on_openFolderButton_clicked();

    void on_loadVersionListButton_clicked();

    void on_checkForUpdatesButton_clicked();

    void on_clearFolderButton_clicked();

    void on_updateConsumptionButton_clicked();

    void on_syncConsumptionButton_clicked();

    void on_updateFeatureConsumptionButton_clicked();

    void on_syncFeatureConsumptionButton_clicked();

    void on_sendDataButton_clicked();

    virtual void showEvent(QShowEvent *);

signals:
    void licenseDeactivated();

private:
    Ui::LicenseWindow *ui;

    void updateLicenseFields();
    void updateCustomFieldsTable();
    void updateLicenseFeaturesTable();
};
