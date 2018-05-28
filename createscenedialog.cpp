#include "createscenedialog.h"
#include "ui_createscenedialog.h"

CreateSceneDialog::CreateSceneDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateSceneDialog) {
    ui->setupUi(this);

    //wiring local buttons
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(okSlot()));
}

CreateSceneDialog::~CreateSceneDialog() {
    delete ui;
}

void CreateSceneDialog::okSlot() {
    emit(ok(ui->lineEdit->text()));
}
