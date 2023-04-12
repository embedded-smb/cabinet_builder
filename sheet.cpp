#include "sheet.h"
#include "dxf_file.h"
#include "partdrillobj.h"
#include "parttextobj.h"

#include <QRectF>
#include <QVector>
#include <QDebug>

Sheet::Sheet()
    : _layer(""),
      _width(48),
      _length(96)
{}

Sheet::Sheet(QString layer)
    : _layer(layer),
      _width(48),
      _length(96)
{}

void Sheet::AddPart(Part* part, PartPoint offset)
{
    _part_list.append(part->Transform(offset));
}

void Sheet::WriteDXF(QString filename)
{
    dxf_file file;

    file.open(filename);

    QList<Part*>::Iterator part_itr;
    Part* temp;
    Part* subpart_temp;

    for (part_itr=_part_list.begin();part_itr!=_part_list.end();part_itr++)
    {
        temp = *part_itr;
        file.write(temp->_point_list);

        for (QList<PartDrillObj*>::Iterator itr=temp->_drill_list.begin(); itr != temp->_drill_list.end(); itr++)
        {
            PartDrillObj* drill = *itr;

            file.write_hole(drill->center(), drill->diameter());
        }

        for (QList<PartTextObj*>::Iterator itr=temp->_text_list.begin(); itr != temp->_text_list.end(); itr++)
        {
            PartTextObj* text_obj = *itr;

            file.write_text(text_obj->point(), text_obj->text(), text_obj->size());
        }

        QList<Part*>::Iterator subpart_itr;

        for (subpart_itr=temp->_sub_part_list.begin();subpart_itr!=temp->_sub_part_list.end();subpart_itr++)
        {
            subpart_temp = *subpart_itr;

            file.write(subpart_temp->_point_list);
        }
    }

    file.close();
}


quint32 grid_res = 6;
quint8 Grids(quint8 measure)
{
    return measure / grid_res;
}
void Sheet::InitGrid()
{
    for (quint8 x=0;x<_grid_width;x++)
    {
        for (quint8 y=0;y<_grid_length;y++)
        {
            _grid[x][y] = false;
        }
    }
}
void Sheet::AutoArrange()
{
    // Fitting parts to a 6"x6" grid.
    _grid_width = Grids(_width);
    _grid_length = Grids(_length);

    InitGrid();



    QList<Part*>::Iterator itr;
    Part* part;

    QList<QRectF> extents_list;

    for (itr=_part_list.begin();itr!=_part_list.end();itr++)
    {
        part = *itr;
        extents_list.append(part->GetExtents());
    }
}
