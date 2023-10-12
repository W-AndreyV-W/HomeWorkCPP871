#include "graphicwindows.h"
#include "ui_graphicwindows.h"

GraphicWindows::GraphicWindows(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphicWindows)
{
    ui->setupUi(this);

    graph = new QCPGraph(ui->wt_customPlot->xAxis2, ui->wt_customPlot->yAxis2);

    ui->wt_customPlot->setInteraction(QCP::iRangeZoom, true);
    ui->wt_customPlot->setInteraction(QCP::iRangeDrag, true);
}

GraphicWindows::~GraphicWindows() {

    delete ui;
}

void GraphicWindows::printGraphicWindows(const QVector<double>& graphic_x, const QVector<double>& graphic_y) {

    ui->wt_customPlot->addGraph();

    graph->setData(graphic_x, graphic_y, true);

    ui->wt_customPlot->rescaleAxes();
    ui->wt_customPlot->replot();
}

void GraphicWindows::clearGraphicWindows() {

    for(int i = 0; i < ui->wt_customPlot->graphCount(); i++) {

        ui->wt_customPlot->graph(i)->data()->clear();
    }
}

void GraphicWindows::on_pB_close_clicked() {

    GraphicWindows::close();
}
