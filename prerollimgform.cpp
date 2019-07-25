#include "prerollimgform.h"
#include "ui_prerollimgform.h"
#include <QPixmap>
#include <QFileDialog>
#include "qdebug.h"

PrerollImgForm::PrerollImgForm(QWidget *parent) :
    AbstractForm(parent),
    ui(new Ui::PrerollImgForm) {
    ui->setupUi(this);

    ui->imgLabel->setScaledContents(false);

    //connect slots
    connect(ui->doneButton, SIGNAL(clicked(bool)), this, SLOT(doneSlot()));
    connect(ui->removeButton, SIGNAL(clicked(bool)), this, SLOT(removeSlot()));
    connect(ui->addChangeButton, SIGNAL(clicked(bool)), this, SLOT(addChangeSlot()));
}

PrerollImgForm::~PrerollImgForm()
{
    delete ui;
}


void PrerollImgForm::update() {
    ui->removeButton->setEnabled(getCurrentScene()->hasPrerollImage());
    displayPrerollImage();
}

void PrerollImgForm::showEvent(QShowEvent *event) {
    displayPrerollImage();
}

//slots
//back to sceneform
void PrerollImgForm::doneSlot() {
    qDebug("doneSlot...");
    emit(transition("formscene"));
}

void PrerollImgForm::addChangeSlot() {
    addPrerollImage();
}

void PrerollImgForm::removeSlot() {
    removePrerollImage();
}

void PrerollImgForm::displayPrerollImage() {
    qDebug("Label dims2: %d %d", ui->imgLabel->width(), ui->imgLabel->height());
    if(getCurrentScene()->hasPrerollImage()) {
        QPixmap pic = QPixmap(getCurrentScene()->prerollImgPath());
        qDebug("pic dims: %d %d", pic.width(), pic.height());
        pic = pic.scaled(ui->imgLabel->width(), ui->imgLabel->height(), Qt::KeepAspectRatio);
        qDebug("pic new dims: %d %d", pic.width(), pic.height());

        ui->imgLabel->hide();
        ui->imgLabel->setPixmap(pic);
        ui->imgLabel->update();
        ui->imgLabel->show();
    } else {
        ui->imgLabel->clear();
    }
}

void PrerollImgForm::addPrerollImage() {
    QString filePath = QFileDialog::getOpenFileName(this,
        tr("Choose Image"), lastOpenDir, tr("Images (*.jpg)"));
    if(filePath != "") {
        //check if image is right
        if(QPixmap(filePath).isNull()) {
            showWarningDialog("Invalid image");
        } else {
            QFileInfo fi(filePath);
            lastOpenDir = fi.canonicalPath();
            //add picture to scene
            getCurrentScene()->addPrerollImage(filePath);
            //preview img
            update();
        }
    }
}

Scene * PrerollImgForm::getCurrentScene() {
    return sharedModel.value("currScene").value<Scene*>();
}

void PrerollImgForm::removePrerollImage() {
    getCurrentScene()->removePrerollImage();
    ui->imgLabel->clear();
}
