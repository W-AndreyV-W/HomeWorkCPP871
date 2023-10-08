#include "graphicwindows.h"
#include "ui_graphicwindows.h"

GraphicWindows::GraphicWindows(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphicWindows)
{
    ui->setupUi(this);
}

GraphicWindows::~GraphicWindows() {

    delete ui;
}

QCustomPlot* GraphicWindows::get_customPlot() {

    return ui->wt_customPlot;
}

void GraphicWindows::on_pB_close_clicked() {

    GraphicWindows::close();
}

