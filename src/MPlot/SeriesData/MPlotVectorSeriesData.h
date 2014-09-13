#ifndef MPLOTVECTORSERIESDATA_H
#define MPLOTVECTORSERIESDATA_H
#include <QObject>
#include <QVector>
#include "../MPlot_global.h"
#include "MPlotAbstractSeriesData.h"

/// This is a simple example implementation of MPlotAbstractSeriesData that uses a QVector<qreal> to represent the X and Y point values.  You can use it directly if you want to draw a simple plot and don't want to implement your own data model.
class MPLOTSHARED_EXPORT MPlotVectorSeriesData : public MPlotAbstractSeriesData {

public:
    /// Constructs an empty data model
    MPlotVectorSeriesData();

    /// Implements MPlotAbstractSeriesData: returns the x value at \c index.
    virtual qreal x(unsigned index) const { return xValues_.at(index); }
    /// Copy all the x values from \c indexStart to \c indexEnd (inclusive) into \c outputValues. You can assume that the indexes are valid.
    virtual void xValues(unsigned indexStart, unsigned indexEnd, qreal *outputValues) const { memcpy(outputValues, xValues_.constData()+indexStart, (indexEnd-indexStart+1)*sizeof(qreal)); }
    /// Implements MPlotAbstractSeriesData: returns the y value at \c index.
    virtual qreal y(unsigned index) const { return yValues_.at(index); }
    /// Copy all the y values from \c indexStart to \c indexEnd (inclusive) into \c outputValues.  You can assume that the indexes are valid.
    virtual void yValues(unsigned indexStart, unsigned indexEnd, qreal* outputValues) const { memcpy(outputValues, yValues_.constData()+indexStart, (indexEnd-indexStart+1)*sizeof(qreal)); }

    /// Implements MPlotAbstractSeriesData: returns the number of data points.
    virtual int count() const { return xValues_.count(); }


    /// Set the X and Y values. \c xValues and \c yValues must have the same size(); if not, this does nothing and returns false.
    bool setValues(const QVector<qreal>& xValues, const QVector<qreal>& yValues);
    /// Set a specfic X value. \c index must be in range for the current data, otherwise does nothing and returns false.
    bool setXValue(int index, qreal xValue);
    /// Set a specific Y value. \c index must be in range for the current data, otherwise does nothing and returns false.
    bool setYValue(int index, qreal yValue);



protected:
    QVector<qreal> xValues_;
    QVector<qreal> yValues_;
};
#endif // MPLOTVECTORSERIESDATA_H
