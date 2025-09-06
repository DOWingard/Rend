#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <LicenseSpring/LicenseHandler.h>
using namespace LicenseSpring;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), statusMsgTimeout(8000)
{
    ui->setupUi(this);

    connect(ui->activationScreen, &ActivationWidget::licenseActivated, this,
        &MainWindow::onLicenseActivated);
    connect(ui->activationScreen, &ActivationWidget::ssoUrlReceived, this,
        &MainWindow::onSSOUrlReceived);
    connect(ui->activationScreen, &ActivationWidget::errorOccurred, this, &MainWindow::onError);
    connect(ui->licenseScreen, &LicenseInfoWidget::errorOccurred, this, &MainWindow::onError);
    connect(
        ui->licenseScreen, &LicenseInfoWidget::licenseRemoved, this, &MainWindow::onLicenseRemoved);
    connect(ui->licenseScreen, &LicenseInfoWidget::updateStatus, this, &MainWindow::onUpdateStatus);
    connect(
        ui->browserScreen, &BrowserWidget::licenseActivated, this, &MainWindow::onLicenseActivated);
    connect(ui->browserScreen, &BrowserWidget::errorOccurred, this, [this]() {
        onError();
        showActivationScreen();
    });

    ui->centralwidget->setStyleSheet(" QStackedWidget { background: white }");
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::setStartState()
{
    auto &lh = LicenseHandler::instance();
    if (lh.isLicenseExists())
    {
        lh.checkLicenseLocal();
        if (lh.wasError())
            onError();
        else
            onUpdateStatus("Local license check passed.");
        showLicenseInfoScreen();
    }
    else
        showActivationScreen();
}

void MainWindow::showActivationScreen()
{
    ui->stackedWidget->setCurrentWidget(ui->activationScreen);
    ui->activationScreen->initialize();
}

void MainWindow::showLicenseInfoScreen()
{
    ui->stackedWidget->setCurrentWidget(ui->licenseScreen);
    ui->licenseScreen->loadLicenseInfo();
}

void MainWindow::onLicenseActivated()
{
    showLicenseInfoScreen();
    ui->statusbar->showMessage("License successfully activated.", statusMsgTimeout);
}

void MainWindow::onSSOUrlReceived(QString url)
{
    ui->browserScreen->setUrl(url);
    ui->stackedWidget->setCurrentWidget(ui->browserScreen);
}

void MainWindow::setSSOArgUrl(QString url) { ui->browserScreen->onUrlChanged(url); }

void MainWindow::onLicenseRemoved()
{
    showActivationScreen();
    ui->statusbar->showMessage("License has been removed.", statusMsgTimeout);
}

void MainWindow::onError()
{
    auto &lh = LicenseHandler::instance();
    if (lh.wasError())
    {
        QString error("Error: ");
        error.append(lh.getLastErrorMsg().c_str());
        error.append(" Error code: ");
        error.append(QString::number(static_cast<int>(lh.getLastError())));
        ui->statusbar->showMessage(error, statusMsgTimeout);
    }
}

void MainWindow::onUpdateStatus(QString msg) { ui->statusbar->showMessage(msg, statusMsgTimeout); }
