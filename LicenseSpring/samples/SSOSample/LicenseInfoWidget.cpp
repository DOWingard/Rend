#include "LicenseInfoWidget.h"
#include "ui_LicenseInfoWidget.h"
#include <QDesktopServices>
#include <QFileInfo>
#include <QDir>
#include <QUrl>

#include <LicenseSpring/LicenseHandler.h>
using namespace LicenseSpring;

LicenseInfoWidget::LicenseInfoWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::LicenseInfoWidget)
{
    ui->setupUi(this);

    connect(ui->bRemoveLicense, &QPushButton::clicked, this, &LicenseInfoWidget::onRemoveLicense);
    connect(ui->bOpenLicenseFolder, &QPushButton::clicked, this,
        &LicenseInfoWidget::onOpenLicenseFolder);
    connect(ui->bCheck, &QPushButton::clicked, this, &LicenseInfoWidget::onCheckLicense);
    connect(ui->bDeactivate, &QPushButton::clicked, this, &LicenseInfoWidget::onDeactivateLicense);
}

LicenseInfoWidget::~LicenseInfoWidget() { delete ui; }

void LicenseInfoWidget::loadLicenseInfo()
{
    auto &lh = LicenseHandler::instance();
    if (!lh.isLicenseExists())
        return;

    ui->eLicenseLocation->setText(QString::fromWCharArray(lh.licenseFilePath().c_str()));
    ui->lType->setText(lh.getLicenseType().toFormattedString().c_str());
    ui->lStatus->setText(lh.getLicenseStatusStr().c_str());

    auto formatStr = [](std::string &str, const std::string &value) {
        if (value.empty())
            return;
        if (!str.empty())
            str.append(" ");
        str.append(value);
    };

    const auto &licenseOwner = lh.getLicenseOwner();
    {
        std::string ownerInfo;
        formatStr(ownerInfo, licenseOwner.firstName());
        formatStr(ownerInfo, licenseOwner.lastName());
        formatStr(ownerInfo, licenseOwner.email());
        formatStr(ownerInfo, licenseOwner.company());
        if (!ownerInfo.empty())
            ui->lCustomer->setText(ownerInfo.c_str());
    }

    auto licenseUser = lh.getLicenseUserPtr();
    if (licenseUser)
    {
        std::string userInfo;
        formatStr(userInfo, licenseUser->firstName());
        formatStr(userInfo, licenseUser->lastName());
        formatStr(userInfo, licenseUser->email());
        if (!userInfo.empty())
            ui->lUser->setText(userInfo.c_str());
    }

    ui->lTrial->setText(lh.isLicenseTrial() ? "True" : "False");
    ui->lFloating->setText(lh.isLicenseFloating() ? "True" : "False");

    ui->lStartDate->setText(lh.getLicenseStartDate().c_str());
    ui->lValidity->setText(
        LicenseSpring::TmToString(lh.getLicenseExpiryDate(), "%d-%m-%Y %H:%M:%S").c_str());
    ui->lMaintenance->setText(
        LicenseSpring::TmToString(lh.getLicenseMaintenancePeriod(), "%d-%m-%Y %H:%M:%S").c_str());
    ui->lCheckDate->setText(
        LicenseSpring::TmToString(lh.getLicenseLastCheckDate(), "%d-%m-%Y %H:%M:%S").c_str());

    ui->lTimesActivated->setText(QString::number(lh.getLicenseTimesActivated()));
    ui->lMaxActivations->setText(QString::number(lh.getLicenseMaxActivations()));
}

void LicenseInfoWidget::onRemoveLicense()
{
    auto &lh = LicenseHandler::instance();
    lh.clearLocalStorage();
    Q_EMIT licenseRemoved();
}

void LicenseInfoWidget::onOpenLicenseFolder()
{
    QFileInfo fi(ui->eLicenseLocation->text());
    if (!fi.exists())
        return;
    QDesktopServices::openUrl(QUrl::fromLocalFile(fi.dir().absolutePath()));
}

void LicenseInfoWidget::onCheckLicense()
{
    auto &lh = LicenseHandler::instance();
    if (!lh.isLicenseExists())
        return;

    lh.checkLicense();
    loadLicenseInfo();

    if (lh.wasError())
        Q_EMIT errorOccurred();
    else
        Q_EMIT updateStatus("License online check passed successfully.");
}

void LicenseInfoWidget::onDeactivateLicense()
{
    auto &lh = LicenseHandler::instance();
    if (!lh.isLicenseExists())
        return;

    if (!lh.isLicenseActive())
    {
        Q_EMIT updateStatus("License is inactive.");
        return;
    }

    if (lh.deactivateLicense())
    {
        Q_EMIT updateStatus("License has been successfully deactivated.");
        ui->lStatus->setText(lh.getLicenseStatusStr().c_str());
    }
    else
        Q_EMIT errorOccurred();
}
