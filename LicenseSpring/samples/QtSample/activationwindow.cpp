#include "activationwindow.h"
#include "productdetailsdialog.h"
#include "reconfiguredialog.h"
#include "ui_activationwindow.h"
#include <LicenseSpring/LicenseHandler.h>
#include <qdesktopservices.h>
#include <qfiledialog.h>
#include <qmessagebox.h>

using namespace LicenseSpring;

ActivationWindow::ActivationWindow(QWidget *parent) : QWidget(parent), ui(new Ui::ActivationWindow)
{
    ui->setupUi(this);
}

ActivationWindow::~ActivationWindow() { delete ui; }

void ActivationWindow::on_productDetailsButton_clicked()
{

    ProductDetailsDialog wdg;
    wdg.exec();
    bool enableKeyBased = false;
    bool enableUserBased = false;
    auto &lh = LicenseHandler::instance();
    auto details = lh.getProductDetails();
    if (details->authorizationMethod() == AuthMethodKeyBased)
        enableKeyBased = true;
    if (details->authorizationMethod() == AuthMethodUserBased)
        enableUserBased = true;

    ui->userEdit->setEnabled(enableUserBased);
    ui->passwordEdit->setEnabled(enableUserBased);
    ui->activateUserButton->setEnabled(enableUserBased);
    ui->createRequestUserButton->setEnabled(enableUserBased);
    ui->activateKeyButton->setEnabled(enableKeyBased);
    ui->licenseKeyEdit->setEnabled(enableKeyBased);
    ui->createRequestKeyButton->setEnabled(enableKeyBased);
}

void ActivationWindow::on_reconfigureButton_clicked()
{
    auto &lh = LicenseHandler::instance();
    auto currentConfig = lh.currentConfig();
    ReconfigureDialog dlg(this);
    dlg.setApiKey(QString::fromStdString(currentConfig->getApiKey()));
    dlg.setSharedKey(QString::fromStdString(currentConfig->getSharedKey()));
    dlg.setClientId(QString::fromStdString(currentConfig->getClientId()));
    dlg.setClientSecret(QString::fromStdString(currentConfig->getClientSecret()));
    dlg.setProductCode(QString::fromStdString(currentConfig->getProductCode()));
    dlg.setAppName(QString::fromStdString(currentConfig->getAppName()));
    dlg.setAppVersion(QString::fromStdString(currentConfig->getAppVersion()));
    dlg.setHardwareKeySerial(QString::fromStdString(currentConfig->getHardwareKeyTargetSerial()));
    dlg.setHardwareKeyPin(QString::fromStdString(currentConfig->getHardwareKeyPin()));
    if (dlg.exec() == QDialog::Accepted) {
        ExtendedOptions options;
        HardwareKeyOptions hwKeyOptions;
        hwKeyOptions.setTargetSerial(dlg.hardwareKeySerial().toStdString());
        hwKeyOptions.setPin(dlg.hardwareKeyPin().toStdString());
        options.setHardwareKeyOptions(hwKeyOptions);

        if (!currentConfig->getAlternateServiceUrl().empty())
            options.setAlternateServiceURL(currentConfig->getAlternateServiceUrl());
        if (!currentConfig->getAlternateKey().empty())
            options.setAlternateKey(currentConfig->getAlternateKey());

        Configuration::ptr_t config = nullptr;
        if (dlg.apiKey().isEmpty() || dlg.sharedKey().isEmpty())
            config = Configuration::CreateOAuth(dlg.clientId().toStdString(), dlg.clientSecret().toStdString(), dlg.productCode().toStdString(), dlg.appName().toStdString(), dlg.appVersion().toStdString(), options);
        else
            config = Configuration::Create(dlg.apiKey().toStdString(), dlg.sharedKey().toStdString(), dlg.productCode().toStdString(), dlg.appName().toStdString(), dlg.appVersion().toStdString(), options);
        lh.reconfigure(config);
    }
}

void ActivationWindow::on_listHardwareKeysButton_clicked()
{
    auto hardwareKeySerials = LicenseSpring::HardwareKeyOptions::ListAvailableKeys();
    ui->hardwareKeyList->clear();
    for (const auto &k : hardwareKeySerials)
        ui->hardwareKeyList->addItem(QString::fromStdString(k));
}

void ActivationWindow::on_activateKeyButton_clicked()
{
    auto key = ui->licenseKeyEdit->text();
    if (!key.isEmpty())
    {
        auto &lh = LicenseHandler::instance();
        lh.activateLicense(LicenseID::fromKey(key.toStdString()));

        if (lh.wasError())
        {
            QMessageBox::critical(
                0, QString("Error"), QString(lh.getLastErrorMsg().c_str()), QMessageBox::Ok);
            return;
        }

        if (lh.isLicenseValid())
            emit licenseActivated();
    }
    else
        QMessageBox::warning(
            0, QString("Warning"), QString("Type key for activation"), QMessageBox::Ok);
}

