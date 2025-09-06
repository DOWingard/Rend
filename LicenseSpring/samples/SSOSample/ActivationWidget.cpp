#include "ActivationWidget.h"
#include "ui_ActivationWidget.h"
#include <QRegularExpression>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QStringList>
#include <QMessageBox>
#include <QDebug>
#include <QSettings>
#include <QTextStream>
#include <QUrl>
#include <QStandardPaths>
#include <QDesktopServices>

#include <LicenseSpring/LicenseHandler.h>
using namespace LicenseSpring;

QString EmailRegExp()
{
    return QStringLiteral("^[-a-z0-9!#$%&'*+/=?^_`{|}~]+(?:\\.[-a-z0-9!#$%&'*+/=?^_`"
                          "{|}~]+)*@(?:[a-z0-9]([-a-z0-9]{0,61}[a-z0-9])?\\.)*(?:aer"
                          "o|arpa|asia|biz|cat|com|coop|edu|gov|info|int|jobs|mil|mo"
                          "bi|museum|name|net|org|pro|tel|travel|[a-z][a-z])$");
}

ActivationWidget::ActivationWidget(QWidget *parent) : QWidget(parent), ui(new Ui::ActivationWidget)
{
    ui->setupUi(this);

    connect(ui->userEdit, &QLineEdit::textChanged, this, &ActivationWidget::checkUserAndPassword);
    connect(
        ui->passwordEdit, &QLineEdit::textChanged, this, &ActivationWidget::checkUserAndPassword);
    connect(ui->codeEdit, &QLineEdit::textChanged, this, &ActivationWidget::checkAccountCode);
    connect(ui->bActivate, &QPushButton::clicked, this, &ActivationWidget::onActivate);
    connect(ui->bActivateViaSSO, &QPushButton::clicked, this, &ActivationWidget::onActivateViaSSO);
    connect(ui->bActivateSysBrowser, &QPushButton::clicked, this,
        &ActivationWidget::onActivateViaSSOSysBrowser);
}

ActivationWidget::~ActivationWidget() { delete ui; }

void ActivationWidget::initialize()
{
    auto &lh = LicenseHandler::instance();
    ui->codeEdit->setText(QString::fromStdString(lh.currentConfig()->getCustomerAccount()));

    checkUserAndPassword();
    checkAccountCode();
}

QString ActivationWidget::user() const { return ui->userEdit->text().trimmed(); }

QString ActivationWidget::password() const { return ui->passwordEdit->text().trimmed(); }

QString ActivationWidget::customerAccountCode() const { return ui->codeEdit->text().trimmed(); }

void ActivationWidget::onActivate()
{
    QString userStr = user();
    QString passwordStr = password();

    auto &lh = LicenseHandler::instance();
    lh.activateLicense(LicenseID::fromUser(userStr.toStdString(), passwordStr.toStdString()));

    if (lh.wasError())
    {
        Q_EMIT errorOccurred();
        return;
    }
    else
        Q_EMIT licenseActivated();
}

void ActivationWidget::onActivateViaSSO()
{
    auto &lh = LicenseHandler::instance();
    QString ssoUrl = QString::fromStdString(lh.getSSOUrl(customerAccountCode().toStdString()));
    if (lh.wasError())
        Q_EMIT errorOccurred();
    else
        Q_EMIT ssoUrlReceived(ssoUrl);
}

