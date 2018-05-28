#include "vsexception.h"
#include <QStringBuilder>

VSException::VSException(QString msg) {
    this->msg = msg;
}

VSException::VSException(QString msg, int code) {
    this->msg = msg;
    this->code = code;
}


void VSException::raise() const {
    throw *this;
}

VSException * VSException::clone() const {
    return new VSException(*this);
}

QString VSException::getMessage() {
    QString ret;
    if(code == -1)
        ret = this->msg;
    else
        ret = "[" % QString::number(this->code) % "] " % this->msg;

    return ret;
}
