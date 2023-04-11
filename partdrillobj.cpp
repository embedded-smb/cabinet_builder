#include "partdrillobj.h"
#include <QDebug>

QDebug operator<<(QDebug debug, PartDrillObj val)
{
    QDebugStateSaver saver(debug);

    qDebug() << Qt::fixed << qSetRealNumberPrecision(3);
    debug.nospace() << "DillObj is [" << val._center << "  Diameter: " << val._diameter << "  Depth: " << val._depth << "]";
    return debug;
}

PartDrillObj::PartDrillObj(PartPoint* center, qreal diameter, qreal depth)
    : _center(center),
      _diameter(diameter),
      _depth(depth)
{}

PartPoint* PartDrillObj::center()
{
    return _center;
}

qreal PartDrillObj::diameter()
{
    return _diameter;
}

qreal PartDrillObj::depth()
{
    return _depth;
}
