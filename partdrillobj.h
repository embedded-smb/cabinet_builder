#pragma once
#include "partpoint.h"

class PartDrillObj
{
public:
    PartDrillObj(PartPoint* center, qreal diameter, qreal depth);

    PartPoint* center();
    qreal      diameter();
    qreal      depth();

private:
    PartPoint* _center;
    qreal      _diameter;
    qreal      _depth;

    friend QDebug operator<<(QDebug debug, PartDrillObj);
};
