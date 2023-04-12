#include <QDebug>
#include "part.h"

#define MIRROR_OFFSET 1.0
QDebug operator<<(QDebug debug, Part val)
{
    QDebugStateSaver saver(debug);

    qDebug() << Qt::fixed << qSetRealNumberPrecision(3);
    debug.nospace() << val._name << " is\n\tPartPoints[";
    for (QList<PartPoint*>::Iterator itr=val._point_list.begin();itr!=val._point_list.end();itr++)
    {
        PartPoint* pt = *itr;
        debug.nospace() << *pt << " ";
    }
    debug.nospace() << "]\n\tSubParts[";
    for (QList<Part*>::Iterator itr=val._sub_part_list.begin();itr!=val._sub_part_list.end();itr++)
    {
        Part* part = *itr;
        debug.nospace() << *part << "  ";
    }
    debug.nospace() << "]\n\tDrill Holes [";
    for (QList<PartDrillObj*>::Iterator itr=val._drill_list.begin();itr!=val._drill_list.end();itr++)
    {
        PartDrillObj* drill = *itr;
        debug.nospace() << *drill << "  ";
    }
    debug.nospace() << "]\n\tText [";
    for (QList<PartTextObj*>::Iterator itr=val._text_list.begin();itr!=val._text_list.end();itr++)
    {
        PartTextObj* txt_obj = *itr;
        debug.nospace() << *txt_obj << "  ";
    }
    debug.nospace() << "]\n";

    return debug;
}

Part::Part()
    : _name("")
{}

Part::Part(QString name)
    : _name(name)
{}

Part::Part(Part obj, QString name)
    : _name(name)
{
    this->_point_list    = obj._point_list;
    this->_sub_part_list = obj._sub_part_list;
    this->_drill_list    = obj._drill_list;
    this->_text_list     = obj._text_list;
}

Part::Part(Part& obj)
    : _name(obj._name),
      _sub_part_list(obj._sub_part_list),
      _point_list(obj._point_list),
      _drill_list(obj._drill_list),
      _text_list(obj._text_list)
{}

Part::~Part()
{}

void Part::AddPoint(PartPoint* pt)
{
    _point_list.append(pt);
}

void Part::AddSubPart(Part* sub_part)
{
    _sub_part_list.append(sub_part);
}

void Part::AddDado(QString name, PartPoint* pt, qreal length, qreal width, qreal depth)
{
    Q_UNUSED(depth)

    Part*      dado = new Part(name);
    PartPoint* point(pt);

    dado->AddPoint(point);

    point = new PartPoint(*pt + PartPoint(length, 0, 0));
    dado->AddPoint(point);

    point = new PartPoint(*pt + PartPoint(length, width, 0));
    dado->AddPoint(point);

    point = new PartPoint(*pt + PartPoint(0, width, 0));
    dado->AddPoint(point);

    AddSubPart(dado);
}

void Part::AddDrill(PartPoint* pt, qreal diameter, qreal depth)
{
    PartDrillObj* drill = new PartDrillObj(pt, diameter, depth);

    _drill_list.append(drill);
}

void Part::AddText(PartPoint* pt, QString text, qreal size)
{
    PartTextObj* text_part = new PartTextObj(pt, text, size);

    _text_list.append(text_part);
}

QRectF Part::GetExtents()
{
    QRectF temp;

    temp.setTop(std::numeric_limits<qreal>::min());
    temp.setBottom(std::numeric_limits<qreal>::max());
    temp.setLeft(std::numeric_limits<qreal>::max());
    temp.setRight(std::numeric_limits<qreal>::min());

    for (QList<PartPoint*>::iterator itr=_point_list.begin(); itr!=_point_list.end(); itr++)
    {
        PartPoint* pt = *itr;

        if (pt->Y() > temp.top())
        {
            temp.setTop(pt->Y());
        }
        if (pt->Y() < temp.bottom())
        {
            temp.setBottom(pt->Y());
        }
        if (pt->X() > temp.right())
        {
            temp.setRight(pt->X());
        }
        if (pt->X() < temp.left())
        {
            temp.setLeft(pt->X());
        }
    }

    return temp;
}

