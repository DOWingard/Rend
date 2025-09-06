#include "licensewindow.h"
#include "devicedatadialog.h"
#include "productdetailsdialog.h"
#include "deactivationdialog.h"
#include "ui_licensewindow.h"
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
    auto license = licenseManager->getCurrentLicense();
    auto type = license->type();
    if (type != LicenseTypeConsumption)
    {
        ui->updateConsumptionButton->setDisabled(true);
        ui->syncConsumptionButton->setDisabled(true);
        ui->consumptionSpinBox->setDisabled(true);
    }
    ui->licensePathEdit->setText(
        QString::fromWCharArray(licenseManager->currentConfig()->getLicenseFilePath().c_str()));
    updateLicenseFields();
}

void LicenseWindow::updateLicenseFields()
{

    auto license = licenseManager->getCurrentLicense();
    ui->labelKey->setText(license->key().c_str());
    ui->labelUser->setText(license->user().c_str());
    ui->labelType->setText(license->type().toFormattedString().c_str());
    ui->labelStatus->setText(license->status().c_str());
    ui->labelOfflineActivated->setText(license->isOfflineActivated() ? "True" : "False");
    ui->labelTrial->setText(license->isTrial() ? "True" : "False");
    ui->labelValid->setText(license->isValid() ? "True" : "False");
    ui->labelValidityPeriod->setText(
        LicenseSpring::TmToString(license->validityPeriod(), "%d-%m-%Y %H:%M:%S").c_str());
    ui->labelDaysRemaining->setText(QString::number(license->daysRemaining()));
    ui->labelMaintenancePeriod->setText(
        LicenseSpring::TmToString(license->maintenancePeriod(), "%d-%m-%Y %H:%M:%S").c_str());
    ui->labelLastChecked->setText(
        LicenseSpring::TmToString(license->lastCheckDate(), "%d-%m-%Y %H:%M:%S").c_str());
    ui->labelFloating->setText(license->isFloating() ? "True" : "False");
    ui->labelFloatingTimeout->setText(QString::number(license->floatingTimeout()));
    ui->labelTotalCount->setText(QString::number(license->totalConsumption()));
    ui->labelMaxCount->setText(QString::number(license->maxConsumption()));

    updateCustomFieldsTable();

    updateLicenseFeaturesTable();
}

void LicenseWindow::updateCustomFieldsTable()
{
    auto license = licenseManager->getCurrentLicense();
    ui->customFieldsTable->clearContents();
    auto fields = license->customFields();
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
    auto license = licenseManager->getCurrentLicense();
    ui->licenseFeaturesTable->clearContents();
    auto features = license->features();
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
    auto license = licenseManager->getCurrentLicense();
    try
    {
        auto list = licenseManager->getVersionList(license->id());

        ui->installationFilesTable->clearContents();
        ui->installationFilesTable->setRowCount(static_cast<int>(list.size()));
        for (int i = 0; i < list.size(); i++)
        {
            auto installFile = licenseManager->getInstallationFile(license->id(), list[i]);

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
            ui->installationFilesTable->setItem(i, 7,
                new QTableWidgetItem(QString::fromStdString(installFile->releaseNotesLink()), 0));
            ui->installationFilesTable->setItem(
                i, 8, new QTableWidgetItem(QString::fromStdString(installFile->eulaLink()), 0));
            ui->installationFilesTable->setItem(i, 9,
                new QTableWidgetItem(QString::fromStdString(installFile->requiredVersion()), 0));
        }
        ui->installationFilesTable->resizeColumnsToContents();
    }
    catch (LicenseSpringException ex)
    {
        QMessageBox::critical(0, QString("Error"), QString(ex.what()), QMessageBox::Ok);
        return;
    }
}

