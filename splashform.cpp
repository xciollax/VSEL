#include "splashform.h"
#include "ui_splashform.h"
#include <QThread>
#include <qdebug.h>

SplashForm::SplashForm(QWidget *parent) :
    AbstractForm(parent),
    ui(new Ui::SplashForm) {
    ui->setupUi(this);

    //sleep for 2 secs
    qDebug() << "about to sleep..";
    QThread::sleep(2);
    qDebug() << "woke up..";

    emit transition("tsplash");
}

SplashForm::~SplashForm()
{
    delete ui;
}

void SplashForm::update() {}

