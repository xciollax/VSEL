#ifndef ABSTRACTFORM_H
#define ABSTRACTFORM_H

#include <QWidget>
#include <QMap>
#include <QVariant>
#include <QString>
#include "vsexception.h"

class AbstractForm : public QWidget {
    Q_OBJECT
    public:
        AbstractForm(QWidget * parent);
        virtual void update() = 0;
        virtual void clean();
        QString getBasePath();
        void setBasePath(QString bPath);
        QString getInstallationPath();
        void setInstallationPath(QString thePath);
        static QMap<QString, QVariant> sharedModel;
        static void addToModel(QString key, QVariant value);
        static QVariant getFromModel(QString key);
        static void removeFromModel(QString key);
        void showMsgDialog(QString msg);
        void showWarningDialog(QString msg);
        bool showQuestionDialog(QString msg);
        QString lastOpenDir;
    signals:
        void transition(QString t);
        void installationPathChanged(QString instPath);

    private:
        QMap<QString, QVariant> privateModel;
};

#endif // ABSTRACTFORM_H
