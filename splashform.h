#ifndef SPLASHFORM_H
#define SPLASHFORM_H

#include "abstractform.h"
namespace Ui {
class SplashForm;
}

class SplashForm : public AbstractForm
{
    Q_OBJECT

public:
    explicit SplashForm(QWidget *parent = 0);
    ~SplashForm();
    void update();

private:
    Ui::SplashForm *ui;
};


#endif // SPLASHFORM_H