void LicenseWindow::on_checkForUpdatesButton_clicked()
{

    std::string version;
    auto license = licenseManager->getCurrentLicense();
    try
    {
        auto installFile = licenseManager->getInstallationFile(license->id());
        if (installFile)
            version = installFile->version();

        if (version.empty())
        {
            QMessageBox::critical(
                0, QString("Error"), "Couldn't get app version info.", QMessageBox::Ok);
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
    catch (LicenseSpringException ex)
    {
        QMessageBox::critical(0, QString("Error"), ex.what(), QMessageBox::Ok);
    }
}

LicenseWindow::~LicenseWindow() { delete ui; }

void LicenseWindow::on_deactivateLicenseButton_clicked()
{
    DeactivationDialog wdg(licenseManager);
    wdg.exec();
    if (licenseManager->getCurrentLicense() == nullptr ||
        !licenseManager->getCurrentLicense()->isActive())
        emit licenseDeactivated();
}

void LicenseWindow::on_checkLicenseButton_clicked()
{

    auto license = licenseManager->getCurrentLicense();
    try
    {
        license->localCheck();
    }
    catch (LicenseSpringException ex)
    {
        QMessageBox::critical(0, QString("Error"),
            QString("Local license check failed: ") + QString(ex.what()), QMessageBox::Ok);
    }
    try
    {
        license->check();
    }
    catch (LicenseSpringException ex)
    {
        QMessageBox::critical(0, QString("Error"),
            QString("Online license check failed: ") + QString(ex.what()), QMessageBox::Ok);
    }

    updateLicenseFields();
    QMessageBox::information(
        0, QString("Success"), QString("License check passed successfully."), QMessageBox::Ok);
}

void LicenseWindow::on_updateConsumptionButton_clicked()
{
    auto license = licenseManager->getCurrentLicense();
    try
    {
        license->updateConsumption(ui->consumptionSpinBox->value());
    }
    catch (LicenseStateException ex)
    {
        QMessageBox::critical(0, QString("Error"), QString(ex.what()), QMessageBox::Ok);
        return;
    }
    ui->labelTotalCount->setText(QString::number(license->totalConsumption()));
}

void LicenseWindow::on_syncConsumptionButton_clicked()
{
    auto license = licenseManager->getCurrentLicense();
    try
    {
        license->syncConsumption();
    }
    catch (LicenseSpringException ex)
    {
        QMessageBox::critical(0, QString("Error"), QString(ex.what()), QMessageBox::Ok);
        return;
    }
    QMessageBox::information(
        0, QString("Success"), QString("Consumption synchronized!"), QMessageBox::Ok);
}

void LicenseWindow::on_productDetailsButton_clicked()
{
    ProductDetailsDialog wdg(licenseManager);
    wdg.exec();
}

void LicenseWindow::on_openFolderButton_clicked()
{
    QFileDialog::getOpenFileName(this, "License Directory",
        QString::fromWCharArray(licenseManager->currentConfig()->getLicenseFilePath().c_str()),
        tr("All Files *.*"));
}

void LicenseWindow::on_clearFolderButton_clicked()
{
    licenseManager->clearLocalStorage();
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

        auto license = licenseManager->getCurrentLicense();
        auto features = license->features();
        for (size_t i = 0; i < features.size(); i++)
        {
            if (features[i].code() == code.toStdString() &&
                features[i].featureType() == FeatureTypeConsumption)
            {
                try
                {
                    license->updateFeatureConsumption(features[i].code(), value);
                }
                catch (LicenseSpringException ex)
                {
                    QMessageBox::critical(0, QString("Error"), QString(ex.what()), QMessageBox::Ok);
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
    auto license = licenseManager->getCurrentLicense();
    try
    {
        if (!selected.isEmpty())
        {
            auto feature = selected.first();
            auto code = feature->data(0).toString();
            license->syncFeatureConsumption(code.toStdString());
        }
        else
            license->syncFeatureConsumption();
    }
    catch (LicenseSpringException ex)
    {
        QMessageBox::critical(0, QString("Error"), QString(ex.what()), QMessageBox::Ok);
        return;
    }
    updateLicenseFeaturesTable();
}

void LicenseWindow::on_sendDataButton_clicked()
{
    DeviceDataDialog wdg;
    wdg.setManager(licenseManager);
    wdg.exec();
}
