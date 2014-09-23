#ifndef MPLOTAXISCHANGEMEDIATOR_H
#define MPLOTAXISCHANGEMEDIATOR_H

#include <QObject>
#include "MPlotAxisRange.h"
#include "MPlot.h"

class MPlotAxisChangeMediator : public QObject
{
    Q_OBJECT
public:
    MPlotAxisChangeMediator(QObject*parent = 0);
    virtual ~MPlotAxisChangeMediator() {}
    virtual void addPlot(MPlot*p);
public slots:
    void leftDataRangeChange(const MPlotAxisRange & r);
    void bottomDataRangeChange(const MPlotAxisRange & r);
private:
    QVector<MPlot*> _plots;
    bool _updating;
};

#endif // MPLOTAXISCHANGEMEDIATOR_H
