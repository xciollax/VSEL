#include "sceneform.h"
#include "ui_sceneform.h"
#include <QDir>
#include <QVariant>
#include <iostream>
#include <QFileDialog>
#include "noteeditdelegate.h"
#include "blurtypedelegate.h"
#include "zoomtypedelegate.h"
#include "qdebug.h"
#include <QStringBuilder>

SceneForm::SceneForm(QWidget *parent) :
    AbstractForm(parent),
    ui(new Ui::SceneForm) {
    ui->setupUi(this);

    //wiring pushbuttons
    connect(ui->doneButton, SIGNAL(clicked(bool)), this, SLOT(doneSlot()));
    connect(ui->addVideoButton, SIGNAL(clicked(bool)), this, SLOT(addVideoSlot()));
    connect(ui->deleteVideoButton, SIGNAL(clicked(bool)), this, SLOT(removeVideoSlot()));
    connect(ui->prerollButton, SIGNAL(clicked(bool)), this, SLOT(prerollSlot()));
}

SceneForm::~SceneForm() {
    delete ui;
    if(model != nullptr) {
        delete model;
    }
}

void SceneForm::setSceneName(QString sName) {
    this->sceneName = sName;
    model = new SceneModel();
}

/**
 * @brief SceneForm::update Deletes previously loaded model, creates new one from shared model
 */
void SceneForm::update() {


    qDebug("Sceneform executing update");

    if(model != nullptr) {
        delete model;
    }

    //retrieve current scene from sharedModel
    QVariant tmpVal = sharedModel.value("currScene");

    //if current scene exists build model
    if(tmpVal.isValid()) {
        qDebug("creating and setting model");
        model = new SceneModel(nullptr, tmpVal.value<Scene*>());
        ui->videoTable->setModel(model);
        ui->sceneNameLabel->setText(tmpVal.value<Scene*>()->name);
        //wiring selection events
        connect(ui->videoTable->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(updateSelection(QItemSelection,QItemSelection)) );
    } else {
        qWarning() << "No selected scene";
        showWarningDialog("please select a scene");
        emit transition("formfs");
    }

    //set custom editors for midi note, blur and zoom types
    ui->videoTable->setItemDelegateForColumn(1, new NoteEditDelegate(this, 0, 127));
    ui->videoTable->setItemDelegateForColumn(3, new BlurtypeDelegate(this));
    ui->videoTable->setItemDelegateForColumn(4, new ZoomtypeDelegate(this));
    //first row autmatically selected
    ui->videoTable->resizeColumnsToContents();
    ui->videoTable->setFocus();
    ui->videoTable->selectRow(0);
}

void SceneForm::addNewVideo() {
    QString filePath = QFileDialog::getOpenFileName(this,
        tr("Choose Video"), lastOpenDir, tr("Video Files (*.mp4 *.h264)"));
    if(filePath != "") {
        QFileInfo fi (filePath);
        lastOpenDir = fi.canonicalPath();
        if(!model->hasVideo(filePath)) {
            if(!checkVideo(filePath)) {
                showWarningDialog("Video format could be incompatible with the Yeti!");
            }
            try {
                model->addNewVideo(filePath);
            } catch(VSException &vse) {
                showWarningDialog(vse.getMessage());
            }
        } else if(model->hasVideo(filePath) && showQuestionDialog("Video already in scene, overwrite?")) {
            try {
                model->replaceVideo(filePath);
            } catch(VSException &vse) {
                showWarningDialog(vse.getMessage());
            }
        }
    }

    ui->videoTable->resizeColumnsToContents();
    ui->videoTable->selectRow(0);
}

/**
 * @brief SceneForm::checkVideo
 * @param path
 * @return true if mime type is ok, false otherwise
 */
bool SceneForm::checkVideo(QString path) {
    //check video format
    QMimeDatabase qmdb;
    QString mimeType = qmdb.mimeTypeForUrl(QUrl::fromLocalFile(path)).name();
    //qDebug() << "aliases are:" << qmdb.mimeTypeForUrl(QUrl::fromLocalFile(videoPath)).aliases();

    return (mimeType == "video/mp4");
}

void SceneForm::removeVideo() {
    try {
        model->removeSelectedVideo();
    } catch(VSException &vse) {
        showWarningDialog(vse.getMessage());
    }

//    //orrido fix ma non mi viene in mente altro!
//    if(ui->videoTable->selectionModel()->currentIndex().row() != model->selectedVideoIndex) {
//        model->selectVideo(ui->videoTable->selectionModel()->currentIndex().row());
//    }

    ui->videoTable->selectRow(0);

}

//slots
void SceneForm::doneSlot() {
    qDebug("doneSlot...");
    emit(transition("formfs"));
}

void SceneForm::addVideoSlot() {
    addNewVideo();
}

void SceneForm::removeVideoSlot() {
    if(showQuestionDialog("Delete selected video?")) {
        removeVideo();
    }
}

void SceneForm::updateSelection(const QItemSelection &selected, const QItemSelection &deselected) {
//void SceneForm::updateSelection(const QModelIndex &selected, const QModelIndex &deselected) {
    if(!selected.indexes().isEmpty()) {
        qDebug() << "Updating selection, row is " << selected.indexes().constFirst().row();
        model->selectVideo(selected.indexes().constFirst().row());
    } else {
        qWarning() << "Selection index empty!";
    }
}

void SceneForm::deleteVideoOkSlot() {
    qDebug() << "removevideo " << model->selectedVideoIndex;
    removeVideo();
}

void SceneForm::prerollSlot() {
    emit(transition("formpreroll"));
}
