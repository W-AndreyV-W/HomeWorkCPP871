#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pb_clearResult->setCheckable(true);

    QObject::connect(this, &MainWindow::sig_printGraphic, this, &MainWindow::printGraphic);
}

MainWindow::~MainWindow() {

    if(customPlot != nullptr) {

        customPlot->close();
    }

    delete ui;
    delete customPlot;
    delete graph;
}

QVector<uint32_t> MainWindow::ReadFile(QString path, uint8_t numberChannel) {

    QFile file(path);
    file.open(QIODevice::ReadOnly);

    if(file.isOpen() == false) {

        QMessageBox mb;
        mb.setWindowTitle("Ошибка");
        mb.setText("Ошибка открытия фала");
        mb.exec();
    }

    QDataStream dataStream;
    dataStream.setDevice(&file);
    dataStream.setByteOrder(QDataStream::LittleEndian);

    QVector<uint32_t> readData;
    uint32_t currentWorld = 0;
    uint32_t sizeFrame = 0;

    readData.clear();

    while(dataStream.atEnd() == false) {

        dataStream >> currentWorld;

        if(currentWorld == 0xFFFFFFFF) {

            dataStream >> currentWorld;

            if(currentWorld < 0x80000000) {

                dataStream >> sizeFrame;

                if(sizeFrame > 1500) {

                    continue;
                }

                for(int i = 0; i < sizeFrame / sizeof(uint32_t); i++) {

                    dataStream >> currentWorld;

                    if((currentWorld >> 24) == numberChannel) {

                        readData.append(currentWorld);
                    }
                }
            }
        }
    }

    ui->chB_readSucces->setChecked(true);

    return readData;
}

QVector<double> MainWindow::ProcessFile(const QVector<uint32_t> dataFile) {

    QVector<double> resultData;
    resultData.clear();

    foreach (int word, dataFile) {

        word &= 0x00FFFFFF;

        if(word > 0x800000) {

            word -= 0x1000000;
        }

        double res = ((double)word/6000000)*10;

        resultData.append(res);
    }

    ui->chB_procFileSucces->setChecked(true);

    return resultData;
}

QVector<double> MainWindow::FindMax(QVector<double> resultData)
{
    double max = 0;
    double sMax=0;

    foreach (double num, resultData) {

        if(num > max) {

            max = num;
        }
    }

    foreach (double num, resultData) {

        if(num > sMax && (qFuzzyCompare(num, max) == false)) {

            sMax = num;
        }
    }

    QVector<double> maxs = {max, sMax};

    ui->chB_maxSucess->setChecked(true);

    return maxs;
}

QVector<double> MainWindow::FindMin(QVector<double> resultData) {

    double min = 0;
    double sMin = 0;

    foreach (double num, resultData) {

        if(num < min) {

            min = num;
        }
    }

    foreach (double num, resultData) {

        if(num < sMin && (qFuzzyCompare(num, min) == false)) {

            sMin = num;
        }
    }

    QVector<double> mins = {min, sMin};

    ui->chB_minSucess->setChecked(true);

    return mins;
}

void MainWindow::DisplayResult(QVector<double> mins, QVector<double> maxs) {

    ui->te_Result->append("Расчет закончен!");

    ui->te_Result->append("Первый минимум " + QString::number(mins.first()));
    ui->te_Result->append("Второй минимум " + QString::number(mins.at(1)));

    ui->te_Result->append("Первый максимум " + QString::number(maxs.first()));
    ui->te_Result->append("Второй максимум " + QString::number(maxs.at(1)));
}


void MainWindow::on_pb_path_clicked() {

    pathToFile = "";

    ui->le_path->clear();

    pathToFile =  QFileDialog::getOpenFileName(this, tr("Открыть файл"), "/home/", tr("ADC Files (*.adc)"));

    ui->le_path->setText(pathToFile);
}

void MainWindow::on_pb_start_clicked() {

    if(pathToFile.isEmpty()) {

        QMessageBox mb;
        mb.setWindowTitle("Ошибка");
        mb.setText("Выберите файл!");
        mb.exec();

        return;
    }

    ui->chB_maxSucess->setChecked(false);
    ui->chB_procFileSucces->setChecked(false);
    ui->chB_readSucces->setChecked(false);
    ui->chB_minSucess->setChecked(false);

    int selectIndex = ui->cmB_numCh->currentIndex();

    if(selectIndex == 0) {

        numberSelectChannel = 0xEA;
    }
    else if(selectIndex == 1) {

        numberSelectChannel = 0xEF;
    }
    else if(selectIndex == 2) {

        numberSelectChannel = 0xED;
    }

    auto read = [&] () {

        return ReadFile(pathToFile, numberSelectChannel);
    };

    auto process = [&] (QVector<uint32_t> res) {

        return ProcessFile(res);
    };

    auto findMax = [&] (QVector<double> res) {

        uint size_arr = static_cast<uint>(time_out) * FD;

        maxs = FindMax(res);
        mins = FindMin(res);

        DisplayResult(mins, maxs);

        graphic_x.resize(size_arr, 0);
        graphic_y.resize(size_arr, 0);

        for(int i = 0; i < size_arr; i++) {

            graphic_x[i] = i * step;

            if((res.begin() + i) != res.end()) {

                graphic_y[i] = res[i];
            }
        }

        emit sig_printGraphic();
    };

    auto result = QtConcurrent::run(read).then(process).then(findMax);
}

void MainWindow::printGraphic() {

    if(customPlot == nullptr) {

        customPlot = new GraphicWindows;
        graph = new QCPGraph(customPlot->get_customPlot()->xAxis,customPlot->get_customPlot()->yAxis);

        customPlot->get_customPlot()->setInteraction(QCP::iRangeZoom, true);
        customPlot->get_customPlot()->setInteraction(QCP::iRangeDrag, true);
    }
    else {

        for(int i = 0; i < customPlot->get_customPlot()->graphCount(); i++) {

            customPlot->get_customPlot()->graph(i)->data()->clear();
        }
    }

    graph->addData(graphic_x, graphic_y);

    customPlot->show();

    customPlot->get_customPlot()->rescaleAxes();
    customPlot->get_customPlot()->replot();
}

void MainWindow::closeEvent(QCloseEvent *event) {

    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Close",
        tr("Are you sure?\n"), QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
        QMessageBox::Yes);

    if (resBtn != QMessageBox::Yes) {

        event->ignore();
    }
    else {

        MainWindow::~MainWindow();
        event->accept();
    }
}
