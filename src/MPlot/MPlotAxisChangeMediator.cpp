#include "MPlotAxisChangeMediator.h"

MPlotAxisChangeMediator::MPlotAxisChangeMediator(QObject*parent ) :
    QObject(parent),
    _updating(false)
{

}

void MPlotAxisChangeMediator::addPlot(MPlot *p)
{
    _plots.append(p);
    this->connect(p->axisScaleLeft(),SIGNAL(dataRangeChanged(MPlotAxisRange)),
                  this,SLOT(leftDataRangeChange(MPlotAxisRange)));
    this->connect(p->axisScaleBottom(),SIGNAL(dataRangeChanged(MPlotAxisRange)),
                  this,SLOT(bottomDataRangeChange(MPlotAxisRange)));
}
void MPlotAxisChangeMediator::leftDataRangeChange(const MPlotAxisRange & r)
{
    if (!_updating) {
        _updating=true;
        MPlot * o = reinterpret_cast<MPlot*>(this->sender());
        for (MPlot* p : _plots) {
            if (p!=o) {
                p->axisScaleLeft()->setDataRange(r,false);
            }
        }
        _updating=false;
    }

}
void MPlotAxisChangeMediator::bottomDataRangeChange(const MPlotAxisRange & r)
{
    if (!_updating) {
        _updating=true;
        MPlot * o = reinterpret_cast<MPlot*>(this->sender());
        for (MPlot* p : _plots) {
            if (p!=o) {
                p->axisScaleBottom()->setDataRange(r,false);
            }
        }
        _updating=false;
    }

}
