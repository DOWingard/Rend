#include "mainwindow.h"
#include "../AppConfig.h"
#include "ui_mainwindow.h"
#include <LicenseSpring/LicenseManager.h>
#include <qfileinfo.h>
#include <qmessagebox.h>

using namespace LicenseSpring;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(
        ui->licensePage, &LicenseWindow::licenseDeactivated, this, &MainWindow::openActivationPage);
    connect(ui->activationPage, &ActivationWindow::licenseActivated, this,
        &MainWindow::openLicensePage);
    QString name = QFileInfo(QCoreApplication::applicationFilePath()).fileName();
    QString ver = QString::number(VERSION_MAJOR) + "." + QString::number(VERSION_MINOR) + "." +
                  QString::number(VERSION_BUILD);
    AppConfig appConfig(name.toStdString(), ver.toStdString());

    auto config = appConfig.createLicenseSpringConfig();
    auto licenseManager = LicenseSpring::LicenseManager::create(config);

    ui->activationPage->setManager(licenseManager);
    ui->licensePage->setManager(licenseManager);
    auto license = licenseManager->getCurrentLicense();

    if (license != nullptr)
    {
        try
        {
            license->check();
        }
        catch (LicenseServerException ex)
        {
            QMessageBox::critical(0, QString("Error"),
                QString("Local license check failed: ") + QString(ex.what()), QMessageBox::Ok);
            ui->stackedWidget->setCurrentWidget(ui->activationPage);
        }
        ui->stackedWidget->setCurrentWidget(ui->licensePage);
    }
    else
        ui->stackedWidget->setCurrentWidget(ui->activationPage);
}

void MainWindow::openActivationPage() { ui->stackedWidget->setCurrentWidget(ui->activationPage); }

void MainWindow::openLicensePage() { ui->stackedWidget->setCurrentWidget(ui->licensePage); }

MainWindow::~MainWindow() { delete ui; }
