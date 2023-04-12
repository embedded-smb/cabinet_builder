#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dxf_file.h"
#include "part.h"
#include "partpoint.h"
#include "sheet.h"
#include "timer.h"

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
    timer tmr("CalculateParts");
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

    Sheet sides_sheet("Sides");
    Sheet inside_sheet("Insde");
    Sheet back_sheet("Back");

    QRectF bound;

    PartPoint current_offset(0.25, 0.25, 0.0, "Offset");
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

    right_side->AddPoint(new PartPoint(0.0, 0.0, 0.0, "lower left"));
    right_side->AddPoint(new PartPoint(0.0, side_height, 0.0, "upper left"));
    right_side->AddPoint(new PartPoint(side_width + face_frame_offset, side_height, 0.0, "upper right"));

    if (toekick_present)
    {
        right_side->AddPoint(new PartPoint(side_width + face_frame_offset,                 toekick_height, 0.0, "front top of toekick"));
        right_side->AddPoint(new PartPoint(side_width - toekick_depth + face_frame_offset, toekick_height, 0.0, "back top of toekick"));
        right_side->AddPoint(new PartPoint(side_width - toekick_depth + face_frame_offset, 0.0,            0.0, "lower right"));
    }
    else
    {
        right_side->AddPoint(new PartPoint(side_width + face_frame_offset, 0.0, 0.0, "lower right"));
    }

    //=============================================================================================
    // Create Dado for back
    //=============================================================================================
    right_side->AddDado("Back Dado", new PartPoint(0.0, 0.0, 1.0), back_thk, side_height, sides_thk/2.0);

    //=============================================================================================
    // Create Dado for bottom
    //=============================================================================================
    QPointF right_bottom_dado_ll;

    if (toekick_present)
    {
        right_bottom_dado_ll.setX(back_thk);
        right_bottom_dado_ll.setY(toekick_height + bottom_offset);
    }
    else
    {
        right_bottom_dado_ll.setX(back_thk);
        right_bottom_dado_ll.setY(0.0 + bottom_offset);
    }

    right_side->AddDado("Bottom Dado", new PartPoint(right_bottom_dado_ll), side_width-back_thk, bottom_thk, sides_thk/2.0);

    //=============================================================================================
    // Create Dados for top cross pieces
    //=============================================================================================
    QPointF side_top_cross_dado_ll_back(back_thk, side_height - sides_thk);
    QPointF side_top_cross_dato_ll_front(side_top_cross_dado_ll_back + QPointF(side_width - top_crosses_width - back_thk, 0.0));

    if (gen_top_crosses)
    {
        right_side->AddDado("Top Cross Dado", new PartPoint(side_top_cross_dado_ll_back), top_crosses_width, sides_thk, sides_thk/2.0);
        right_side->AddDado("Top Cross Front", new PartPoint(side_top_cross_dato_ll_front), top_crosses_width, sides_thk, sides_thk/2.0);
    }


    //=============================================================================================
    // Label Part - Right Side
    //=============================================================================================
    bound = right_side->GetExtents();
    right_side->AddText(new PartPoint(bound.left() + 1.0, bound.bottom() + fabs(bound.height()) / 2.0, 0.0), "Right Side", 1.0);

    //=============================================================================================
    //=============================================================================================
    // Create left side outline
    //=============================================================================================
    //=============================================================================================
    Part* left_side = right_side->Mirror(Part::YAXIS, "Left Side");

    //=============================================================================================
    // Label Part - Left Side
    //=============================================================================================
    bound = left_side->GetExtents();
    left_side->AddText(new PartPoint(bound.left() + 1.0, bound.bottom() + fabs(bound.height()) / 2.0, 0.0), "Left Side", 1.0);
    left_side = left_side->Transform(PartPoint(right_side->GetExtents().right() + fabs(left_side->GetExtents().left()) + 0.25, 0.0, 0.0));

    sides_sheet.AddPart(right_side, current_offset);
    sides_sheet.AddPart(left_side,  current_offset);

    sides_sheet.WriteDXF("Prj_Sides.dxf");

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

    Part* back = new Part("Back");
    back->AddPoint(new PartPoint(0.0, 0.0, 0.0, "Back-LL"));
    back->AddPoint(new PartPoint(0.0, back_height, 0.0, "Back-UL"));
    back->AddPoint(new PartPoint(back_width, back_height, 0.0, "Back-UR"));
    back->AddPoint(new PartPoint(back_width, 0.0, 0.0, "Back-LR"));

    //=============================================================================================
    // Label Part - Back
    //=============================================================================================
    bound = back->GetExtents();
    back->AddText(new PartPoint(bound.left() + 1.0, bound.bottom() + fabs(bound.height()) / 2.0, 0.0), "Back", 1.0);

    current_offset = PartPoint(0.25, 0.25, 0.0);
    back_sheet.AddPart(back, current_offset);
    back_sheet.WriteDXF("Prj_Back.dxf");

    //=============================================================================================
    //=============================================================================================
    // Create bottom
    //=============================================================================================
    //=============================================================================================
    double bottom_width = width - sides_thk;
    double bottom_depth = depth - back_thk;

    if (offset_face)
    {
        bottom_depth -= face_thk;
    }
    if (side_face_joint == 0)
    {
        bottom_depth += dado_depth;
    }

    Part* bottom = new Part("Bottom");
    bottom->AddPoint(new PartPoint(0.0,          0.0,          0.0, "Bottom-FrontL"));
    bottom->AddPoint(new PartPoint(0.0,          bottom_depth, 0.0, "Bottom-BackL"));
    bottom->AddPoint(new PartPoint(bottom_width, bottom_depth, 0.0, "Bottom-BackR"));
    bottom->AddPoint(new PartPoint(bottom_width, 0.0,          0.0, "Bottom-FrontR"));


    //=============================================================================================
    // Label Part - Bottom
    //=============================================================================================
    bound = bottom->GetExtents();
    bottom->AddText(new PartPoint(bound.left() + 1.0, bound.bottom() + fabs(bound.height()) / 2.0, 0.0), "Bottom", 1.0);

    current_offset = PartPoint(0.25, 0.25, 0.0);
    inside_sheet.AddPart(bottom, current_offset);

    //=============================================================================================
    //=============================================================================================
    // Create toekick front
    //=============================================================================================
    //=============================================================================================
    Part* toekick = new Part("Toekick");
    toekick->AddPoint(new PartPoint(0.0, 0.0, 0.0, "Toekick-LL"));
    toekick->AddPoint(new PartPoint(0.0, toekick_height, 0.0, "Toekick-UL"));
    toekick->AddPoint(new PartPoint(bottom_width, toekick_height, 0.0, "Toekick-UR"));
    toekick->AddPoint(new PartPoint(bottom_width, 0.0, 0.0, "Toekick-LR"));

    //=============================================================================================
    // Label Part - Back
    //=============================================================================================
    bound = toekick->GetExtents();
    toekick->AddText(new PartPoint(bound.left() + 1.0, bound.bottom() + fabs(bound.height()) / 2.0, 0.0), "Toekick", 1.0);

    bound = bottom->GetExtents();
    current_offset += PartPoint(0.0, fabs(bound.height()) + 0.25, 0.0);

    inside_sheet.AddPart(toekick, current_offset);

    //=============================================================================================
    //=============================================================================================
    // Create top cross pieces
    //=============================================================================================
    //=============================================================================================
    if (gen_top_crosses)
    {
        Part* cross_piece = new Part("Cross Piece");
        cross_piece->AddPoint(new PartPoint(0.0, 0.0, 0.0, "Cross_FL"));
        cross_piece->AddPoint(new PartPoint(0.0, top_crosses_width, 0.0, "Cross_RL"));
        cross_piece->AddPoint(new PartPoint(bottom_width, top_crosses_width, 0.0, "Cross_RR"));
        cross_piece->AddPoint(new PartPoint(bottom_width, 0.0, 0.0, "Cross_FR"));

        //=============================================================================================
        // Label Part - Cross Piece
        //=============================================================================================
        bound = cross_piece->GetExtents();
        cross_piece->AddText(new PartPoint(bound.left() + 1.0, bound.bottom() + fabs(bound.height()) / 2.0, 0.0), "Cross Piece", 1.0);

        bound = toekick->GetExtents();
        current_offset += PartPoint(0.0, fabs(bound.height()) + 0.25, 0.0);

        inside_sheet.AddPart(cross_piece, current_offset);

        bound = cross_piece->GetExtents();
        current_offset += PartPoint(0.0, fabs(bound.height()) + 0.25, 0.0);
        inside_sheet.AddPart(cross_piece, current_offset);
    }

    inside_sheet.WriteDXF("Prj_inside.dxf");
}
