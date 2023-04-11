#pragma once

#include <QObject>
#include <QFile>
#include <QList>
#include <QPointF>
#include "partpoint.h"

class dxf_file
{
public:
    dxf_file();

    void open(QString filename);
    void close();

    void write_shape(QString layer_name, QList<PartPoint*> point_list, PartPoint offset = PartPoint(0.0, 0.0, 0.0));
    void write_text(QString layer_name, PartPoint pos, QString text, qreal height, PartPoint offset = PartPoint(0.0, 0.0, 0.0));
    void write_hole(QString layer_name, PartPoint pos, qreal diameter, PartPoint offset = PartPoint(0.0, 0.0, 0.0));

    void write_shape(QList<QPointF> point_list, QPointF offset = QPointF(0.0, 0.0), qreal z_offset = 0.0);
    void write_text(QPointF pos, QString text, qreal height, QPointF offset = QPointF(0.0, 0.0));
    void write_hole(QPointF pos, qreal diameter, QPointF offset = QPointF(0.0, 0.0));

    void write(QList<PartPoint*> point_list);
    void write_text(PartPoint pos, QString text, qreal height);
    void write_hole(PartPoint pos, qreal diameter);

private:
    QFile* file;

    void write_header();
    void write_trailer();

};

