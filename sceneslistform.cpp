#include "sceneslistform.h"
#include "ui_formfilesystemlist.h"
#include <QItemSelectionModel>
#include <QModelIndex>
#include <QModelIndexList>
#include "confirmdialog.h"
#include "installationhelper.h"
#include <QFileInfoList>
#include "video.h"
#include <QFileDialog>
#include <QStringBuilder>

ScenesListForm::ScenesListForm(QWidget *parent) :
    AbstractForm(parent),
    ui(new Ui::FormFileSystemList) {
    ui->setupUi(this);

    //create dialogs
    csd = new CreateSceneDialog();
    confd = new ConfirmDialog(NULL, "");

    //wiring local buttons
    connect(ui->buttonPlus, SIGNAL(clicked(bool)), this, SLOT(plusSceneSlot()));
    connect(ui->buttonMinus, SIGNAL(clicked(bool)), this, SLOT(minusSceneSlot()));
    connect(ui->buttonEdit, SIGNAL(clicked(bool)), this, SLOT(editSceneSlot()));
    connect(ui->installButton, SIGNAL(clicked(bool)), this, SLOT(installSceneSlot()));
    connect(ui->uninstallButton, SIGNAL(clicked(bool)), this, SLOT(uninstallSceneSlot()));
    connect(ui->createInstButton, SIGNAL(clicked(bool)), this, SLOT(createInstallationSlot()));
    connect(ui->selectInstButton, SIGNAL(clicked(bool)), this, SLOT(selectInstallationSlot()));
    connect(ui->exitButton, SIGNAL(clicked(bool)), this, SLOT(vselExitSlot()));
    connect(ui->changeBaseButton, SIGNAL(clicked(bool)), this, SLOT(changeBasePathSlot()));

    //wiring local events
    connect(this, SIGNAL(installationPathChanged(QString)), this, SLOT(installationChangedSlot(QString)));
    connect(this, SIGNAL(currentSceneChanged(QString)), this, SLOT(currentSceneChangedSlot(QString)));

    //wiring create scene dialog buttons
    connect(csd, SIGNAL(ok(QString)), this, SLOT(plusOkSlot(QString)));

    updateButtons();
}

ScenesListForm::~ScenesListForm() {
    delete ui;
}

void ScenesListForm::setupModel() {
    mod = new QFileSystemModel;
    mod->setRootPath(getBasePath());

    baseDir = new QDir(getBasePath());

    ui->listView->setModel(mod);
    ui->listView->setRootIndex(mod->index(getBasePath()));

    //wiring selection events
    connect(ui->listView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(updateSelectionSlot(QItemSelection,QItemSelection)) );
}

void ScenesListForm::update() {
    if(mod == NULL) {
        setupModel();
    } else if(mod->rootPath() != getBasePath()) {
        ui->listView->setRootIndex(mod->index(getBasePath()));
    }

    ui->listView->setFocus();
}

Scene * ScenesListForm::loadScene(const QString sceneName) {
    qDebug("loading scene %s", sceneName.toLocal8Bit().constData());
    Scene * ret = new Scene(getBasePath() + QDir::separator() + sceneName);
    QVariant var;
    var.setValue(ret);

    sharedModel.remove("currScene");

    sharedModel.insert("currScene", var);
    return ret;
}

void ScenesListForm::updateButtons() {
    bool sceneSelected = (currentScene != "");
    bool installationSelected = getInstallationPath() != "";

    ui->buttonMinus->setEnabled(sceneSelected);
    ui->buttonEdit->setEnabled(sceneSelected);
    ui->installButton->setEnabled(sceneSelected);

    ui->installButton->setEnabled(sceneSelected && installationSelected);
    ui->uninstallButton->setEnabled(sceneSelected && installationSelected);
    ui->syncButton->setEnabled(installationSelected);
}

//biz func
void ScenesListForm::createScene(QString sceneName) {
    if(!validateSceneName(sceneName)) {
        throw VSException("Invalid Scene Name, only 4 letter words allowed :)", 13);
    } else {
        baseDir->mkdir(sceneName);
    }
}

void ScenesListForm::deleteScene(QString sceneName) {
    if(baseDir->exists(sceneName)) {
        qDebug("About to delete %s", sceneName.toLocal8Bit().constData());
        QDir temp = QDir(baseDir->absolutePath().append(QDir::separator()).append(sceneName));
        if(!temp.removeRecursively()) {
            throw VSException("cannot remove scene folder", 14);
        }
    } else {
        throw VSException("non existent scene folder", 15);
    }
}

/**
 * @brief ScenesListForm::validateSceneName Scene names must be 4 charachters long, contain only letters, digits, -, _, #
 * @param sceneName
 * @return
 */
