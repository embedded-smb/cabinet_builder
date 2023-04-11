#include "sheet.h"
#include "dxf_file.h"
#include "partdrillobj.h"

#include <QRectF>
#include <QVector>

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
//    AutoArrange();
    dxf_file file;

    file.open(filename);

    QList<Part*>::Iterator part_itr;
    Part* temp;

    for (part_itr=_part_list.begin();part_itr!=_part_list.end();part_itr++)
    {
        temp = *part_itr;
        file.write(temp->_point_list);

        for (QList<PartDrillObj>::Iterator itr=temp->_drill_list.begin(); itr != temp->_drill_list.end(); itr++)
        {
            PartDrillObj drill = *itr;

            file.write_hole(*drill.center(), drill.diameter());
        }

        QList<Part*>::Iterator subpart_itr;

        for (subpart_itr=_part_list.begin();subpart_itr!=_part_list.end();subpart_itr++)
        {
            temp = *subpart_itr;
            file.write(temp->_point_list);
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
