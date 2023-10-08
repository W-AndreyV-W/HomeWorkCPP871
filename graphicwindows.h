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
    QCustomPlot* get_customPlot();

private slots:

    void on_pB_close_clicked();

private:

    Ui::GraphicWindows *ui;
};

#endif // GRAPHICWINDOWS_H
