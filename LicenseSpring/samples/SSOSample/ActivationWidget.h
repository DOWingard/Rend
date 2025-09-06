#ifndef ACTIVATION_WIDGET_H
#define ACTIVATION_WIDGET_H

#pragma once

#include <QWidget>

namespace Ui
{
class ActivationWidget;
}

class ActivationWidget : public QWidget
{
    Q_OBJECT

public:
    ActivationWidget(QWidget *parent = nullptr);
    ~ActivationWidget();

    void initialize();

    QString user() const;
    QString password() const;
    QString customerAccountCode() const;

    Q_SIGNAL void errorOccurred();
    Q_SIGNAL void licenseActivated();
    Q_SIGNAL void ssoUrlReceived(QString url);

private Q_SLOTS:
    void onActivate();
    void onActivateViaSSO();
    void onActivateViaSSOSysBrowser();
    void checkUserAndPassword();
    void checkAccountCode();

private:
    Ui::ActivationWidget *ui;
};

#endif // ACTIVATION_WIDGET_H
