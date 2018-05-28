#ifndef FORMFILESYSTEMLIST_H
#define FORMFILESYSTEMLIST_H

#include "abstractform.h"
#include "createscenedialog.h"
#include "confirmdialog.h"
#include <QFileSystemModel>
#include <QItemSelection>
#include "scene.h"

namespace Ui {
class FormFileSystemList;
}

class ScenesListForm : public AbstractForm
{
    Q_OBJECT

public:
    explicit ScenesListForm(QWidget *parent = 0);
    ~ScenesListForm();
    void update();
    void vselExit();

public slots:
    void plusSceneSlot();
    void minusSceneSlot();
    void editSceneSlot();
    void plusOkSlot(QString name);
    void updateSelectionSlot(const QItemSelection &selected, const QItemSelection &deselected);
    void installSceneSlot();
    void uninstallSceneSlot();
    void createInstallationSlot();
    void selectInstallationSlot();
    void installationChangedSlot(QString instPath);
    void currentSceneChangedSlot(QString scene);
    void changeBasePathSlot();
    void vselExitSlot();

signals:
    void currentSceneChanged(QString scene);

private:
    Ui::FormFileSystemList *ui;
    QFileSystemModel *mod;
    CreateSceneDialog * csd;
    ConfirmDialog * confd;
    Scene * loadedScene;
    void setupModel();
    void createScene(const QString sceneName);
    void deleteScene(const QString sceneName);
    void uninstallScene();
    void createInstallation();
    void selectInstallation();
    void updateButtons();
    Scene * loadScene(const QString sceneName);
    QDir * baseDir;
    bool validateSceneName(QString sceneName);
    QString currentScene;
    void setInstallationText(QString text);
};

#endif // FORMFILESYSTEMLIST_H