void Part::Mirror(Part* dest, valid_axis axis)
{
    QRectF bounding_box = GetExtents();

    switch (axis)
    {
    case XAXIS:
    {
        qreal top = bounding_box.top();

        for (QList<PartPoint*>::Iterator itr=_point_list.begin(); itr != _point_list.end(); itr++)
        {
            PartPoint* pt = *itr;
            qreal diff = top - pt->Y();
            PartPoint* new_pt = new PartPoint(pt->X(), diff + MIRROR_OFFSET, pt->Z(), "From " + pt->Label());
            dest->AddPoint(new_pt);
        }

        for (QList<Part*>::Iterator itr=_sub_part_list.begin(); itr != _sub_part_list.end(); itr++)
        {
            Part* part = *itr;

            Mirror(part, axis);

            dest->AddSubPart(part);
        }

        for (QList<PartDrillObj*>::Iterator itr=_drill_list.begin(); itr != _drill_list.end(); itr++)
        {
            PartDrillObj* drill = *itr;

            PartPoint* pt = drill->center();

            qreal diff = top - pt->Y();
            PartPoint* new_pt = new PartPoint(pt->X(), diff + MIRROR_OFFSET, pt->Z(), "From " + pt->Label());
            dest->AddDrill(new_pt, drill->diameter(), drill->depth());
        }
    }
    break;

    case YAXIS:
    {
        qreal left = bounding_box.left();

        for (QList<PartPoint*>::Iterator itr=_point_list.begin(); itr != _point_list.end(); itr++)
        {
            PartPoint* pt = *itr;
            qreal diff = left - pt->X();
            PartPoint* new_pt = new PartPoint(diff - MIRROR_OFFSET, pt->Y(), pt->Z(), "From " + pt->Label());
            dest->AddPoint(new_pt);
        }

        for (QList<Part*>::Iterator itr=_sub_part_list.begin(); itr != _sub_part_list.end(); itr++)
        {
            Part* part = *itr;

            Part* new_part = new Part(part->_name);
            for (QList<PartPoint*>::Iterator itr=part->_point_list.begin(); itr != part->_point_list.end(); itr++)
            {
                PartPoint* pt = *itr;
                qreal diff = left - pt->X();
                PartPoint* new_pt = new PartPoint(diff - MIRROR_OFFSET, pt->Y(), pt->Z(), "From " + pt->Label());
                new_part->AddPoint(new_pt);
            }

            dest->AddSubPart(new_part);
        }

        for (QList<PartDrillObj*>::Iterator itr=_drill_list.begin(); itr != _drill_list.end(); itr++)
        {
            PartDrillObj* drill = *itr;

            PartPoint* pt = drill->center();

            qreal diff = left - pt->X();
            PartPoint* new_pt = new PartPoint(diff - MIRROR_OFFSET, pt->Y(), pt->Z(), "From " + pt->Label());
            dest->AddDrill(new_pt, drill->diameter(), drill->depth());
        }
    }
    break;
    }
}

