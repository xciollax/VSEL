#include "abstractform.h"
#include <QMessageBox>
#include "vsexception.h"
#include <QDir>
AbstractForm::AbstractForm(QWidget * parent) : QWidget(parent) {
    lastOpenDir = QDir::homePath();
}

QMap<QString, QVariant> AbstractForm::sharedModel;

void AbstractForm::clean() {
    privateModel.clear();
}

//shared model
void AbstractForm::addToModel(QString key, QVariant value) {
    sharedModel.insert(key, value);
}

QVariant AbstractForm::getFromModel(QString key) {
    return sharedModel[key];
}

void AbstractForm::removeFromModel(QString key) {
    sharedModel.remove(key);
}

QString AbstractForm::getBasePath() {
    if(getFromModel("basePath").isValid()) {
        return getFromModel("basePath").toString();
    } else {
        return "";
    }
}

void AbstractForm::setBasePath(QString bPath) {
    addToModel("basePath", bPath);
}

void AbstractForm::setInstallationPath(QString thePath) {
    addToModel("instPath", thePath);
    emit installationPathChanged(thePath);
}

QString AbstractForm::getInstallationPath() {
    if(getFromModel("instPath").isValid()) {
        return getFromModel("instPath").toString();
    } else {
        return "";
    }
}

void AbstractForm::showMsgDialog(QString msg) {
    QMessageBox::information(this, tr(""), tr(msg.toLocal8Bit().constData()));
}

void AbstractForm::showWarningDialog(QString msg) {
    QMessageBox::warning(this, tr(""), tr(msg.toLocal8Bit().constData()));
}

bool AbstractForm::showQuestionDialog(QString msg) {
    QMessageBox::StandardButton choice = QMessageBox::question(this, "", msg);
    if(choice == QMessageBox::Yes) {
        return true;
    } else {
        return false;
    }
}
