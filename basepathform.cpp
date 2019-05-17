#include "basepathform.h"
#include "ui_basepathform.h"
#include <QFileDialog>
#include <QDir>
#include <QSettings>

BasePathForm::BasePathForm(QWidget *parent) :
    AbstractForm(parent),
    ui(new Ui::BasePathForm)
{
    ui->setupUi(this);
    if(getBasePath() != "") {
        ui->currPathLabel->setText(getBasePath());
    } else {
        ui->currentMsgLabel->hide();
        ui->currPathLabel->hide();
    }

    connect(ui->okButton, SIGNAL(clicked(bool)), this, SLOT(okButtonSlot()));
    connect(ui->cancelButton, SIGNAL(clicked(bool)), this, SLOT(cancelButtonSlot()));
}

BasePathForm::~BasePathForm()
{
    delete ui;
}

void BasePathForm::okButtonSlot() {
    QString bPath = QFileDialog::getExistingDirectory(this, tr("Choose Base Directory"),QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    qDebug() << "bPath: " << bPath ;
    QDir test(bPath);
    if(bPath != "" && test.exists()) {
        setBasePath(bPath);
        QSettings qS;
        qS.setValue("basePath", bPath);
        ui->currPathLabel->setText(getBasePath());
    }

    emit transition("formfs");
}

void BasePathForm::cancelButtonSlot() {
    emit transition("formfs");
}

void BasePathForm::update() {}