Part* Part::Mirror(Part::valid_axis axis, QString name)
{
    Part* dest = new Part(name);

    QRectF bounding_box = GetExtents();

    switch (axis)
    {
    case XAXIS:
    {
        qreal top = bounding_box.top();

        for (QList<PartPoint*>::Iterator itr=_point_list.begin(); itr != _point_list.end(); itr++)
        {
            PartPoint* pt = *itr;
            qreal diff = top - pt->Y();
            PartPoint* new_pt = new PartPoint(pt->X(), diff + MIRROR_OFFSET, pt->Z(), "From " + pt->Label());
            dest->AddPoint(new_pt);
        }

        for (QList<Part*>::Iterator itr=_sub_part_list.begin(); itr != _sub_part_list.end(); itr++)
        {
            Part* part = *itr;

            Mirror(part, axis);

            dest->AddSubPart(part);
        }

        for (QList<PartDrillObj*>::Iterator itr=_drill_list.begin(); itr != _drill_list.end(); itr++)
        {
            PartDrillObj* drill = *itr;

            PartPoint* pt = drill->center();

            qreal diff = top - pt->Y();
            PartPoint* new_pt = new PartPoint(pt->X(), diff + MIRROR_OFFSET, pt->Z(), "From " + pt->Label());
            dest->AddDrill(new_pt, drill->diameter(), drill->depth());
        }
    }
    break;

    case YAXIS:
    {
        qreal left = bounding_box.left();

        for (QList<PartPoint*>::Iterator itr=_point_list.begin(); itr != _point_list.end(); itr++)
        {
            PartPoint* pt = *itr;
            qreal diff = left - pt->X();
            PartPoint* new_pt = new PartPoint(diff - MIRROR_OFFSET, pt->Y(), pt->Z(), "From " + pt->Label());
            dest->AddPoint(new_pt);
        }

        for (QList<Part*>::Iterator itr=_sub_part_list.begin(); itr != _sub_part_list.end(); itr++)
        {
            Part* part = *itr;

            Part* new_part = new Part(part->_name);
            for (QList<PartPoint*>::Iterator itr=part->_point_list.begin(); itr != part->_point_list.end(); itr++)
            {
                PartPoint* pt = *itr;
                qreal diff = left - pt->X();
                PartPoint* new_pt = new PartPoint(diff - MIRROR_OFFSET, pt->Y(), pt->Z(), "From " + pt->Label());
                new_part->AddPoint(new_pt);
            }

            dest->AddSubPart(new_part);
        }

        for (QList<PartDrillObj*>::Iterator itr=_drill_list.begin(); itr != _drill_list.end(); itr++)
        {
            PartDrillObj* drill = *itr;

            PartPoint* pt = drill->center();

            qreal diff = left - pt->X();
            PartPoint* new_pt = new PartPoint(diff - MIRROR_OFFSET, pt->Y(), pt->Z(), "From " + pt->Label());
            dest->AddDrill(new_pt, drill->diameter(), drill->depth());
        }
    }
    break;
    }

    return dest;
}

Part* Part::Transform(PartPoint offset)
{
    Part* dest = new Part(_name);

    for (QList<PartPoint*>::Iterator itr=_point_list.begin(); itr != _point_list.end(); itr++)
    {
        PartPoint* pt = *itr;

        PartPoint* new_pt = new PartPoint(*pt + offset);

        dest->AddPoint(new_pt);
    }

    for (QList<Part*>::Iterator itr=_sub_part_list.begin(); itr != _sub_part_list.end(); itr++)
    {
        Part* part = *itr;

        Part* new_part = new Part(part->_name);
        for (QList<PartPoint*>::Iterator itr=part->_point_list.begin(); itr != part->_point_list.end(); itr++)
        {
            PartPoint* pt = *itr;
            PartPoint* new_pt = new PartPoint(*pt + offset);
            new_part->AddPoint(new_pt);
        }

        dest->AddSubPart(new_part);
    }

    for (QList<PartDrillObj*>::Iterator itr=_drill_list.begin(); itr != _drill_list.end(); itr++)
    {
        PartDrillObj* drill = *itr;

        PartPoint* pt = drill->center();

        PartPoint* new_pt = new PartPoint(*pt + offset);
        dest->AddDrill(new_pt, drill->diameter(), drill->depth());
    }

    for (QList<PartTextObj*>::Iterator itr=_text_list.begin(); itr != _text_list.end(); itr++)
    {
        PartTextObj* text_part = *itr;

        PartPoint* pt = text_part->point();

        PartPoint* new_pt = new PartPoint(*pt + offset);
        dest->AddText(new_pt, text_part->text(), text_part->size());
    }

    return dest;
}

//void Part::SaveLayer(QString layer_name)
//{
//    if (_parent == nullptr)
//    {
//        _parent = this;
//    }
//    _parent.write_shape(layer_name, _point_list);
//
//    for (QList<Part*>::Iterator itr=_sub_part_list.begin(); itr != _sub_part_list.end();itr++)
//    {
//        Part* part = *itr;
//       part->SaveLayer(layer_name);
//    }
//
//    for (QList<PartDrillObj*>::Iterator itr=_drill_list.begin(); itr != _drill_list.end(); itr++)
//    {
//        PartDrillObj* drill = *itr;
//
//        write_hole(layer_name, *drill.center(), drill.diameter());
//    }
//}
