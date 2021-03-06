#include "scene.h"
#include <QDir>
#include <QTextStream>
#include <iostream>
#include <qdebug.h>
#include "vsexception.h"
#include <QStringBuilder>

const QString Scene::VID_DATA_FILE = "video_data.txt";
const QString Scene::PREROLL_FILENAME = "preroll.jpg";

Scene::Scene() {}

Scene::Scene(QString path) {
    qDebug("Creating scene from path %s", path.toLocal8Bit().constData());
    this->scenePath = path;
    loadVideos();
    createId();
}

void Scene::loadVideos() {
    this->videos.clear();
    QDir sceneDir(this->scenePath);
    if(sceneDir.exists()) {
        //set name
        this->name = sceneDir.dirName();
        sceneDir.setFilter(QDir::Files | QDir::NoSymLinks);
        sceneDir.setSorting(QDir::Name);

        QFileInfoList list = sceneDir.entryInfoList();

        //load videos
        Video * tmpVideo;
        for (int i = 0; i < list.size(); ++i) {
            QFileInfo fileInfo = list.at(i);
            //exclude video data file and preroll image
            if(fileInfo.fileName() != Scene::VID_DATA_FILE && fileInfo.fileName() != Scene::PREROLL_FILENAME) {
                tmpVideo = new Video();
                tmpVideo->name = fileInfo.fileName();

                this->videos.append(*tmpVideo);
            }
        }

        //load additional video data
        QFile file(sceneDir.filePath(Scene::VID_DATA_FILE));

        //we support non existent video data files for scenes
        if(file.exists() && file.open(QIODevice::ReadOnly)) {
            QTextStream in(&file);
            QString line;

            bool found = false;

            while (!in.atEnd()) {
                found = false;
                line = in.readLine();
                for(int i = 0; i < this->videos.size(); i++) {
                    if(this->videos[i].fromLine(line)) {
                        found = true;
                        break;
                    }
                }

                if(!found && line != "") {
                    //we've found the id
                    qDebug() << "old id is " << id.toString();
                    id = QUuid::fromString(line);
                    qDebug() << "new id is " << id.toString();
                }
            }

            file.close();
        } else {
            qWarning("No additional video data, will use default values");
        }
    } else {
        throw VSException("non existent scene directory " + this->scenePath, 9);
    }

    verify();
}

void Scene::addVideo(Video v) {
    this->videos.append(v);
    createId();
}



void Scene::addPrerollImage(QString imgPath) {
    if(imgPath != "") {
        QFileInfo fi(imgPath);
        if(!fi.exists()) {
            throw new VSException("non existent image file, can't add to scene", 667);
        }

        removePrerollImage();

        if(!QFile::copy(imgPath, prerollImgPath())) {
            QFileInfo dfi(prerollImgPath());
            if(!dfi.exists()) {
                throw VSException("can't copy image file to scene directory", 667);
            }
        }
    }
}

QString Scene::prerollImgPath() const {
    return scenePath % QDir::separator() % PREROLL_FILENAME;
}

void Scene::removePrerollImage() {
    QDir sceneDir(scenePath);
    sceneDir.remove(PREROLL_FILENAME);
}

bool Scene::hasPrerollImage() const {
    QFileInfo fi(prerollImgPath());
    return fi.exists();
}

void Scene::addVideoFile(QString videoPath) {
    if(videoPath != "") {
        qDebug() << "Filename:" << videoPath;
        QFileInfo fi(videoPath);

        if(!fi.exists()) {
            throw VSException("non existent video file, can't add to scene", 666);
        }

        //add video file to scene
        QString fileName = fi.fileName();
        removeDuplicateVideo(fileName);

        QString destPath = scenePath % QDir::separator() % fileName;
        //copia il file nella directory della scena
        if(!QFile::copy(videoPath, destPath)) {
            //if scene doesn't contain video file already then we have a problem
            QFileInfo dfi(destPath);
            if(!dfi.exists()) {
                //throw exception
                throw VSException("can't copy video file to scene directory", 666);
            }
        } else {
            reload();
        }

        createId();
    }
}



void Scene::removeVideo(int i) {
    if(i >= 0 && i < videos.size()) {
        //remove video file
        QDir sceneDir(scenePath);
        sceneDir.remove(videos[i].name);

        //remove video object
        this->videos.removeAt(i);
        //sync video data
        saveVideoData();
        //verify: maybe note duplicate removed?
        verify();
        createId();
    }
}

void Scene::saveVideoData() {
    QDir sceneDir(scenePath);
    if(sceneDir.exists()) {
        QFile file(sceneDir.filePath(Scene::VID_DATA_FILE));
        if(file.exists() && file.open(QIODevice::ReadWrite)) {
            bool ok = file.remove();
            file.close();
            if(!ok) {
                throw VSException("can't remove", 10);
            }
        }

        if(file.open(QIODevice::ReadWrite)) {
            for(int i = 0 ; i < videos.size(); i++) {
                file.write(videos[i].toLine().append('\n').toLocal8Bit().constData());
            }

            const char nl = '\n';
            //write a blank line
            file.write(&nl);
            //wite id
            file.write(id.toByteArray());

            file.close();
        } else {
            throw VSException("can't write video files", 11);
        }
    } else {
        throw VSException("non existent scene dir, can't save videos", 12);
        qWarning() << "Non existent scene directory";
    }
}

void Scene::reload() {
    loadVideos();
}

void Scene::install(QString destPath) {
    qDebug() << "installing to " << destPath;
}

void Scene::verify() {
    //all video on non unique midi notes are not ok
    ok = true;
    for(int i = 0; i < videos.size(); i++) {
        videos[i].ok = true;
    }

    for(int i = 0; i < videos.size(); i++) {
        if(videos[i].ok) {
            for(int j = 0; j < videos.size(); j++) {
                if(i != j && videos[j].ok) {
                    if(videos[i].midiNote == videos[j].midiNote) {
                        videos[i].ok = false;
                        videos[j].ok = false;
                        ok = false;
                    }
                }
            }
        }
    }
}

bool Scene::hasVideo(QString n) {
    bool ret = false;
    for(Video v: videos) {
        if(v.name == n) {
            ret = true;
            break;
        }
    }

    return ret;
}

void Scene::removeDuplicateVideo(QString name) {
    int indx = indexOf(name);

    if(indx != -1) {
        removeVideo(indx);
    }
}
int Scene::indexOf(QString name) {
    int ret = -1;

    for(int i = 0; i < videos.size(); i++) {
        if(videos[i].name == name) {
            ret = i;
            break;
        }
    }

    return ret;
}

void Scene::createId() {
    id = QUuid::createUuid();
}

bool Scene::operator==(const Scene &other) const {
    return (name == other.name && id == other.id);
}

