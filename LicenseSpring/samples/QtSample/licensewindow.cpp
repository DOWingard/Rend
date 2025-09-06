#include "licensewindow.h"
#include "devicedatadialog.h"
#include "productdetailsdialog.h"
#include "ui_licensewindow.h"
#include <LicenseSpring/LicenseHandler.h>
#include <LicenseSpring/Version.h>
#include <qfiledialog.h>
#include <qmessagebox.h>

using namespace LicenseSpring;

const QString CurrentVersion = QString::number(VERSION_MAJOR) + "." +
                               QString::number(VERSION_MINOR) + "." +
                               QString::number(VERSION_BUILD);

LicenseWindow::LicenseWindow(QWidget *parent) : QWidget(parent), ui(new Ui::LicenseWindow)
{
    ui->setupUi(this);

    ui->labelAppVersion->setText(CurrentVersion);

    ui->customFieldsTable->setColumnCount(2);
    ui->customFieldsTable->setShowGrid(true);
    ui->customFieldsTable->verticalHeader()->hide();
    ui->customFieldsTable->setSelectionMode(QAbstractItemView::NoSelection);
    ui->customFieldsTable->setHorizontalHeaderLabels({"Name", "Value"});
    ui->customFieldsTable->horizontalHeader()->setStretchLastSection(true);
    ui->customFieldsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->installationFilesTable->setColumnCount(10);
    ui->installationFilesTable->setShowGrid(true);
    ui->installationFilesTable->verticalHeader()->hide();
    ui->installationFilesTable->setSelectionMode(QAbstractItemView::NoSelection);
    ui->installationFilesTable->setHorizontalHeaderLabels(
        {"Version", "Url", "Hash", "Size", "Channel", "Environment", "Release Date",
            "Release Notes", "EULA Link", "Required Version"});
    ui->installationFilesTable->horizontalHeader()->setStretchLastSection(true);
    ui->installationFilesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->licenseFeaturesTable->setColumnCount(9);
    ui->licenseFeaturesTable->setShowGrid(true);
    ui->licenseFeaturesTable->verticalHeader()->hide();
    ui->licenseFeaturesTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->licenseFeaturesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->licenseFeaturesTable->setHorizontalHeaderLabels(
        {"Code", "Name", "Feature type", "Max Consumption", "Total Consumption",
            "Local Consumption", "Expiry Date", "Expiry Date UTC", "Is Expired"});
    ui->licenseFeaturesTable->horizontalHeader()->setStretchLastSection(true);
    ui->licenseFeaturesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void LicenseWindow::showEvent(QShowEvent *)
{
    auto &lh = LicenseHandler::instance();
    auto type = lh.getLicenseType();
    if (type != LicenseTypeConsumption)
    {
        ui->updateConsumptionButton->setDisabled(true);
        ui->syncConsumptionButton->setDisabled(true);
        ui->consumptionSpinBox->setDisabled(true);
    }
    ui->licensePathEdit->setText(QString::fromWCharArray(
        LicenseHandler::instance().currentConfig()->getLicenseFilePath().c_str()));
    updateLicenseFields();
}

void LicenseWindow::updateLicenseFields()
{

    auto &lh = LicenseHandler::instance();
    ui->labelKey->setText(lh.getLicenseKey().c_str());
    ui->labelUser->setText(lh.getLicenseUser().c_str());
    ui->labelType->setText(lh.getLicenseType().toFormattedString().c_str());
    ui->labelStatus->setText(lh.getLicenseStatusStr().c_str());
    ui->labelOfflineActivated->setText(lh.isLicenseOfflineActivated() ? "True" : "False");
    ui->labelTrial->setText(lh.isLicenseTrial() ? "True" : "False");
    ui->labelValid->setText(lh.isLicenseValid() ? "True" : "False");
    ui->labelValidityPeriod->setText(
        LicenseSpring::TmToString(lh.getLicenseExpiryDate(), "%d-%m-%Y %H:%M:%S").c_str());
    ui->labelDaysRemaining->setText(QString::number(lh.getDaysRemaining()));
    ui->labelMaintenancePeriod->setText(
        LicenseSpring::TmToString(lh.getLicenseMaintenancePeriod(), "%d-%m-%Y %H:%M:%S").c_str());
    ui->labelLastChecked->setText(
        LicenseSpring::TmToString(lh.getLicenseLastCheckDate(), "%d-%m-%Y %H:%M:%S").c_str());
    ui->labelFloating->setText(lh.isLicenseFloating() ? "True" : "False");
    ui->labelFloatingTimeout->setText(QString::number(lh.getLicenseFloatingTimeout()));
    ui->labelTotalCount->setText(QString::number(lh.getLicenseTotalConsumption()));
    ui->labelMaxCount->setText(QString::number(lh.getLicenseMaxConsumption()));

    updateCustomFieldsTable();

    updateLicenseFeaturesTable();
}

void LicenseWindow::updateCustomFieldsTable()
{
    auto &lh = LicenseHandler::instance();
    ui->customFieldsTable->clearContents();
    auto fields = lh.getLicenseCustomFields();
    ui->customFieldsTable->setRowCount(static_cast<int>(fields.size()));
    for (int i = 0; i < fields.size(); i++)
    {
        ui->customFieldsTable->setItem(
            i, 0, new QTableWidgetItem(QString::fromStdString(fields[i].fieldName()), 0));
        ui->customFieldsTable->setItem(
            i, 1, new QTableWidgetItem(QString::fromStdString(fields[i].fieldValue()), 0));
    }
}

void LicenseWindow::updateLicenseFeaturesTable()
{
    auto &lh = LicenseHandler::instance();
    ui->licenseFeaturesTable->clearContents();
    auto features = lh.getLicenseFeatures();
    ui->licenseFeaturesTable->setRowCount(static_cast<int>(features.size()));
    for (int i = 0; i < features.size(); i++)
    {
        ui->licenseFeaturesTable->setItem(
            i, 0, new QTableWidgetItem(QString::fromStdString(features[i].code()), 0));
        ui->licenseFeaturesTable->setItem(
            i, 1, new QTableWidgetItem(QString::fromStdString(features[i].name()), 0));
        ui->licenseFeaturesTable->setItem(i, 2,
            new QTableWidgetItem(
                QString::fromStdString(features[i].FeatureTypeToString(features[i].featureType())),
                0));
        ui->licenseFeaturesTable->setItem(
            i, 3, new QTableWidgetItem(QString::number(features[i].maxConsumption()), 0));
        ui->licenseFeaturesTable->setItem(
            i, 4, new QTableWidgetItem(QString::number(features[i].totalConsumption()), 0));
        ui->licenseFeaturesTable->setItem(
            i, 5, new QTableWidgetItem(QString::number(features[i].localConsumption()), 0));
        ui->licenseFeaturesTable->setItem(i, 6,
            new QTableWidgetItem(
                LicenseSpring::TmToString(features[i].expiryDate(), "%d-%m-%Y %H:%M:%S").c_str(),
                0));
        ui->licenseFeaturesTable->setItem(i, 7,
            new QTableWidgetItem(
                LicenseSpring::TmToString(features[i].expiryDateUtc(), "%d-%m-%Y %H:%M:%S").c_str(),
                0));
        ui->licenseFeaturesTable->setItem(
            i, 8, new QTableWidgetItem(features[i].isExpired() ? "Yes" : "No", 0));
    }
}

void LicenseWindow::on_loadVersionListButton_clicked()
{
    auto &lh = LicenseHandler::instance();
    auto list = lh.getVersionList(lh.getLicenseID());
    if (lh.wasError())
    {
        QMessageBox::critical(
            0, QString("Error"), QString(lh.getLastErrorMsg().c_str()), QMessageBox::Ok);
        return;
    }
    ui->installationFilesTable->clearContents();
    ui->installationFilesTable->setRowCount(static_cast<int>(list.size()));
    for (int i = 0; i < list.size(); i++)
    {
        auto installFile = lh.getInstallFile(lh.getLicenseID(), list[i]);
        if (lh.wasError())
        {
            QMessageBox::critical(0, QString("Error"),
                QString("Failed to get install file info for app v") + list[i].c_str() + "\n " +
                    lh.getLastErrorMsg().c_str(),
                QMessageBox::Ok);

            return;
        }

        ui->installationFilesTable->setItem(
            i, 0, new QTableWidgetItem(QString::fromStdString(installFile->version()), 0));
        ui->installationFilesTable->setItem(
            i, 1, new QTableWidgetItem(QString::fromStdString(installFile->url()), 0));
        ui->installationFilesTable->setItem(
            i, 2, new QTableWidgetItem(QString::fromStdString(installFile->md5Hash()), 0));
        ui->installationFilesTable->setItem(
            i, 3, new QTableWidgetItem(QString::number(installFile->size()), 0));
        ui->installationFilesTable->setItem(
            i, 4, new QTableWidgetItem(QString::fromStdString(installFile->channel()), 0));
        ui->installationFilesTable->setItem(
            i, 5, new QTableWidgetItem(QString::fromStdString(installFile->environment()), 0));
        ui->installationFilesTable->setItem(
            i, 6, new QTableWidgetItem(QString::fromStdString(installFile->releaseDate()), 0));
        ui->installationFilesTable->setItem(
            i, 7, new QTableWidgetItem(QString::fromStdString(installFile->releaseNotesLink()), 0));
        ui->installationFilesTable->setItem(
            i, 8, new QTableWidgetItem(QString::fromStdString(installFile->eulaLink()), 0));
        ui->installationFilesTable->setItem(
            i, 9, new QTableWidgetItem(QString::fromStdString(installFile->requiredVersion()), 0));
    }
    ui->installationFilesTable->resizeColumnsToContents();
}

void LicenseWindow::on_checkForUpdatesButton_clicked()
{

    std::string version;
    auto &lh = LicenseHandler::instance();
    auto installFile = lh.getInstallFile(lh.getLicenseID());
    if (installFile)
        version = installFile->version();

    if (version.empty() || lh.wasError())
    {
        QString msg = "Couldn't get app version info.";

        if (lh.wasError())
            msg.append(QString("\n") + lh.getLastErrorMsg().c_str());
        QMessageBox::critical(0, QString("Error"), msg, QMessageBox::Ok);
        return;
    }

    Version serverVer(version);
    Version currentVer(CurrentVersion.toStdString());

    if (currentVer >= serverVer)
        QMessageBox::information(
            0, QString("Info"), QString("The application is up to date."), QMessageBox::Ok);
    else
        QMessageBox::information(0, QString("Info"),
            QString("There is a new version of the application available."), QMessageBox::Ok);
}

LicenseWindow::~LicenseWindow() { delete ui; }

void LicenseWindow::on_deactivateLicenseButton_clicked()
{
    auto &lh = LicenseHandler::instance();
    lh.deactivateLicense();
    if (lh.wasError())
    {
        if (lh.isNoInternetError())
        {
            QString str = QString(lh.getLastErrorMsg().c_str()) +
                          "\n Would you like to make request file for offline deactivation?";

            if (QMessageBox::Ok == QMessageBox::critical(0, QString("Error"), str,
                                       QMessageBox::Ok | QMessageBox::Cancel))
            {
                QString name = QFileDialog::getSaveFileName(
                    this, "Save Request", "", tr("Request Files\0*.req\0All Files\0*.*\0"));

                auto path = lh.deactivateLicenseOffline(name.toStdWString());
                QMessageBox::information(0, QString("Success"),
                    QString("Request path: ") + QString::fromStdWString(path), QMessageBox::Ok);
            }
        }
        else
            QMessageBox::critical(0, QString("Error"),
                QString("Deactivation failed: ") + QString(lh.getLastErrorMsg().c_str()),
                QMessageBox::Ok);
        return;
    }

    if (!lh.wasError())
    {
        lh.clearLocalStorage();
        emit licenseDeactivated();
    }
}

void LicenseWindow::on_checkLicenseButton_clicked()
{

    auto &lh = LicenseHandler::instance();

    lh.checkLicenseLocal();
    if (lh.wasError())
    {
        QMessageBox::critical(0, QString("Error"),
            QString("Local license check failed: ") + QString(lh.getLastErrorMsg().c_str()),
            QMessageBox::Ok);
    }
    lh.checkLicense();
    if (lh.wasError())
    {
        QMessageBox::critical(0, QString("Error"),
            QString("Online license check failed: ") + QString(lh.getLastErrorMsg().c_str()),
            QMessageBox::Ok);
        return;
    }
    updateLicenseFields();
    QMessageBox::information(
        0, QString("Success"), QString("License check passed successfully."), QMessageBox::Ok);
}

void LicenseWindow::on_updateConsumptionButton_clicked()
{
    auto &lh = LicenseHandler::instance();
    lh.updateConsumption(ui->consumptionSpinBox->value());
    if (lh.wasError())
    {
        QMessageBox::critical(
            0, QString("Error"), QString(lh.getLastErrorMsg().c_str()), QMessageBox::Ok);
        return;
    }
    ui->labelTotalCount->setText(QString::number(lh.getLicenseTotalConsumption()));
}

void LicenseWindow::on_syncConsumptionButton_clicked()
{

    auto &lh = LicenseHandler::instance();
    lh.syncConsumption();
    if (lh.wasError())
    {
        QMessageBox::critical(
            0, QString("Error"), QString(lh.getLastErrorMsg().c_str()), QMessageBox::Ok);
        return;
    }
    QMessageBox::information(
        0, QString("Success"), QString("Consumption synchronized!"), QMessageBox::Ok);
}

void LicenseWindow::on_productDetailsButton_clicked()
{
    ProductDetailsDialog wdg;
    wdg.exec();
}

void LicenseWindow::on_openFolderButton_clicked()
{
    auto &lh = LicenseHandler::instance();
    QFileDialog::getOpenFileName(this, "License Directory",
        QString::fromWCharArray(lh.currentConfig()->getLicenseFilePath().c_str()),
        tr("All Files *.*"));
}

void LicenseWindow::on_clearFolderButton_clicked()
{
    auto &lh = LicenseHandler::instance();
    lh.clearLocalStorage();
    emit licenseDeactivated();
}

void LicenseWindow::on_updateFeatureConsumptionButton_clicked()
{
    int value = ui->featureConsumptionSpinBox->value();
    auto selected = ui->licenseFeaturesTable->selectedItems();

    if (!selected.isEmpty())
    {
        auto feature = selected.first();
        auto code = feature->data(0).toString();

        auto &lh = LicenseHandler::instance();
        auto features = lh.getLicenseFeatures();
        for (size_t i = 0; i < features.size(); i++)
        {
            if (features[i].code() == code.toStdString() &&
                features[i].featureType() == FeatureTypeConsumption)
            {
                lh.updateFeatureConsumption(features[i].code(), value);

                if (lh.wasError())
                {
                    QMessageBox::critical(0, QString("Error"),
                        QString(lh.getLastErrorMsg().c_str()), QMessageBox::Ok);
                    return;
                }
                break;
            }
        }
    }

    updateLicenseFeaturesTable();
}

void LicenseWindow::on_syncFeatureConsumptionButton_clicked()
{
    auto selected = ui->licenseFeaturesTable->selectedItems();
    auto &lh = LicenseHandler::instance();
    if (!selected.isEmpty())
    {
        auto feature = selected.first();
        auto code = feature->data(0).toString();
        lh.syncFeatureConsumption(code.toStdString());
    }
    else
        lh.syncFeatureConsumption();

    if (lh.wasError())
    {
        QMessageBox::critical(
            0, QString("Error"), QString(lh.getLastErrorMsg().c_str()), QMessageBox::Ok);
        return;
    }
    updateLicenseFeaturesTable();
}

void LicenseWindow::on_sendDataButton_clicked()
{
    DeviceDataDialog wdg;
    wdg.exec();
}
