#include "MPlotAbstractSeriesData.h"

MPlotSeriesDataSignalSource::MPlotSeriesDataSignalSource(MPlotAbstractSeriesData& parent)
    : QObject(0),
      data_(parent)
{

}

MPlotAbstractSeriesData::MPlotAbstractSeriesData()
{
    signalSource_ = new MPlotSeriesDataSignalSource(*this);
    cachedDataRectUpdateRequired_ = true;
}

MPlotAbstractSeriesData::~MPlotAbstractSeriesData()
{
    delete signalSource_;
    signalSource_ = 0;
}

#include <QDebug>
#include <QTime>
QRectF MPlotAbstractSeriesData::boundingRect() const {
    if(count() == 0)
        return QRectF();

    if(cachedDataRectUpdateRequired_) {

        qreal minY = searchMinY();
        qreal maxY = searchMaxY();
        qreal minX = searchMinX();
        qreal maxX = searchMaxX();

        cachedDataRect_ = QRectF(minX,
                                 minY,
                                 qMax(maxX-minX, std::numeric_limits<qreal>::min()),
                                 qMax(maxY-minY, std::numeric_limits<qreal>::min()));	// this is to make sure that if we have a single data point (ie: minY=maxY, minX=maxX) there is still a valid boundingRect() for the data.  A QRectF with a width and height of exactly 0 would be indentified as invalid; this makes it valid, without making it much bigger in size.  [min() is the smallest positive number for qreals.]
        cachedDataRectUpdateRequired_ = false;
    }

    return cachedDataRect_;
}
