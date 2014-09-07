#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <MPlotWidget.h>
#include <MPlot.h>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QVector<QVector<uint16_t>> datos;
    datos.resize(7);
    for (unsigned int i=0;i<7;i++) {
        for (unsigned int x=0;x<4000000;x++) {
            datos[i][x] = std::sin(x/500.0f) * 55;
        }
        MPlotWidget * plotWindow = new MPlotWidget();
        MPlot       * plot       = new MPlot();
        plotWindow->setPlot(plot);
    }



}
void MainWindow::configurePlot(MPlot * plot)
{

}

MainWindow::~MainWindow()
{
    delete ui;
}
