#include "productdetailsdialog.h"
#include "ui_productdetailsdialog.h"

ProductDetailsDialog::ProductDetailsDialog(
    LicenseSpring::LicenseManager::ptr_t licenseManager, QWidget *parent)
    : QDialog(parent), ui(new Ui::ProductDetailsDialog)
{
    ui->setupUi(this);

    auto details = licenseManager->getProductDetails();

    ui->labelProductName->setText(details.productName().c_str());
    ui->labelProductCode->setText(details.productCode().c_str());

    switch (details.authorizationMethod())
    {
    case LSAuthMethod::AuthMethodKeyBased:
        ui->labelAuthType->setText("Key Based");
        break;
    case LSAuthMethod::AuthMethodUserBased:
        ui->labelAuthType->setText("User Based");
        break;
    default:
        ui->labelAuthType->setText("Unknown");
    }
    ui->labelTrialAllowed->setText(details.isTrialAllowed() ? "True" : "False");
    ui->labelTrialPeriod->setText(QString::number(details.trialPeriod()));
}

ProductDetailsDialog::~ProductDetailsDialog() { delete ui; }
