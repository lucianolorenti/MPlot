#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <cstdint>
#include <MPlot/MPlot.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void configurePlot(MPlot *plot);
    QVector<QVector<uint16_t>> _datos;
    MPlot *_plots[7];
};


#endif // MAINWINDOW_H
