#include "video.h"
#include <QStringList>

Video::Video() {
    this->name = "";
    this->loopAtEnd = false;
    this->blurType = 0;
    this->midiNote = 0;
}

Video::Video(QString n, bool lat, int bt, int mn) {
    this->name = n;
    this->loopAtEnd = lat;
    this->blurType = bt;
    this->midiNote = mn;
}

QString Video::toLine() {
    char sep = '\t';
    return name + sep + QString::number(midiNote) + sep + QString::number(loopAtEnd) + sep + QString::number(blurType);
}

bool Video::fromLine(QString line) {
    bool ret = false;
    QStringList al = line.split('\t');

    //if video has a name already it must be equal to the first token
    //if video doesn't have a name the line's contents are always loaded
    if(al.size() == 4 && (this->name == "" || this->name == al.value(0))) {
        ret = true;
        this->name = al.value(0);
        this->midiNote = al.value(1).toInt();
        this->loopAtEnd = al.value(2).toInt();
        this->blurType = al.value(3).toInt();
    }

    return ret;
}

QString Video::blurTypes[] = {"Rewind", "Pause", "Virtual play"};
