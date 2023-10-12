#ifndef GRAPHICWINDOWS_H
#define GRAPHICWINDOWS_H

#include <QWidget>

#include "qcustomplot.h"

namespace Ui {
class GraphicWindows;
}

class GraphicWindows : public QWidget {

    Q_OBJECT

public:

    explicit GraphicWindows(QWidget *parent = nullptr);
    ~GraphicWindows();

    void printGraphicWindows(const QVector<double>& graphic_x, const QVector<double>& graphic_y);
    void clearGraphicWindows();

private slots:

    void on_pB_close_clicked();

private:

    Ui::GraphicWindows *ui;

    QCPGraph* graph;
};

#endif // GRAPHICWINDOWS_H
