#ifndef PREROLLIMGFORM_H
#define PREROLLIMGFORM_H
#include "abstractform.h"
#include "scene.h"

#include <QDialog>
#include <QDir>
#include <QString>
#include <QWidget>

namespace Ui {
class PrerollImgForm;
}

class PrerollImgForm : public AbstractForm
{
    Q_OBJECT

public:
    explicit PrerollImgForm(QWidget *parent = 0);
    ~PrerollImgForm();
    void update();
    void showEvent(QShowEvent *event);

public slots:
    void doneSlot();
    void removeSlot();
    //void removeOkSlot();
    void addChangeSlot();


private:
    Ui::PrerollImgForm *ui;
    void addPrerollImage();
    void removePrerollImage();
    Scene * getCurrentScene();
    void displayPrerollImage();
};

#endif // PREROLLIMGFORM_H
