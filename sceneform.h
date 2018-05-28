#ifndef SCENEFORM_H
#define SCENEFORM_H

#include "abstractform.h"
#include <QString>
#include <QItemSelection>
#include "scenemodel.h"
#include "confirmdialog.h"

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

private:
    void setSceneName(QString sName);
    Ui::SceneForm *ui;
    QString sceneName;
    SceneModel * model = nullptr;
    void addNewVideo();
    void removeVideo();
    ConfirmDialog * confd;
};

#endif // SCENEFORM_H