void ActivationWindow::on_activateUserButton_clicked()
{

    auto user = ui->userEdit->text();
    auto password = ui->passwordEdit->text();
    if (user.isEmpty() || password.isEmpty())
        QMessageBox::warning(0, QString("Warning"),
            QString("Type user emal and password for activation"), QMessageBox::Ok);
    else
    {
        auto &lh = LicenseHandler::instance();
        lh.activateLicense(LicenseID::fromUser(user.toStdString(), password.toStdString()));

        if (lh.wasError())
        {
            QMessageBox::critical(
                0, QString("Error"), QString(lh.getLastErrorMsg().c_str()), QMessageBox::Ok);
            return;
        }

        if (lh.isLicenseValid())
            emit licenseActivated();
    }

    QMessageBox::critical(
        0, QString("Error"), QString("Unknown exception encountered!"), QMessageBox::Ok);
}

void ActivationWindow::on_userEdit_textChanged(const QString &arg1)
{
    ui->customerEdit->setText(arg1);
}

void ActivationWindow::on_getTrialButton_clicked()
{
    auto user = ui->customerEdit->text();
    auto &lh = LicenseHandler::instance();
    auto licenseId = lh.getTrialLicense(user.toStdString());
    if (lh.wasError())
    {
        QMessageBox::critical(0, QString("Error"),
            QString("Get trial license error:\n") + lh.getLastErrorMsg().c_str(), QMessageBox::Ok);
        return;
    }

    if (!licenseId.isValid())
    {
        QMessageBox::critical(
            0, QString("Error"), QString("Invalid trial license"), QMessageBox::Ok);
        return;
    }

    ui->userEdit->setText(licenseId.user().c_str());
    ui->passwordEdit->setText(licenseId.password().c_str());
    ui->licenseKeyEdit->setText(licenseId.key().c_str());
}

void ActivationWindow::on_createRequestUserButton_clicked()
{
    auto user = ui->userEdit->text();
    auto password = ui->passwordEdit->text();
    if (user.isEmpty() || password.isEmpty())
        QMessageBox::warning(0, QString("Warning"),
            QString("Type user emal and password to create request"), QMessageBox::Ok);
    else
    {
        QString fileName = QFileDialog::getSaveFileName(
            this, tr("Save File"), "", tr("Request Files *.req All Files *.*"));
        std::wstring fileWString = fileName.toStdWString();

        auto &lh = LicenseHandler::instance();
        auto path = lh.createOfflineActivationFile(
            LicenseID::fromUser(user.toStdString(), password.toStdString()), fileWString);

        if (lh.wasError())
        {
            QMessageBox::critical(
                0, QString("Error"), QString(lh.getLastErrorMsg().c_str()), QMessageBox::Ok);
            return;
        }

        QMessageBox::information(0, QString("Success"),
            QString("Request file path: ") + QString::fromWCharArray(path.c_str()),
            QMessageBox::Ok);
    }
}

void ActivationWindow::on_createRequestKeyButton_clicked()
{
    auto key = ui->licenseKeyEdit->text();
    if (!key.isEmpty())
    {
        QString fileName = QFileDialog::getSaveFileName(
            this, tr("Save File"), "", tr("Request Files *.req All Files *.*"));

        std::wstring fileWString = fileName.toStdWString();
        auto &lh = LicenseHandler::instance();
        auto path =
            lh.createOfflineActivationFile(LicenseID::fromKey(key.toStdString()), fileWString);
        if (lh.wasError())
        {
            QMessageBox::critical(
                0, QString("Error"), QString(lh.getLastErrorMsg().c_str()), QMessageBox::Ok);
            return;
        }
        QMessageBox::information(0, QString("Success"),
            QString("Request file path: ") + QString::fromWCharArray(path.c_str()),
            QMessageBox::Ok);
    }
    else
        QMessageBox::warning(
            0, QString("Warning"), QString("Type key to create request"), QMessageBox::Ok);
}

void ActivationWindow::on_openPageButton_clicked()
{
    QDesktopServices::openUrl(QUrl("https://saas.licensespring.com/offline/"));
}

void ActivationWindow::on_activateOfflineButton_clicked()
{

    QString fileName = QFileDialog::getOpenFileName(
        this, tr("Open File"), "", tr("License Files *.lic All Files *.*"));
    if (fileName.isEmpty())
        return;
    std::wstring fileWString = fileName.toStdWString();
    auto &lh = LicenseHandler::instance();
    lh.activateLicenseOffline(fileWString);
    if (lh.wasError())
    {
        QMessageBox::critical(
            0, QString("Error"), QString(lh.getLastErrorMsg().c_str()), QMessageBox::Ok);
        return;
    }
    if (lh.isLicenseValid())
        emit licenseActivated();
}

void ActivationWindow::on_customerEdit_textChanged(const QString &arg1)
{
    ui->userEdit->setText(arg1);
}
