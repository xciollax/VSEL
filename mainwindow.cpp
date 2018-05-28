#include "mainwindow.h"
#include "sceneslistform.h"
#include "sceneform.h"
#include "basepathform.h"
#include <iostream>
#include <QHBoxLayout>
#include <QFrame>

MainWindow::MainWindow(QWidget *parent, QString basePath) :
    QMainWindow(parent)
{
    hide();
    currentForm = NULL;
    setWindowTitle("Video Slave editor/librarian v." VSEL_V);
    AbstractForm::addToModel("basePath", basePath);
    AbstractForm * tmp = NULL;

    QFrame *fr = new QFrame(this);
    QHBoxLayout *layout = new QHBoxLayout();
    fr->setLayout(layout);
    setCentralWidget(fr);

    tmp = new ScenesListForm(fr);
    connect(tmp, SIGNAL(transition(QString)), this, SLOT(transitionSlot(QString)));
    forms.insert("formfs", tmp);
    layout->addWidget(tmp);
    tmp->hide();

    tmp = new SceneForm(fr);
    connect(tmp, SIGNAL(transition(QString)), this, SLOT(transitionSlot(QString)));
    forms.insert("formscene", tmp);
    layout->addWidget(tmp);
    tmp->hide();

    tmp = new BasePathForm(fr);
    connect(tmp, SIGNAL(transition(QString)), this, SLOT(transitionSlot(QString)));
    forms.insert("formbp", tmp);
    layout->addWidget(tmp);
    tmp->hide();


    if(basePath == "") {
       showForm("formbp");
    } else {
       showForm("formfs");
    }

    //show();
}

MainWindow::~MainWindow() {}

void MainWindow::showForm(QString name) {
    AbstractForm * w = formByName(name);
    if(w != NULL) {
        if(w != currentForm && currentForm != NULL) {
            qDebug("hiding...");
            currentForm->hide();
        }

        currentForm = w;

        w->update();
        w->show();
    }
}

/**
 * @brief MainWindow::transitionSlot
 * @param t nome della transizione rchiesta. Corrisponde al nome della form da mostrare
 */
void MainWindow::transitionSlot(QString t) {
    qDebug("Ricevuta richiesta di transizione %s", t.toLocal8Bit().constData());
    showForm(t);
}

AbstractForm * MainWindow::formByName(QString name) {
    return forms[name];
}
