#ifndef CONFIRMDIALOG_H
#define CONFIRMDIALOG_H

#include <QDialog>

namespace Ui {
class ConfirmDialog;
}

class ConfirmDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfirmDialog(QWidget *parent = 0, QString msg = "");
    ~ConfirmDialog();
    void setMessage(QString msg);

private slots:
    void okSlot();
signals:
    void ok();

private:
    Ui::ConfirmDialog *ui;
};

#endif // CONFIRMDIALOG_H
