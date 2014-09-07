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
    for (unsigned int i=0;i<7;i++) {
        _datos[i].resize(4000000);
        for (unsigned int x=0;x<4000000;x++) {
            _datos[i][x] = std::sin(x/500.0f) * 55;
        }





        MPlotVectorRefSeriesData<uint16_t>* plot2d = new MPlotVectorRefSeriesData<uint16_t>(_datos[i]);
        MPlotSeriesBasic * series = new MPlotSeriesBasic(plot2d);

        MPlot       * plot       = new MPlot();
        MPlotWidget * plotWindow = new MPlotWidget();
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
