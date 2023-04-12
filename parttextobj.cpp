#include "parttextobj.h"
#include <QDebug>

QDebug operator<<(QDebug debug, PartTextObj val)
{
    QDebugStateSaver saver(debug);

    qDebug() << Qt::fixed << qSetRealNumberPrecision(3);
    debug.nospace() << "Text Label: " << val._text << " at (" << val._origin << ") of size " << val._size;
    return debug;
}

PartTextObj::PartTextObj()
{

}

PartTextObj::PartTextObj(PartPoint* origin, QString text, qreal size)
    : _origin(origin),
      _text(text),
      _size(size)
{}

PartPoint* PartTextObj::point()
{
    return _origin;
}

QString PartTextObj::text()
{
    return _text;
}

qreal PartTextObj::size()
{
    return _size;
}
