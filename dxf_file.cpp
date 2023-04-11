#include "dxf_file.h"
#include <QTextStream>
#include <QByteArray>
#include <QDebug>

dxf_file::dxf_file()
{

}



void dxf_file::open(QString filename)
{
    file = new QFile(filename);

    file->open(QIODeviceBase::ReadWrite);

    write_header();
}

void dxf_file::close()
{
    write_trailer();

    file->close();
}


void dxf_file::write_header()
{
    QTextStream stream(file);

    stream << "  0\n";
    stream << "SECTION\n";
    stream << "  2\n";
    stream << "HEADER\n";
//    stream << "  9\n";
//    stream << "$INSUNITS\n";
//    stream << "  70\n";
//    stream << "  4\n";
//    stream << "  9\n";
//    stream << "$ACADVER\n";
//    stream << "  1\n";
//    stream << "AC1014\n";
//    stream << "  9\n";
//    stream << "$HANDSEED\n";
//    stream << "  5\n";
//    stream << "FFFF\n";
    stream << "  0\n";
    stream << "ENDSEC\n";
    stream << "  0\n";
    stream << "SECTION\n";
    stream << "  2\n";
    stream << "TABLES\n";
    stream << "  0\n";
    stream << "ENDSEC\n";
    stream << "  0\n";
    stream << "SECTION\n";
    stream << "  2\n";
    stream << "BLOCKS\n";
    stream << "  0\n";
    stream << "ENDSEC\n";
    stream << "  0\n";
    stream << "SECTION\n";
    stream << "  2\n";
    stream << "ENTITIES\n";

}

void dxf_file::write_trailer()
{
    QTextStream stream(file);

    stream << "  0\n";
    stream << "ENDSEC\n";
    stream << "  0\n";
    stream << "EOF\n";
}

void dxf_file::write_shape(QString layer_name, QList<PartPoint *> point_list, PartPoint offset)
{
    QTextStream stream(file);
    PartPoint   temp;
    auto        i = 0;

    for (;i<point_list.count()-1;i++)
    {
        stream << "  0\n";
        stream << "LINE\n";
        stream << "  8\n";
        stream << layer_name << "\n";
        stream << "  10\n";
        temp = *point_list.at(i);
        temp += offset;
        stream << temp.X() << "\n";
        stream << "  20\n";
        stream << temp.Y() << "\n";
        stream << "  30\n";
        stream << temp.Z() << "\n";
        stream << "  11\n";
        temp = *point_list.at(i+1);
        temp += offset;
        stream << temp.X() << "\n";
        stream << "  21\n";
        stream << temp.Y() << "\n";
        stream << "  31\n";
        stream << temp.Z() << "\n";
    }

    // Close the polygon
    stream << "  0\n";
    stream << "LINE\n";
    stream << "  8\n";
    stream << layer_name << "\n";
    stream << "  10\n";
    temp = *point_list.at(i);
    temp += offset;
    stream << temp.X() << "\n";
    stream << "  20\n";
    stream << temp.Y() << "\n";
    stream << "  30\n";
    stream << temp.Z() << "\n";
    stream << "  11\n";
    temp = *point_list.at(0);
    temp += offset;
    stream << temp.X() << "\n";
    stream << "  21\n";
    stream << temp.Y() << "\n";
    stream << "  31\n";
    stream << temp.Z() << "\n";
}
void dxf_file::write_text(QString layer_name, PartPoint pos, QString text, qreal height, PartPoint offset)
{
    pos += offset;

    QTextStream stream(file);
    stream << "  0\n";
    stream << "TEXT\n";
    stream << "  8\n";
    stream << layer_name << "\n";
    stream << "  39\n";  // Thickness
    stream << "20\n";  // Thickness Value
    stream << "  10\n";  // X
    stream << pos.X() << "\n";
    stream << "  20\n";  // Y
    stream << pos.Y() << "\n";
    stream << "  40\n";  // Height
    stream << height << "\n";
    stream << "  41\n";  // Relative X Scale Factor - Width
    stream << "1\n";
    stream << "  1\n";   // Text String
    stream << text << "\n";
}

void dxf_file::write_hole(QString layer_name, PartPoint pos, qreal diameter, PartPoint offset)
{
    pos += offset;
    QTextStream stream(file);

    stream << "  0\n";
    stream << "CIRCLE\n";
    stream << "  8\n";
    stream << layer_name << "\n";
    stream << "  39\n";
    stream << "1\n";
    stream << "  10\n";
    stream << pos.X() << "\n";
    stream << "  20\n";
    stream << pos.Y() << "\n";
    stream << "  40\n";
    stream << diameter << "\n";
}

