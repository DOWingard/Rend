#ifndef BROWSERWIDGET_H
#define BROWSERWIDGET_H

#pragma once

#include <QWidget>
#include <QUrl>

extern const QString LS_URL_Scheme;

namespace Ui
{
class BrowserWidget;
}

class BrowserWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BrowserWidget(QWidget *parent = nullptr);
    ~BrowserWidget();

    void setUrl(QString url);

    Q_SIGNAL void errorOccurred();
    Q_SIGNAL void licenseActivated();

    Q_SLOT void onUrlChanged(const QUrl &url);

private:
    Ui::BrowserWidget *ui;
};

#endif // BROWSERWIDGET_H
