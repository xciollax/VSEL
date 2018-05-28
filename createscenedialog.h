#ifndef CREATESCENEDIALOG_H
#define CREATESCENEDIALOG_H

#include <QDialog>
#include <QString>
namespace Ui {
class CreateSceneDialog;
}

class CreateSceneDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateSceneDialog(QWidget *parent = 0);
    ~CreateSceneDialog();
private slots:
    void okSlot();
signals:
    void ok(QString sceneName);
private:
    Ui::CreateSceneDialog *ui;
};

#endif // CREATESCENEDIALOG_H
