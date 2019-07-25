#ifndef VIDEO_H
#define VIDEO_H

#include "QString"

class Video
{
public:
    Video();
    Video(QString n, bool lat, int bt, int mn, int zt);
    bool fromLine(QString line);
    QString toLine();
    QString name;
    bool loopAtEnd;
    int blurType;
    int midiNote;
    int zoomType;
    bool preload;
    static QString blurTypes[];
    static QString zoomTypes[];
    bool ok = true;
};

#endif // VIDEO_H
