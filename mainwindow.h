#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QMap>
#include "abstractform.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, QString basePath = "");
    ~MainWindow();
    void showForm(QString name);
    void switchWidget();

public slots:
    void transitionSlot(QString t);

private:
    Ui::MainWindow *ui;
    QWidget *currentForm;
    QMap<QString, AbstractForm *> forms;
    AbstractForm *formByName(QString name);
};

#endif // MAINWINDOW_H
