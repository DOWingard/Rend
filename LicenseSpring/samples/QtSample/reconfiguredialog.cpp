#include "reconfiguredialog.h"
#include "ui_reconfiguredialog.h"

ReconfigureDialog::ReconfigureDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::ReconfigureDialog)
{
    ui->setupUi(this);
}

ReconfigureDialog::~ReconfigureDialog()
{
    delete ui;
}

QString ReconfigureDialog::apiKey() const
{
    return ui->apiKeyLineEdit->text();
}

QString ReconfigureDialog::sharedKey() const
{
    return ui->sharedKeyLineEdit->text();
}

QString ReconfigureDialog::clientId() const
{
    return ui->clientIdLineEdit->text();
}

QString ReconfigureDialog::clientSecret() const
{
    return ui->clientSecretLineEdit->text();
}

QString ReconfigureDialog::productCode() const
{
    return ui->productCodeLineEdit->text();
}

QString ReconfigureDialog::appName() const
{
    return ui->appNameLineEdit->text();
}

QString ReconfigureDialog::appVersion() const
{
    return ui->appVersionLineEdit->text();
}

QString ReconfigureDialog::hardwareKeySerial() const
{
    return ui->hardwareKeySerialLineEdit->text();
}

QString ReconfigureDialog::hardwareKeyPin() const
{
    return ui->hardwareKeyPinLineEdit->text();
}

void ReconfigureDialog::setApiKey(const QString &v)            { ui->apiKeyLineEdit->setText(v); }
void ReconfigureDialog::setSharedKey(const QString &v)         { ui->sharedKeyLineEdit->setText(v); }
void ReconfigureDialog::setClientId(const QString &v)          { ui->clientIdLineEdit->setText(v); }
void ReconfigureDialog::setClientSecret(const QString &v)      { ui->clientSecretLineEdit->setText(v); }
void ReconfigureDialog::setProductCode(const QString &v)       { ui->productCodeLineEdit->setText(v); }
void ReconfigureDialog::setAppName(const QString &v)           { ui->appNameLineEdit->setText(v); }
void ReconfigureDialog::setAppVersion(const QString &v)        { ui->appVersionLineEdit->setText(v); }
void ReconfigureDialog::setHardwareKeySerial(const QString &v) { ui->hardwareKeySerialLineEdit->setText(v); }
void ReconfigureDialog::setHardwareKeyPin(const QString &v)    { ui->hardwareKeyPinLineEdit->setText(v); }
