#pragma once

#include <QObject>
#include <QPointF>

class PartPoint
{

public:
    PartPoint();
    PartPoint(qreal x, qreal y, qreal z, QString label = "");
    PartPoint(QPointF& pt, QString label = "");
    PartPoint(PartPoint& pt, QString label);
    PartPoint(PartPoint& pt);
    ~PartPoint();

    PartPoint  operator+  (PartPoint pt);
    PartPoint  operator+= (PartPoint pt);
    PartPoint  operator-  (PartPoint pt);
    PartPoint  operator-= (PartPoint pt);
    PartPoint& operator=  (const PartPoint &pt);
    bool       operator== (const PartPoint &pt);


    qreal X();
    qreal Y();
    qreal Z();
    QString Label();
    qreal DistTo(PartPoint& pt);

private:
    quint64 ConvertToInt(qreal val);
    qreal   ConvertToReal(quint64 val);

    qreal _x;
    qreal _y;
    qreal _z;
    QString _label;

    friend QDebug operator<<(QDebug debug, PartPoint);
};