void dxf_file::write_shape(QList<QPointF> point_list, QPointF offset, qreal z_offset)
{
    QTextStream stream(file);
    QPointF     temp;
    auto        i = 0;

    for (;i<point_list.count()-1;i++)
    {
        stream << "  0\n";
        stream << "LINE\n";
        stream << "  8\n";
        stream << "0\n";
        stream << "  10\n";
        temp = point_list.at(i) + offset;
        stream << temp.x() << "\n";
        stream << "  20\n";
        stream << temp.y() << "\n";
        stream << "  30\n";
        stream << z_offset << "\n";
        stream << "  11\n";
        temp = point_list.at(i+1) + offset;
        stream << temp.x() << "\n";
        stream << "  21\n";
        stream << temp.y() << "\n";
        stream << "  31\n";
        stream << z_offset << "\n";
    }

    // Close the polygon
    stream << "  0\n";
    stream << "LINE\n";
    stream << "  8\n";
    stream << "0\n";
    stream << "  10\n";
    temp = point_list.at(i) + offset;
    stream << temp.x() << "\n";
    stream << "  20\n";
    stream << temp.y() << "\n";
    stream << "  30\n";
    stream << z_offset << "\n";
    stream << "  11\n";
    temp = point_list.at(0) + offset;
    stream << temp.x() << "\n";
    stream << "  21\n";
    stream << temp.y() << "\n";
    stream << "  31\n";
    stream << z_offset << "\n";
}

void dxf_file::write_text(QPointF pos, QString text, qreal height, QPointF offset)
{
    pos += offset;

    QTextStream stream(file);
    stream << "  0\n";
    stream << "TEXT\n";
    stream << "  8\n";
    stream << "label\n";
    stream << "  39\n";  // Thickness
    stream << "20\n";  // Thickness Value
    stream << "  10\n";  // X
    stream << pos.x() << "\n";
    stream << "  20\n";  // Y
    stream << pos.y() << "\n";
    stream << "  40\n";  // Height
    stream << height << "\n";
    stream << "  41\n";  // Relative X Scale Factor - Width
    stream << "1\n";
    stream << "  1\n";   // Text String
    stream << text << "\n";
}

void dxf_file::write_hole(QPointF pos, qreal diameter, QPointF offset)
{
    pos += offset;
    QTextStream stream(file);

    stream << "  0\n";
    stream << "CIRCLE\n";
    stream << "  39\n";
    stream << "1\n";
    stream << "  10\n";
    stream << pos.x() << "\n";
    stream << "  20\n";
    stream << pos.y() << "\n";
    stream << "  40\n";
    stream << diameter << "\n";
}



void dxf_file::write(QList<PartPoint*> point_list)
{
    QTextStream stream(file);
    PartPoint*  temp;
    auto        i = 0;

    for (;i<point_list.count()-1;i++)
    {
        stream << "  0\n";
        stream << "LINE\n";
        stream << "  8\n";
        stream << "0\n";
        stream << "  10\n";
        temp = point_list.at(i);
        stream << temp->X() << "\n";
        stream << "  20\n";
        stream << temp->Y() << "\n";
        stream << "  30\n";
        stream << temp->Z() << "\n";
        stream << "  11\n";
        temp = point_list.at(i+1);
        stream << temp->X() << "\n";
        stream << "  21\n";
        stream << temp->Y() << "\n";
        stream << "  31\n";
        stream << temp->Z() << "\n";
    }

    // Close the polygon
    stream << "  0\n";
    stream << "LINE\n";
    stream << "  8\n";
    stream << "0\n";
    stream << "  10\n";
    temp = point_list.at(i);
    stream << temp->X() << "\n";
    stream << "  20\n";
    stream << temp->Y() << "\n";
    stream << "  30\n";
    stream << temp->Z() << "\n";
    stream << "  11\n";
    temp = point_list.at(0);
    stream << temp->X() << "\n";
    stream << "  21\n";
    stream << temp->Y() << "\n";
    stream << "  31\n";
    stream << temp->Z() << "\n";
}

void dxf_file::write_text(PartPoint pos, QString text, qreal height)
{
    QTextStream stream(file);
    stream << "  0\n";
    stream << "TEXT\n";
    stream << "  8\n";
    stream << "label\n";
    stream << "  39\n";  // Thickness
    stream << "20\n";  // Thickness Value
    stream << "  10\n";  // X
    stream << pos.X() << "\n";
    stream << "  20\n";  // Y
    stream << pos.Y() << "\n";
    stream << "  40\n";  // Height
    stream << height << "\n";
    stream << "  41\n";  // Relative X Scale Factor - Width
    stream << "1\n";
    stream << "  1\n";   // Text String
    stream << text << "\n";
}

void dxf_file::write_hole(PartPoint pos, qreal diameter)
{
    QTextStream stream(file);

    stream << "  0\n";
    stream << "CIRCLE\n";
    stream << "  39\n";
    stream << "1\n";
    stream << "  10\n";
    stream << pos.X() << "\n";
    stream << "  20\n";
    stream << pos.Y() << "\n";
    stream << "  40\n";
    stream << diameter << "\n";
}
