#pragma once

#include "partpoint.h"
#include "partdrillobj.h"
#include "parttextobj.h"

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

    Part();
    Part(QString name);
    Part(Part obj, QString name);
    Part(Part& obj);
    ~Part();

//    Part& operator= (const Part& prt);

    void AddPoint(PartPoint* pt);
    void AddSubPart(Part* sub_part);
    void AddDado(QString name, PartPoint* pt, qreal length, qreal width, qreal depth);
    void AddDrill(PartPoint* pt, qreal diameter, qreal depth);
    void AddText(PartPoint* pt, QString text, qreal size);

    QRectF GetExtents();

    void  Mirror(Part* dest, valid_axis axis);
    Part* Mirror(valid_axis axis, QString name);
    Part* Transform(PartPoint offset);
//    void SaveLayer(QString layer_name);

signals:

private:
    QString               _name;
    QList<Part*>          _sub_part_list;
    QList<PartPoint*>     _point_list;
    QList<PartDrillObj*>  _drill_list;
    QList<PartTextObj*>   _text_list;

    friend QDebug operator<<(QDebug debug, Part);
    friend class Sheet;
};

