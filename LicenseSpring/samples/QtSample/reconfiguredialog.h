#ifndef RECONFIGUREDIALOG_H
#define RECONFIGUREDIALOG_H

#include <QDialog>

namespace Ui {
class ReconfigureDialog;
}

class ReconfigureDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ReconfigureDialog(QWidget *parent = nullptr);
    ~ReconfigureDialog();

    QString apiKey() const;
    QString sharedKey() const;
    QString clientId() const;
    QString clientSecret() const;
    QString productCode() const;
    QString appName() const;
    QString appVersion() const;
    QString hardwareKeyPin() const;
    QString hardwareKeySerial() const;

    void setApiKey(const QString &v);
    void setSharedKey(const QString &v);
    void setClientId(const QString &v);
    void setClientSecret(const QString &v);
    void setProductCode(const QString &v);
    void setAppName(const QString &v);
    void setAppVersion(const QString &v);
    void setHardwareKeySerial(const QString &v);
    void setHardwareKeyPin(const QString &v);

private:
    Ui::ReconfigureDialog *ui;
};

#endif // RECONFIGUREDIALOG_H
