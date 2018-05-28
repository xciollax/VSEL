#ifndef SCENE_H
#define SCENE_H

#include "QString"
#include "QVector"
#include "video.h"
#include "qvariant.h"
class Scene {
public:
    QString name;
    QVector<Video> videos;
    QString scenePath;

    Scene();
    Scene(QString path);

    void addVideo(Video v);
    void removeVideo(int i);
    void saveVideoData();
    void reload();
    void install(QString destPath);
    void verify();
    bool ok = true;
private:
    static const QString VID_DATA_FILE;
    void loadVideos();
};

Q_DECLARE_METATYPE(Scene*)
#endif // SCENE_H
