#include "MPlotVectorSeriesData.h"

bool MPlotVectorSeriesData::setValues(const QVector<qreal> &xValues, const QVector<qreal> &yValues)
{
    if(xValues.count() != yValues.count())
        return false;

    xValues_ = xValues;
    yValues_ = yValues;
    emitDataChanged();
    return true;
}

bool MPlotVectorSeriesData::setXValue(int index, qreal xValue)
{
    if((unsigned)index >= (unsigned)xValues_.count())
        return false;

    xValues_[index] = xValue;
    emitDataChanged();
    return true;
}

bool MPlotVectorSeriesData::setYValue(int index, qreal yValue)
{
    if((unsigned)index >= (unsigned)yValues_.count())
        return false;

    yValues_[index] = yValue;
    emitDataChanged();
    return true;
}

MPlotVectorSeriesData::MPlotVectorSeriesData()
    : MPlotAbstractSeriesData()
{
}

