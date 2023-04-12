#pragma once
#include <QString>
#include <QList>
#include <QVector>

#include "part.h"

class Sheet
{
public:
    Sheet();
    Sheet(QString layer);

    void AddPart(Part* part, PartPoint offset);

    void WriteDXF(QString filename);

private:
    QString  _layer;
    quint32  _width;
    quint32  _length;
    quint32  _grid_width;
    quint32  _grid_length;

    QList<Part*>            _part_list;
    QVector<QVector<bool>> _grid;

    void AutoArrange();
    void InitGrid();

};
