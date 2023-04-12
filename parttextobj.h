#pragma once
#include "partpoint.h"

#include <QString>


class PartTextObj
{
public:
    PartTextObj();
    PartTextObj(PartPoint* origin, QString text, qreal size = 1.0);

    PartPoint* point();
    QString    text();
    qreal      size();

private:
    PartPoint* _origin;
    QString    _text;
    qreal      _size;

    friend QDebug operator<<(QDebug debug, PartTextObj);
};
