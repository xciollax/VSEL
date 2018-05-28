#ifndef VIDEO_H
#define VIDEO_H

#include "QString"

class Video
{
public:
    Video();
    Video(QString n, bool lat, int bt, int mn);
    bool fromLine(QString line);
    QString toLine();
    QString name;
    bool loopAtEnd;
    int blurType;
    int midiNote;
    static QString blurTypes[];
    bool ok = true;
};

#endif // VIDEO_H
