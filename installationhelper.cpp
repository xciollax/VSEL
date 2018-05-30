#include "installationhelper.h"
#include <QDir>
#include <QStringBuilder>
#include <QStringList>
#include <QFileDialog>
#include "vsexception.h"
#include <qdebug.h>

InstallationHelper::InstallationHelper() {}

void InstallationHelper::installScene(const Scene *s, const QString installationPath, const QString basePath) {
    QDir instDir(installationPath);
    QDir baseDir(basePath % QDir::separator() % s->name);

    if(checkInstDir(instDir)) {
        QDir scenesDir(instDir.absolutePath() % QDir::separator() % "scenes");
        if(!scenesDir.exists(s->name)) {
            //create scene dir
            if(!scenesDir.mkdir(s->name)) {
                throw VSException("can't create scene folder", 0);
            }
        }

        QDir sceneDir(scenesDir.absolutePath() % QDir::separator() % s->name);
        removeObsoleteVideos(s, sceneDir);

        QString videoFileName;
        for(Video v : s->videos) {
            videoFileName = createFileName(v);
            if(sceneDir.exists(videoFileName)) {
                QFileInfo older(sceneDir, videoFileName);
                QFileInfo newer(baseDir, v.name);
                if(older.size() != newer.size()) {
                    //older file dies
                    (new QFile(older.absoluteFilePath()))->remove();
                } else {
                    //same video no need to copy, older lives
                    continue;
                }
            }

            if(!QFile::copy(baseDir.absolutePath() % QDir::separator() % v.name, sceneDir.absolutePath() % QDir::separator() + videoFileName)) {
                throw VSException("can't copy video to installation", 1);
            }
        }
    } else {
       throw VSException("illegal installation directory tree", 2);
    }
}

void InstallationHelper::uninstallScene(const Scene *s, const QString path) {
    QDir sceneDir(path % QDir::separator() % "scenes" % s->name);
    if(sceneDir.exists()) {
        sceneDir.removeRecursively();
    } else {
        throw VSException("scene not installed, can't remove", 3);
    }
}

//note - blur type - loop at end . ext
QString InstallationHelper::createFileName(const Video v) {
    QString midinote, blurType, loopatEnd;

    QChar sep = '-';
    QChar dot = '.';

    return midinote.setNum(v.midiNote) % sep % blurType.setNum(v.blurType) % sep % loopatEnd.setNum(v.loopAtEnd) % dot % v.name.split(dot).last();
}

/**
 * @brief InstallationHelper::checkInstDir
 * @param instDir
 * @return true if installation dir passes check, false otherwise
 */
bool InstallationHelper::checkInstDir(QDir instDir) {
    return instDir.exists("scenes");
}

/**
 * @brief InstallationHelper::removeObsoleteVideos Remove videos which are not part of the scene anymore.
 * @param s
 * @param sceneDir
 */
void InstallationHelper::removeObsoleteVideos(const Scene *s, QDir sceneDir) {
    QStringList vdz;

    for(Video v : s->videos) {
        vdz << createFileName(v);
    }

    QStringList scVdz = sceneDir.entryList(QDir::Files);

    for(QString n : scVdz) {
        qDebug() << "checking video " << n;
        if(!vdz.contains(n)) {
            //kill obsolete video file
            sceneDir.remove(n);
        }
    }
}

QString InstallationHelper::createInstallation(QString instPath) {
    QString ret("");

    if(instPath != "") {
        //verify chosen path
        if(checkInstLocation(instPath)) {
            QDir instDir(instPath);
            if(instDir.mkpath("video_slave/scenes")) {
                ret = instPath % QDir::separator() % "video_slave";
            } else {
                throw VSException("can't create scenes directory, aborting", 4);
            }
        } else {
            throw VSException("illegal installation dir, can't create", 5);
        }
    } else {
        throw VSException("void installation path, can't create", 6);
    }

    return ret;
}

bool InstallationHelper::checkInstLocation(QString locPath) {
    bool ret = true;
    qDebug() << "Checking installation location " << locPath;
    QDir toCheck(locPath);
    qDebug() << toCheck.exists() << (toCheck.dirName() != "video_slave") << !toCheck.exists("video_slave");
    ret = (toCheck.exists() && toCheck.dirName() != "video_slave" && !toCheck.exists("video_slave"));

    return ret;
}

bool InstallationHelper::checkInstallation(QString instPath) {
    bool ret = true;
    QDir toCheck(instPath);
    ret = (toCheck.exists() && toCheck.dirName() == "video_slave" && toCheck.exists("scenes"));
    return ret;
}

QString InstallationHelper::selectInstallation(QString instPath) {
    QString ret("");

    if(instPath != "") {
        //verify chosen path
        if(checkInstallation(instPath)) {
            ret = instPath;
        } else {
            throw VSException("chosen path is not an installation, can't select", 7);
        }
    } else {
        throw VSException("void installation path, can't select", 8);
    }

    return ret;
}