void ActivationWidget::onActivateViaSSOSysBrowser()
{
    auto &lh = LicenseHandler::instance();
    QString ssoUrl = QString::fromStdString(lh.getSSOUrl(customerAccountCode().toStdString()));
    if (lh.wasError())
    {
        Q_EMIT errorOccurred();
        return;
    }

#if defined(Q_OS_LINUX)
    QProcess subProcess;
    connect(&subProcess, &QProcess::errorOccurred, [this, &subProcess](QProcess::ProcessError err) {
        qDebug() << "Failed to run extermal process: \"" << Qt::endl
                 << "- Program: " << subProcess.program() << Qt::endl
                 << "- Error code: " << (int)err << Qt::endl;
        QMessageBox::critical(this, "Error", "Failed to run extermal browser process");
    });

    if (QStandardPaths::findExecutable("xdg-mime").isEmpty())
    {
        QMessageBox::critical(
            this, "Error", "This action requires xdg-utils package installed in your system");
        return;
    }

    const QString desktopFileName = "licensespring-opener.desktop";
    const QString xdgMimeType = "x-scheme-handler/licensespring";
    auto xdgEntryDir = QDir::homePath() + "/.local/share/applications";
    auto xdgEntryPath = xdgEntryDir + '/' + desktopFileName;

    if (!QDir(xdgEntryDir).exists())
        QDir().mkpath(xdgEntryDir);

    // Create .desktop file and set proper application paths
    QFile desktopFile(xdgEntryPath);
    if (desktopFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QTextStream stream(&desktopFile);
        stream << "[Desktop Entry]" << Qt::endl;
        stream << "Type=Application" << Qt::endl;
        stream << "Name=LicenseSpring authenticator" << Qt::endl;
        stream << QString("Exec=%1 %u").arg(QCoreApplication::applicationFilePath()) << Qt::endl;
        stream << QString("MimeType=%1;").arg(xdgMimeType) << Qt::endl;
        desktopFile.close();
    }

    // Register .desktop file for handling URL scheme
    subProcess.start("xdg-mime", QStringList{"default", desktopFileName, xdgMimeType});
    subProcess.waitForFinished();
    if (subProcess.exitCode())
        return;
#elif defined(Q_OS_WIN)
    // Administrator privileges is required to set the registry keys below
    // Please also note that auth URL should be less than 2048 (INTERNET_MAX_URL_LENGTH) because
    // browser will launch handler app using ShellExecute WinAPI function, and this function has its
    // limitations see
    // https://docs.microsoft.com/en-us/previous-versions/windows/internet-explorer/ie-developer/platform-apis/aa767914(v=vs.85)
    // https://devblogs.microsoft.com/oldnewthing/20031210-00/?p=41553
    QSettings winreg("HKEY_CLASSES_ROOT", QSettings::NativeFormat);
    winreg.setValue("licensespring/.", "URL:LicenseSpring Protocol");
    winreg.setValue("licensespring/URL Protocol", "");
    winreg.setValue("licensespring/shell/open/command/.",
        QString("\"%1\" \"%2\"")
            .arg(QDir::toNativeSeparators(QCoreApplication::applicationFilePath()))
            .arg("%1"));
#else
    QMessageBox::critical(this, "Error", "Not implemented for the current platform");
    return;
#endif
    QDesktopServices::openUrl(QUrl(ssoUrl));
}

void ActivationWidget::checkUserAndPassword()
{
    QString tip;

    if (password().isEmpty())
    {
        tip = tr("The password is empty or contains invalid characters.") + "\n" +
              tr("Please make sure you entered your password correctly.");
    }
    ui->passwordEdit->setToolTip(tip);

    QRegularExpression re(EmailRegExp());
    auto match = re.match(user());

    if (user().isEmpty() || !match.hasMatch())
    {
        tip = tr("User email is empty or invalid.") + "\n" +
              tr("Please make sure you entered your email address correctly.");
        ui->userEdit->setToolTip(tip);
        ui->bActivate->setEnabled(false);
    }
    else
    {
        ui->userEdit->setToolTip(QString());
        ui->bActivate->setEnabled(true);
    }

    ui->bActivate->setToolTip(tip);
    ui->bActivate->setEnabled(tip.isEmpty());
}

void ActivationWidget::checkAccountCode()
{
    QString tip;

    if (customerAccountCode().isEmpty())
    {
        tip = tr("Customer account code is empty or contains invalid characters.") + "\n" +
              tr("Please make sure you entered account code correctly.");
    }

    ui->bActivateViaSSO->setToolTip(tip);
    ui->bActivateViaSSO->setEnabled(tip.isEmpty());
    ui->bActivateSysBrowser->setEnabled(tip.isEmpty());
#ifdef Q_OS_WIN
    tip.append(
        tr(" Please note, administrator privileges is required to set custom URI scheme handler."));
#endif
    ui->bActivateSysBrowser->setToolTip(tip);
}