bool ScenesListForm::validateSceneName(QString sceneName) {
    QRegularExpression re("^[A-Za-z0-9_\\+\\-\\#]{4}$");
    return re.match(sceneName).hasMatch();
}

void ScenesListForm::createInstallation() {
    QString instPath = QFileDialog::getExistingDirectory(this, tr("Choose Installation location"), QDir::homePath(), QFileDialog::ShowDirsOnly);
    if(instPath != "") {
        try {
            QString in = InstallationHelper::createInstallation(instPath);
            if(in != "") {
                setInstallationPath(in);
            } else {
                throw VSException("Can't create installation!", 16);
            }
        } catch(VSException &vse) {
            showWarningDialog(vse.getMessage());
        }
    } else {}
}

void ScenesListForm::selectInstallation() {
    QString instPath = QFileDialog::getExistingDirectory(this, tr("Choose Existing Installation"), QDir::homePath(), QFileDialog::ShowDirsOnly);

    if(instPath != "") {
        try {
            QString in = InstallationHelper::selectInstallation(instPath);
            if(in != "") {
                setInstallationPath(in);
            } else {
                showWarningDialog("Can't select installation!");
            }
        } catch(VSException &vse) {
            showWarningDialog(vse.getMessage());
        }
    } else {}
}

void ScenesListForm::setInstallationText(QString text) {
    if(text == "") {
        text = "None";
    }
    ui->currentInstLabel->setText(text);
}

void ScenesListForm::uninstallScene() {
    if(showQuestionDialog("Uninstall scene " % currentScene % "?")) {
        if(getInstallationPath() != "") {
            try {
                loadScene(currentScene);
                QVariant tmpVal = sharedModel.value("currScene");
                if(tmpVal.isValid()) {
                    InstallationHelper::uninstallScene(tmpVal.value<Scene*>(), getInstallationPath());
                }
            } catch(VSException &vse) {
                showWarningDialog(vse.getMessage());
            }
        } else {
            showWarningDialog("Please create or select an installation");
        }
    }
}

void ScenesListForm::vselExit() {
    QCoreApplication::quit();
}

//slots

void ScenesListForm::changeBasePathSlot() {
    emit transition("formbp");
}
void ScenesListForm::vselExitSlot() {
    if(showQuestionDialog("Really exit?")) {
        vselExit();
    }
}
void ScenesListForm::plusSceneSlot() {
    csd->reset();
    csd->show();
}

void ScenesListForm::minusSceneSlot() {
    if(showQuestionDialog("Delete " + currentScene + "?")) {
        qDebug("Deleting %s", currentScene.toLocal8Bit().constData());
        try {
            deleteScene(currentScene);
        } catch(VSException &vse) {
            showWarningDialog(vse.getMessage());
        }
    }
}

void ScenesListForm::editSceneSlot() {
    qDebug("edit scene...");
    try {
        loadScene(currentScene);
        emit transition("formscene");
    } catch(VSException &vse) {
        showWarningDialog(vse.getMessage());
    }
}

void ScenesListForm::installSceneSlot() {
    if(getInstallationPath() != "") {
        try {
            loadScene(currentScene);
            //comanda la esportazione della scena
            QVariant tmpVal = sharedModel.value("currScene");
            if(tmpVal.isValid()) {
                InstallationHelper::installScene(tmpVal.value<Scene*>(),getInstallationPath(), getBasePath());
            } else {
                throw VSException("invalid scene", 17);
            }
        } catch(VSException &vse) {
            showWarningDialog(vse.getMessage());
        }
    } else {
        showWarningDialog("Please create or select an installation");
    }
}

void ScenesListForm::uninstallSceneSlot() {
    uninstallScene();
}

void ScenesListForm::plusOkSlot(QString name) {
    qDebug("Adding %s", name.toLocal8Bit().constData());
    try {
        createScene(name);
    } catch(VSException &vse) {
        showWarningDialog(vse.getMessage());
    }
}


void ScenesListForm::updateSelectionSlot(const QItemSelection &selected, const QItemSelection &deselected) {
    QModelIndex index;
    QModelIndexList items = selected.indexes();

    foreach (index, items) {
        //QString text = QString("(%1,%2)").arg(index.row()).arg(index.column());
        currentScene = mod->data(index, Qt::DisplayRole).toString();
        emit currentSceneChanged(currentScene);
        qDebug(currentScene.toLocal8Bit().constData());
    }
}

void ScenesListForm::createInstallationSlot() {
    createInstallation();
}

void ScenesListForm::selectInstallationSlot() {
    selectInstallation();
}

void ScenesListForm::installationChangedSlot(QString instPath) {
    setInstallationText(instPath);
    updateButtons();
}

void ScenesListForm::currentSceneChangedSlot(QString scene) {
    updateButtons();
}
