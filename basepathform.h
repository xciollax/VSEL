#ifndef BASEPATHFORM_H
#define BASEPATHFORM_H

#include <QWidget>
#include "abstractform.h"

namespace Ui {
class BasePathForm;
}

class BasePathForm : public AbstractForm
{
    Q_OBJECT

public:
    explicit BasePathForm(QWidget *parent = 0);
    ~BasePathForm();
    void update();
public slots:
    void okButtonSlot();
    void cancelButtonSlot();

private:
    Ui::BasePathForm *ui;
};

#endif // BASEPATHFORM_H
