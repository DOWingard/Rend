#pragma once

#include <qmainwindow.h>

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

public slots:
    void openActivationPage();
    void openLicensePage();

private:
    Ui::MainWindow *ui;
};
