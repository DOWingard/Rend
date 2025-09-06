#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once

#include <QMainWindow>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setStartState();
    void showActivationScreen();
    void showLicenseInfoScreen();
    void setSSOArgUrl(QString url);

private Q_SLOTS:
    void onLicenseActivated();
    void onSSOUrlReceived(QString url);
    void onLicenseRemoved();
    void onError();
    void onUpdateStatus(QString msg);

private:
    Ui::MainWindow *ui;
    int statusMsgTimeout;
};

#endif // MAINWINDOW_H
