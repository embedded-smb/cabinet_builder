#pragma once

#include "partpoint.h"
#include "partdrillobj.h"

#include <QList>
#include <QObject>
#include <QPointF>
#include <QRectF>
#include <QString>
#include "dxf_file.h"

// Proposed obj_structure
// part
// +- outline
// +- cutout
// +- dado
// +- drill
// +- text





class Part : public dxf_file
{

public:
    typedef enum {XAXIS, YAXIS} valid_axis;

    Part(QString name, Part* parent = nullptr);
    Part(Part obj, QString name);
    Part(Part& obj);
    ~Part();

//    Part& operator= (const Part& prt);

    void AddPoint(PartPoint* pt);
    void AddSubPart(Part* sub_part);
    void AddDado(QString name, PartPoint* pt, qreal length, qreal width, qreal depth);
    void AddDado(QString name, PartPoint  pt, qreal length, qreal width, qreal depth);
    void AddDrill(PartPoint* pt, qreal diameter, qreal depth);


    QRectF GetExtents();

    void Mirror(Part* dest, valid_axis axis);
    Part* Transform(PartPoint offset);
    void SaveLayer(QString layer_name);

signals:

private:
    Part*               _parent;
    QString             _name;
    QList<Part*>        _sub_part_list;
    QList<PartPoint*>   _point_list;
    QList<PartDrillObj> _drill_list;

    friend QDebug operator<<(QDebug debug, Part);
    friend class Sheet;
};

