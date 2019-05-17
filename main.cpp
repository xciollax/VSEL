#include "mainwindow.h"
#include <QApplication>
#include <QSettings>
#include <QSplashScreen>
#include <QTimer>
#include <qdebug.h>


int main(int argc, char *argv[])
{
    //TODO: fix organization data
    QCoreApplication::setOrganizationName("yodel");
    QCoreApplication::setOrganizationDomain("yodel.xxx");

    QCoreApplication::setApplicationName("Yeti librarian");

    //loads settings
    QSettings appSettings;
    QString scenesDir = "";

    if(appSettings.value("basePath").isValid()) {
        scenesDir = appSettings.value("basePath").toString();
    }

    QApplication a(argc, argv);

    QPixmap pMap(":/images/vsel.png");
    QSplashScreen *splash = new QSplashScreen(pMap, Qt::WindowStaysOnTopHint);
    splash->setPixmap(pMap);
    splash->showMessage("VSEL v. " VSEL_V, Qt::AlignRight);
    splash->show();
    QTimer::singleShot(2500, splash, SLOT(close())); // Timer

    MainWindow w(NULL, scenesDir);
    QTimer::singleShot(2700, &w, SLOT(show()));

    return a.exec();
}
