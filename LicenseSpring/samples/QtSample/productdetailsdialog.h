#pragma once

#include <qdialog.h>

namespace Ui
{
class ProductDetailsDialog;
}

class ProductDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProductDetailsDialog(QWidget *parent = nullptr);
    ~ProductDetailsDialog();

private:
    Ui::ProductDetailsDialog *ui;
};
