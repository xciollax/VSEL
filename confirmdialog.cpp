#include "confirmdialog.h"
#include "ui_confirmdialog.h"

ConfirmDialog::ConfirmDialog(QWidget *parent, QString msg) :
    QDialog(parent),
    ui(new Ui::ConfirmDialog)
{
    ui->setupUi(this);
    setMessage(msg);

    //wiring local buttons
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(okSlot()));

}


ConfirmDialog::~ConfirmDialog()
{
    delete ui;
}

void ConfirmDialog::setMessage(QString msg) {
    ui->message->setText(msg);
}

void ConfirmDialog::okSlot() {
    emit(ok());
}

