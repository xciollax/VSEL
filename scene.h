#ifndef SCENE_H
#define SCENE_H

#include <QString>
#include <QVector>
#include "video.h"
#include <qvariant.h>
#include <QUuid>

class Scene {
public:
    QString name;
    QVector<Video> videos;
    QString scenePath;
    QUuid id;
    Scene();
    Scene(QString path);

    bool operator==(const Scene &other) const;

    void addVideo(Video v);
    void addVideoFile(QString videoPath);
    void removeVideo(int i);
    void saveVideoData();
    void reload();
    void install(QString destPath);
    void verify();
    bool ok = true;
    bool hasVideo(QString name);
    int indexOf(QString name);
private:
    void createId();
    static const QString VID_DATA_FILE;
    void loadVideos();
    void removeDuplicateVideo(QString name);
};

Q_DECLARE_METATYPE(Scene*)
#endif // SCENE_H
