#ifndef SCENEFORM_H
#define SCENEFORM_H

#include "abstractform.h"
#include <QString>
#include <QItemSelection>
#include "scenemodel.h"
#include "confirmdialog.h"
//#include <QMediaResource>
#include <QUrl>
#include <QMimeDatabase>

namespace Ui {
class SceneForm;
}

class SceneForm : public AbstractForm
{
    Q_OBJECT

public:
    explicit SceneForm(QWidget *parent);
    ~SceneForm();
    void update();

public slots:
    void doneSlot();
    void addVideoSlot();
    void removeVideoSlot();
    void updateSelection(const QItemSelection &selected, const QItemSelection &deselected);
    void deleteVideoOkSlot();
    void prerollSlot();

private:
    void setSceneName(QString sName);
    Ui::SceneForm *ui;
    QString sceneName;
    SceneModel * model = nullptr;
    void addNewVideo();
    void removeVideo();
    bool checkVideo(QString path);
};

#endif // SCENEFORM_H
