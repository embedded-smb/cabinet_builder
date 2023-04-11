#include "partpoint.h"
#include <QDebug>

QDebug operator<<(QDebug debug, PartPoint val)
{
    QDebugStateSaver saver(debug);

    qDebug() << Qt::fixed << qSetRealNumberPrecision(3);
    debug.nospace() << val._label << " is (" << val._x << ", " << val._y << ", " << val._z << ")";
    return debug;
}

PartPoint::PartPoint()
    : _x(0.0),
      _y(0.0),
      _z(0.0),
      _label("")
{}

PartPoint::PartPoint(qreal x, qreal y, qreal z, QString label)
    : _x(x),
      _y(y),
      _z(z),
      _label(label)
{}

PartPoint::PartPoint(QPointF& pt, QString label)
    : _label(label)
{
    _x = pt.rx();
    _y = pt.ry();
    _z = 0.0;
}

PartPoint::PartPoint(PartPoint& pt, QString label)
    : _x(pt._x),
      _y(pt._y),
      _z(pt._z),
      _label(label)
{}

PartPoint::PartPoint(PartPoint& pt)
    : _x(pt._x),
      _y(pt._y),
      _z(pt._z),
      _label(pt._label)
{}

PartPoint::~PartPoint()
{}

PartPoint PartPoint::operator+ (PartPoint pt)
{
    PartPoint ret_pt(_x + pt._x, _y + pt._y, _z + pt._z, _label);

    return ret_pt;
}

PartPoint PartPoint::operator+= (PartPoint pt)
{
    _x += pt._x;
    _y += pt._y;
    _z += pt._z;

    return *this;
}

PartPoint PartPoint::operator-(PartPoint pt)
{
    PartPoint ret_pt(_x - pt._x, _y - pt._y, _z - pt._z, _label);

    return ret_pt;
}

PartPoint PartPoint::operator -=(PartPoint pt)
{
    _x -= pt._x;
    _y -= pt._y;
    _z -= pt._z;

    return *this;
}

PartPoint &PartPoint::operator= (const PartPoint &pt)
{
    _x = pt._x;
    _y = pt._y;
    _z = pt._z;
    _label = pt._label;

    return *this;
}

bool PartPoint::operator== (const PartPoint &pt)
{
    return ((_x == pt._x) &&
            (_y == pt._y) &&
            (_z == pt._z));
}

qreal PartPoint::X()
{
    return _x;
}

qreal PartPoint::Y()
{
    return _y;
}

qreal PartPoint::Z()
{
    return _z;
}

QString PartPoint::Label()
{
    return _label;
}

qreal PartPoint::DistTo(PartPoint& pt)
{
    Q_UNUSED(pt)
    return 0.0;
}

quint64 PartPoint::ConvertToInt(qreal val)
{
    return quint64((val + 0.0005) * 1000.0);
}

qreal PartPoint::ConvertToReal(quint64 val)
{
    return (qreal(val) / 1000.0);
}
