#ifndef VSEXCEPTION_H
#define VSEXCEPTION_H

#include <QException>

class VSException : public QException {
public:
    VSException(QString msg);
    VSException(QString msg, int code);
    VSException * clone() const;
    QString getMessage();
    void raise() const;
private:
    QString msg;
    int code = -1;
};

#endif // VSEXCEPTION_H

