#pragma once

#include <QObject>
#include "part.h"
#include "partpoint.h"

class PartRect : public QObject
{
    Q_OBJECT
public:
    explicit PartRect(QObject *parent = nullptr);
    PartRect(PartRect& rect);
    PartRect(Part part);

    PartPoint Corner1(Part part);
    PartPoint Corner2(Part part);


signals:


};
