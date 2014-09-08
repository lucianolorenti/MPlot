#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <MPlot/MPlotWidget.h>
#include <MPlot/MPlot.h>
#include <MPlot/MPlotSeriesData.h>
#include <MPlot/MPlotSeries.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _datos.resize(7);
    unsigned int size = 4000000;
    for (unsigned int i=0;i<7;i++) {
        _datos[i].resize(size);
        for (unsigned int x=0;x<size;x++) {
            _datos[i][x] = std::abs(std::sin(x/5.0f) * 55.0f);
        }





        MPlotVectorRefSeriesData<uint16_t>* plot2d = new MPlotVectorRefSeriesData<uint16_t>(_datos[i]);
        MPlotSeriesBasic * series = new MPlotSeriesBasic(plot2d);

        MPlot       * plot       = new MPlot();
        MPlotWidget * plotWindow = new MPlotWidget();
        plot->axisScaleLeft()->setAutoScaleEnabled();
        plot->axisScaleBottom()->setAutoScaleEnabled();
        plotWindow->setPlot(plot);
        plot->addItem(series);

        this->ui->centralWidget->layout()->addWidget(plotWindow);
    }



}
void MainWindow::configurePlot(MPlot * plot)
{

}

MainWindow::~MainWindow()
{
    delete ui;
}
