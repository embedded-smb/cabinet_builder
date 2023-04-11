#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dxf_file.h"
#include "part.h"
#include "partpoint.h"
#include "sheet.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // connect button to calculate function
    connect(ui->go, SIGNAL(clicked(bool)), this, SLOT(CalculateParts()));

    // initialize UI
    ui->combo_sides_back_jointery->addItem("Dado");
    ui->combo_sides_face_frame_jointery->addItem("Dado");
    ui->combo_sides_face_frame_jointery->addItem("Biscuits");
    ui->combo_sides_back_jointery->setCurrentIndex(0);
    ui->combo_sides_face_frame_jointery->setCurrentIndex(0);
    ui->box_depth->setText("24");
    ui->box_width->setText("30");
    ui->box_height->setText("35");
    ui->mat_back_thickness->setText("0.234");
    ui->mat_bottom_thickness->setText("0.703");
    ui->mat_faceframe_thickness->setText("0.75");
    ui->mat_sides_thickness->setText("0.703");
    ui->toekick_depth->setText("3.0");
    ui->toekick_height->setText("3.5");
    ui->bottom_offset->setText("0.0");
    ui->top_xpiece_width->setText("3.0");
    ui->integrated_toekick->setCheckState(Qt::Checked);
    ui->gen_top_xpiece->setCheckState(Qt::Checked);
    ui->subtract_faceframe_from_sides->setCheckState(Qt::Checked);
    ui->combo_sides_face_frame_jointery->setCurrentIndex(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CalculateParts()
{
//    QList<QPointF> point_list;
    double width                = ui->box_width->text().toDouble();
    double height               = ui->box_height->text().toDouble();
    double depth                = ui->box_depth->text().toDouble();
    double sides_thk            = ui->mat_sides_thickness->text().toDouble();
    double back_thk             = ui->mat_back_thickness->text().toDouble();
    double bottom_thk           = ui->mat_bottom_thickness->text().toDouble();
    double face_thk             = ui->mat_faceframe_thickness->text().toDouble();
    bool   offset_face          = ui->subtract_faceframe_from_sides->isChecked();
    bool   toekick_present      = ui->integrated_toekick->isChecked();
    double toekick_depth        = ui->toekick_depth->text().toDouble();
    double toekick_height       = ui->toekick_height->text().toDouble();
    bool   gen_top_crosses      = ui->gen_top_xpiece->isChecked();
    double top_crosses_width    = ui->top_xpiece_width->text().toDouble();
    double bottom_offset        = ui->bottom_offset->text().toDouble();
    bool   gen_shelf_pin_holes  = ui->gen_shelf_pins->isChecked();
    double shelf_pin_spacing    = ui->shelf_hole_spacing->text().toDouble();
    double shelf_pin_diameter   = ui->shelf_hole_diameter->text().toDouble();
    double shelf_pin_start      = ui->shelf_hole_height_range_start->text().toDouble();
    double shelf_pin_stop       = ui->shelf_hole_height_range_end->text().toDouble();
    quint16 side_back_joint     = ui->combo_sides_back_jointery->currentIndex();
    quint16 side_face_joint     = ui->combo_sides_face_frame_jointery->currentIndex();

//    QPointF current_origin(0.0, 0.0);
    double dado_depth = 0.25;

    double side_width = depth;
    if (offset_face)
    {
        side_width -= face_thk;
    }
    if (side_face_joint == 0)
    {
        side_width += dado_depth;
    }

    double side_height = height;
    if (!toekick_present)
    {
        side_height -= toekick_height;
    }

    double face_frame_offset = 0.0;

    if (side_face_joint == 0)
    {
        face_frame_offset = (face_thk/2.0);
    }

    //=============================================================================================
    //=============================================================================================
    // Create right side outline
    //=============================================================================================
    //=============================================================================================
    Part* right_side = new Part("Right Side");
    right_side->AddPoint(new PartPoint(0.0,                            0.0,         0.0, "lower left"));
    right_side->AddPoint(new PartPoint(0.0,                            side_height, 0.0, "upper left"));
    right_side->AddPoint(new PartPoint(side_width + face_frame_offset, side_height, 0.0, "upper right"));
//    QPointF right_side_ll(0.0, 0.0);
    QPointF right_side_ul(0, side_height);
    QPointF right_side_ur(side_width + face_frame_offset, side_height);
//    point_list.append(right_side_ll);
//    point_list.append(right_side_ul);
//    point_list.append(right_side_ur);

    if (toekick_present)
    {
        right_side->AddPoint(new PartPoint(side_width + face_frame_offset,                 toekick_height, 0.0, "front top of toekick"));
        right_side->AddPoint(new PartPoint(side_width - toekick_depth + face_frame_offset, toekick_height, 0.0, "back top of toekick"));
        right_side->AddPoint(new PartPoint(side_width - toekick_depth + face_frame_offset, 0.0,            0.0, "lower right"));
        QPointF right_side_front_top_of_toekick(side_width + face_frame_offset, toekick_height);
        QPointF right_side_back_top_of_toekick(side_width - toekick_depth + face_frame_offset, toekick_height);
        QPointF right_side_lr(side_width - toekick_depth + face_frame_offset, 0.0);
//        point_list.append(right_side_front_top_of_toekick);
//        point_list.append(right_side_back_top_of_toekick);
//        point_list.append(right_side_lr);
    }
    else
    {
        right_side->AddPoint(new PartPoint(side_width + face_frame_offset, 0.0, 0.0, "lower right"));
        QPointF right_side_lr(side_width + face_frame_offset, 0.0);
//        point_list.append(right_side_lr);
    }

//    file.write_shape(point_list);
//    point_list.clear();

#if 1
    //=============================================================================================
    // Create Dado for back
    //=============================================================================================
//    QPointF right_back_dado_ll(0.0, 0.0);
    QPointF right_back_dado_ul(0.0, side_height);
    QPointF right_back_dado_ur(back_thk, side_height);
    QPointF right_back_dado_lr(back_thk, 0.0);
//    point_list.append(right_back_dado_ll);
//    point_list.append(right_back_dado_ul);
//    point_list.append(right_back_dado_ur);
//    point_list.append(right_back_dado_lr);

    right_side->AddDado("Back Dado", new PartPoint(0.0, 0.0, 1.0), back_thk, side_height, sides_thk/2.0);
//    file.write_shape(point_list, QPointF(0.0, 0.0), 0.005);
//    point_list.clear();

    //=============================================================================================
    // Create Dado for bottom
    //=============================================================================================
    QPointF right_bottom_dado_ll;
    QPointF right_bottom_dado_ul;
    QPointF right_bottom_dado_ur;
    QPointF right_bottom_dado_lr;

    if (toekick_present)
    {
        right_bottom_dado_ll.setX(back_thk);
        right_bottom_dado_ll.setY(toekick_height + bottom_offset);
        right_bottom_dado_ul.setX(back_thk);
        right_bottom_dado_ul.setY(toekick_height + bottom_offset + bottom_thk);
        right_bottom_dado_ur.setX(side_width);
        right_bottom_dado_ur.setY(toekick_height + bottom_offset + bottom_thk);
        right_bottom_dado_lr.setX(side_width);
        right_bottom_dado_lr.setY(toekick_height + bottom_offset);
    }
    else
    {
        right_bottom_dado_ll.setX(back_thk);
        right_bottom_dado_ll.setY(0.0 + bottom_offset);
        right_bottom_dado_ul.setX(back_thk);
        right_bottom_dado_ul.setY(0.0 + bottom_offset + bottom_thk);
        right_bottom_dado_ur.setX(side_width);
        right_bottom_dado_ur.setY(0.0 + bottom_offset + bottom_thk);
        right_bottom_dado_lr.setX(side_width);
        right_bottom_dado_lr.setY(0.0 + bottom_offset);
    }
//    point_list.append(right_bottom_dado_ll);
//    point_list.append(right_bottom_dado_ul);
//    point_list.append(right_bottom_dado_ur);
//    point_list.append(right_bottom_dado_lr);

//    right_side.AddDado("Bottom Dado", new PartPoint(right_bottom_dado_ll.rx(), right_bottom_dado_ll.ry(), 0.0), side_width, bottom_thk, sides_thk/2.0);
    right_side->AddDado("Bottom Dado", right_bottom_dado_ll, side_width-back_thk, bottom_thk, sides_thk/2.0);
//void AddDado(QString name, PartPoint  pt, qreal length, qreal width, qreal depth);
//    file.write_shape(point_list);
//    point_list.clear();

    //=============================================================================================
    // Create Dados for top cross pieces
    //=============================================================================================
    QPointF side_top_cross_dado_ll_back(back_thk, side_height - sides_thk);
    QPointF side_top_cross_dado_ul(back_thk, side_height);
    QPointF side_top_cross_dado_ur(back_thk + top_crosses_width, side_height);
    QPointF side_top_cross_dado_lr(back_thk + top_crosses_width, side_height - sides_thk);
    QPointF side_top_cross_dato_ll_front(side_top_cross_dado_ll_back + QPointF(side_width - top_crosses_width - back_thk, 0.0));

    if (gen_top_crosses)
    {
        right_side->AddDado("Top Cross Dado", side_top_cross_dado_ll_back, top_crosses_width, sides_thk, sides_thk/2.0);
        right_side->AddDado("Top Cross Front", side_top_cross_dato_ll_front, top_crosses_width, sides_thk, sides_thk/2.0);

//        point_list.append(side_top_cross_dado_ll_back);
//        point_list.append(side_top_cross_dado_ul);
//        point_list.append(side_top_cross_dado_ur);
//        point_list.append(side_top_cross_dado_lr);

//        file.write_shape(point_list, QPointF(0.0, 0.0));
//        file.write_shape(point_list, QPointF(side_width - top_crosses_width - back_thk, 0.0));
//        point_list.clear();
    }

right_side->open("project_xxx_right_side.dxf");
right_side->SaveLayer("Sides");
right_side->close();
    //=============================================================================================
    // Label Part - Right Side
    //=============================================================================================
//    file.write_text(QPointF(right_bottom_dado_ll.x() + 1.0, right_bottom_dado_ul.y() + 1.0), "Right Side", 1.0);

    //=============================================================================================
    //=============================================================================================
    // Create left side outline
    //=============================================================================================
    //=============================================================================================
Part* left_side = new Part("Left Side");
right_side->Mirror(left_side, Part::YAXIS);
left_side->open("project_xxx_left_side.dxf");
left_side->SaveLayer("Sides");
left_side->close();

Sheet sides("Sides");
PartPoint current_offset(0.25, 0.25, 0.0, "Offset");

// back, bottom, toekick, cross_piece
sides.AddPart(left_side, current_offset);

QRectF bound = left_side->GetExtents();
current_offset += PartPoint(bound.width() + 0.25, 0.0, 0.0);

sides.AddPart(right_side, current_offset);

sides.WriteDXF("Prj_Sides.dxf");
#if 0
    if (toekick_present)
    {
        QPointF left_side_ll_of_toekick(toekick_depth+face_frame_offset, 0.0);
        QPointF left_side_ul_of_toekick(toekick_depth+face_frame_offset, toekick_height);
        QPointF left_side_ll_of_box(0.0, toekick_height);
        QPointF left_side_ul(0.0, side_height);
        point_list.append(left_side_ll_of_toekick);
        point_list.append(left_side_ul_of_toekick);
        point_list.append(left_side_ll_of_box);
        point_list.append(left_side_ul);

        QPointF left_side_ur(side_width+face_frame_offset, height);
        QPointF left_side_lr(side_width+face_frame_offset, 0.0);
        point_list.append(left_side_ur);
        point_list.append(left_side_lr);
    }
    else
    {
        QPointF left_side_ll_of_box(0.0, 0.0);
        QPointF left_side_ul(0.0, side_height);
        QPointF left_side_ur(side_width+face_frame_offset, side_height);
        QPointF left_side_lr(side_width+face_frame_offset, 0.0);
        point_list.append(left_side_ll_of_box);
        point_list.append(left_side_ul);
        point_list.append(left_side_ur);
        point_list.append(left_side_lr);
    }

    file.write_shape(point_list, QPointF(26.0, 0.0));
    point_list.clear();

    //=============================================================================================
    // Create Dado for back
    //=============================================================================================
    QPointF back_dado_ll(side_width+face_frame_offset-back_thk, 0.0);
    QPointF back_dado_ul(side_width+face_frame_offset-back_thk, side_height);
    QPointF back_dado_ur(side_width+face_frame_offset, side_height);
    QPointF back_dado_lr(side_width+face_frame_offset, 0.0);
    point_list.append(back_dado_ll);
    point_list.append(back_dado_ul);
    point_list.append(back_dado_ur);
    point_list.append(back_dado_lr);
    file.write_shape(point_list, QPointF(26.0, 0.0));
    point_list.clear();

    //=============================================================================================
    // Create Dado for bottom
    //=============================================================================================
    QPointF bottom_dado_ll;
    QPointF bottom_dado_ul;
    QPointF bottom_dado_ur;
    QPointF bottom_dado_lr;

    if (toekick_present)
    {
        bottom_dado_ll.setX(0.0);
        bottom_dado_ll.setY(toekick_height + bottom_offset);
        bottom_dado_ul.setX(0.0);
        bottom_dado_ul.setY(toekick_height + bottom_offset + bottom_thk);
        bottom_dado_ur.setX(side_width+face_frame_offset-back_thk);
        bottom_dado_ur.setY(toekick_height + bottom_offset + bottom_thk);
        bottom_dado_lr.setX(side_width+face_frame_offset-back_thk);
        bottom_dado_lr.setY(toekick_height + bottom_offset);
    }
    else
    {
        bottom_dado_ll.setX(0.0);
        bottom_dado_ll.setY(0.0 + bottom_offset);
        bottom_dado_ul.setX(0.0);
        bottom_dado_ul.setY(0.0 + bottom_offset + bottom_thk);
        bottom_dado_ur.setX(side_width+face_frame_offset-back_thk);
        bottom_dado_ur.setY(0.0 + bottom_offset + bottom_thk);
        bottom_dado_lr.setX(side_width+face_frame_offset-back_thk);
        bottom_dado_lr.setY(0.0 + bottom_offset);
    }
    point_list.append(bottom_dado_ll);
    point_list.append(bottom_dado_ul);
    point_list.append(bottom_dado_ur);
    point_list.append(bottom_dado_lr);

    file.write_shape(point_list, QPointF(26.0, 0.0));
    point_list.clear();

    //=============================================================================================
    // Create Dados for top cross pieces
    //=============================================================================================
    if (gen_top_crosses)
    {
        point_list.append(side_top_cross_dado_ll_back);
        point_list.append(side_top_cross_dado_ul);
        point_list.append(side_top_cross_dado_ur);
        point_list.append(side_top_cross_dado_lr);

        file.write_shape(point_list, QPointF(26.0 - back_thk, 0.0));
        file.write_shape(point_list, QPointF(26.0 + side_width - top_crosses_width - (2.0 * back_thk), 0.0));
        point_list.clear();
    }

    //=============================================================================================
    // Create Shelf Pin Holes
    //=============================================================================================
    if (gen_shelf_pin_holes)
    {
        qreal start = shelf_pin_start + bottom_offset + bottom_thk;
        if (toekick_present)
        {
            start += toekick_height;
        }

        qreal stop = start + (shelf_pin_stop - shelf_pin_start);

        for (qreal y_pos=start;y_pos <= stop;y_pos += shelf_pin_spacing)
        {
            QPointF pos_r(3.0, y_pos);
            QPointF pos_f(20.0, y_pos);
            file.write_hole(pos_r, shelf_pin_diameter/2.0);
            file.write_hole(pos_f, shelf_pin_diameter/2.0);
        }
    }
//    shelf_pin_spacing
//    shelf_pin_diameter
//    shelf_pin_start
//    shelf_pin_stop

    //=============================================================================================
    // Label Part - Left Side
    //=============================================================================================
    file.write_text(QPointF(bottom_dado_ll.x() + 1.0, bottom_dado_ul.y() + 1.0), "Left Side", 1.0, QPointF(26.0, 0.0));
    ui->plainTextEdit->appendPlainText("Sides: " + QString::number(side_width) + "\"w x " + QString::number(side_height) + "\"h x " + QString::number(sides_thk) + "\" thick");
#endif
    //=============================================================================================
    //=============================================================================================
    // Create back
    //=============================================================================================
    //=============================================================================================
    double back_width = width;
    if (side_back_joint == 0)
    {
        back_width -= sides_thk;
    }

    double back_height = height;
    if (!toekick_present)
    {
        back_height -= toekick_height;
    }

    QPointF back_ll(0.0, 0.0);
    QPointF back_ul(0.0, back_height);
    QPointF back_ur(back_width, back_height);
    QPointF back_lr(back_width, 0.0);

    Part* back = new Part("Back");
    back->AddPoint(new PartPoint(back_ll, "Back-LL"));
    back->AddPoint(new PartPoint(back_ul, "Back-UL"));
    back->AddPoint(new PartPoint(back_ur, "Back-UR"));
    back->AddPoint(new PartPoint(back_lr, "Back-LR"));

//    point_list.append(back_ll);
//    point_list.append(back_ul);
//    point_list.append(back_ur);
//    point_list.append(back_lr);

//    file.write_shape(point_list, QPointF(0.0, 36.0));
//    point_list.clear();

    //=============================================================================================
    // Label Part - Back
    //=============================================================================================
//    file.write_text(QPointF(back_ll.x() + 1.0, back_ll.y() + 1.0), "Back", 1.0, QPointF(0.0, 36.0));

    ui->plainTextEdit->appendPlainText("Back: " + QString::number(back_width) + "\"w x " + QString::number(back_height) + "\"h x " + QString::number(back_thk) + "\" thick");
back->open("Project_xxx_back.dxf");
back->SaveLayer("Back");
back->close();
    //=============================================================================================
    //=============================================================================================
    // Create bottom
    //=============================================================================================
    //=============================================================================================    double bottom_width = width - sides_thk;
    double bottom_depth = depth - back_thk;
    if (offset_face)
    {
        bottom_depth -= face_thk;
    }
    if (side_face_joint == 0)
    {
        bottom_depth += dado_depth;
    }

    double bottom_width = width - sides_thk;

    QPointF bottom_fl(0.0, 0.0);
    QPointF bottom_bl(0.0, bottom_depth);
    QPointF bottom_br(bottom_width, bottom_depth);
    QPointF bottom_fr(bottom_width, 0.0);
//    point_list.append(bottom_fl);
//    point_list.append(bottom_bl);
//    point_list.append(bottom_br);
//    point_list.append(bottom_fr);

//    file.write_shape(point_list, QPointF(36.0, 36.0));
//    point_list.clear();
    Part* bottom = new Part("Bottom");
    bottom->AddPoint(new PartPoint(bottom_fl, "Bottom-FrontL"));
    bottom->AddPoint(new PartPoint(bottom_bl, "Bottom-BackL"));
    bottom->AddPoint(new PartPoint(bottom_br, "Bottom-BackR"));
    bottom->AddPoint(new PartPoint(bottom_fr, "Bottom-FrontR"));
    bottom->open("Project_xxx_Bottom.dxf");
    bottom->SaveLayer("Bottom");
    bottom->close();
    Sheet inside("Inside");
    current_offset = PartPoint(0.25, 0.25, 0.0);

    inside.AddPart(bottom, current_offset);
    //=============================================================================================
    // Label Part - Back
    //=============================================================================================
//    file.write_text(QPointF(bottom_fl.x() + 1.0, bottom_fl.y() + 1.0), "Bottom", 1.0, QPointF(36.0, 36.0));

    //=============================================================================================
    //=============================================================================================
    // Create toekick front
    //=============================================================================================
    //=============================================================================================
    QPointF toekick_front_ll(0.0, 0.0);
    QPointF toekick_front_ul(0.0, toekick_height);
    QPointF toekick_front_ur(bottom_width, toekick_height);
    QPointF toekick_front_lr(bottom_width, 0.0);
//    point_list.append(toekick_front_ll);
//    point_list.append(toekick_front_ul);
//    point_list.append(toekick_front_ur);
//    point_list.append(toekick_front_lr);

//    file.write_shape(point_list, QPointF(0.0, 72.0));
//    point_list.clear();
    Part* toekick = new Part("Toekick");
    toekick->AddPoint(new PartPoint(toekick_front_ll, "Toekick-LL"));
    toekick->AddPoint(new PartPoint(toekick_front_ul, "Toekick-UL"));
    toekick->AddPoint(new PartPoint(toekick_front_ur, "Toekick-UR"));
    toekick->AddPoint(new PartPoint(toekick_front_lr, "Toekick-LR"));
    toekick->open("Project_xxx_Toekick.dxf");
    toekick->SaveLayer("Toekick");
    toekick->close();
    bound = bottom->GetExtents();
    current_offset += PartPoint(0.0, bound.height() + 0.25, 0.0);

    inside.AddPart(toekick, current_offset);
    //=============================================================================================
    // Label Part - Back
    //=============================================================================================
//    file.write_text(QPointF(bottom_fl.x() + 1.0, bottom_fl.y() + 1.0), "Toekick", 1.0, QPointF(0.0, 72.0));

    ui->plainTextEdit->appendPlainText("Bottom: " + QString::number(bottom_width) + "\"w x " + QString::number(bottom_depth) + "\"h x " + QString::number(bottom_thk) + "\" thick");

    //=============================================================================================
    //=============================================================================================
    // Create top cross pieces
    //=============================================================================================
    //=============================================================================================
    if (gen_top_crosses)
    {
        QPointF cross_fl(0.0, 0.0);
        QPointF cross_rl(0.0, top_crosses_width);
        QPointF cross_rr(bottom_width, top_crosses_width);
        QPointF cross_fr(bottom_width, 0.0);
//        point_list.append(cross_fl);
//        point_list.append(cross_rl);
//        point_list.append(cross_rr);
//        point_list.append(cross_fr);

//        file.write_shape(point_list, QPointF(0.0, 82.0));
//        file.write_shape(point_list, QPointF(0.0, 86.0));
//        point_list.clear();
        Part* cross_piece = new Part("Cross Piece");
        cross_piece->AddPoint(new PartPoint(cross_fl, "Cross_FL"));
        cross_piece->AddPoint(new PartPoint(cross_rl, "Cross_RL"));
        cross_piece->AddPoint(new PartPoint(cross_rr, "Cross_RR"));
        cross_piece->AddPoint(new PartPoint(cross_fr, "Cross_FR"));
        cross_piece->open("Project_xxx_CrossPiece.dxf");
        cross_piece->SaveLayer("Cross Piece");
        cross_piece->close();

        bound = toekick->GetExtents();
        current_offset += PartPoint(0.0, bound.height() + 0.25, 0.0);
        inside.AddPart(cross_piece, current_offset);

        bound = cross_piece->GetExtents();
        current_offset += PartPoint(0.0, bound.height() + 0.25, 0.0);
        inside.AddPart(cross_piece, current_offset);

        //=============================================================================================
        // Label Part - Back
        //=============================================================================================
//        file.write_text(QPointF(cross_fl.x() + 1.0, cross_fl.y() + 1.0), "Top Cross", 1.0, QPointF(0.0, 82.0));
//        file.write_text(QPointF(cross_fl.x() + 1.0, cross_fl.y() + 1.0), "Top Cross", 1.0, QPointF(0.0, 86.0));

    }
#endif
    inside.WriteDXF("Prj_inside.dxf");
//    file.close();







}
