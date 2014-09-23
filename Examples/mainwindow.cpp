#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <MPlot/MPlotWidget.h>
#include <MPlot/MPlot.h>
#include <MPlot/SeriesData/MPlotVectorRefSeriesData.h>
#include <MPlot/Series/MPlotSeriesBasic.h>
#include <MPlot/MPlotTools.h>
#include <MPlot/MPlotAxisChangeMediator.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _datos.resize(7);
    unsigned int size = 4000000;
    MPlotAxisChangeMediator * mediator = new MPlotAxisChangeMediator();
    for (unsigned int i=0;i<7;i++) {
        _datos[i].resize(size);
        double offset=rand();
        for (unsigned int x=0;x<size;x++) {
            _datos[i][x] = std::abs(std::sin((x+offset)/300000.0f*(i+1)) * 55.0f);
        }





        MPlotVectorRefSeriesData<uint16_t>* plot2d = new MPlotVectorRefSeriesData<uint16_t>(_datos[i]);
        MPlotSeriesBasic * series = new MPlotSeriesBasic(plot2d);

        _plots[i]  = new MPlot();
        MPlot * plot       =_plots[i]   ;
        MPlotWidget * plotWindow = new MPlotWidget();
        plot->axisScaleLeft()->setAutoScaleEnabled();
        plot->axisScaleLeft()->setDataRangeConstraint(MPlotAxisRange(0,50));
        plot->axisScaleBottom()->setDataRangeConstraint(MPlotAxisRange(0,_datos[i].size()));
        plot->axisScaleBottom()->setAutoScaleEnabled();
        plot->setMarginTop(6);
        plot->setMarginRight(0);
        plot->setMarginLeft(5);
        plot->setMarginBottom(5);

        plotWindow->setPlot(plot);
        plot->addItem(series);

        this->ui->centralWidget->layout()->addWidget(plotWindow);

        // this tool adds mouse-wheel based zooming
        //MPlotDragZoomerTool * dzTool =new MPlotDragZoomerTool();
        //plot->addTool(dzTool);

        MPlotWheelZoomerTool * wzTool = new MPlotWheelZoomerTool();
        plot->addTool(wzTool);

//
       //  MPlotDragTool        * dragTool = new MPlotDragTool();
       //  plot->addTool(dragTool);

         mediator->addPlot(plot);

    }



}
void MainWindow::configurePlot(MPlot * plot)
{

}

MainWindow::~MainWindow()
{
    delete ui;
}
