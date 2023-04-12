#pragma once
#include <QElapsedTimer>
#include <QString>

class timer
{
public:
    timer();
    timer(QString name);
    ~timer();

private:
    QElapsedTimer* _timer;
    QString        _name;
};
