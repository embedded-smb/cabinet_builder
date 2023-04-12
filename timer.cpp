#include "timer.h"
#include <QDebug>

timer::timer()
    : _timer(new QElapsedTimer),
      _name("")
{
    _timer->start();
}

timer::timer(QString name)
    : _timer(new QElapsedTimer),
      _name(name)
{
    _timer->start();
}

timer::~timer()
{
    quint64 elapsed = _timer->elapsed();
    qDebug() << "Timer [" << _name << "] took " << elapsed << " ticks";
    delete _timer;
}
