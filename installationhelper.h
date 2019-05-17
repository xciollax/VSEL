#ifndef INSTALLATIONHELPER_H
#define INSTALLATIONHELPER_H
#include "scene.h"
#include <QVector>
#include <QString>
#include <QDir>

class InstallationHelper {
public:
    const static QString instDirName;
    const static QString scenesDirName;

    InstallationHelper();
    static void installScene(const Scene * s, const QString path, const QString basePath);
    static void uninstallScene(const Scene * s, const QString path);
    static QString createFileName(const Video v);
    static bool checkInstDir(QDir instDir);
    static QString createInstallation(QString instPath);
    static QString selectInstallation(QString instPath);
    static void cleanInstallation(QString instPath);
private:
    static void removeObsoleteVideos(const Scene * s, QDir sceneDir);
    static bool checkInstLocation(QString locPath);
    static bool checkInstallation(QString instPath);
};

#endif // INSTALLATIONHELPER_H
