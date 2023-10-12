#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDataStream>
#include <QMessageBox>
//#include <QtGlobal>
#include <QtConcurrent>
#include <algorithm>
#include <QCloseEvent>

//#include <QLineSeries>
//#include <QtCharts>
//#include <QChartView>

#include "graphicwindows.h"


#define FD 1000.0 //частота дискретизации

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QVector<uint32_t> ReadFile(QString path, uint8_t numberChannel);
    QVector<double> ProcessFile(QVector<uint32_t> dataFile);
    QVector<double> FindMax(QVector<double> resultData);
    QVector<double> FindMin(QVector<double> resultData);
    void DisplayResult(QVector<double> mins, QVector<double> maxs);

signals:

    void sig_printGraphic();

private slots:

    void on_pb_path_clicked();
    void on_pb_start_clicked();
    void printGraphic();

private:

    Ui::MainWindow *ui;

    GraphicWindows* customPlot = nullptr;

    QString pathToFile = "";
    uint8_t numberSelectChannel = 0xEA;
    double time_out = 1;
    double step = 0.1;

    QVector<uint32_t> readData;
    QVector<double> procesData;
    QVector<double> mins;
    QVector<double> maxs;
    QVector<double> graphic_x;
    QVector<double> graphic_y;

    void closeEvent (QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
