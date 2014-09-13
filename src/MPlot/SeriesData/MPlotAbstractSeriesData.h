#ifndef MPLOTABSTRACTSERIESDATA_H
#define MPLOTABSTRACTSERIESDATA_H
#include "../MPlot_global.h"
#include <QObject>
#include <QRectF>
class MPlotAbstractSeriesData;


/// This class acts as a proxy to emit signals for MPlotAbstractSeriesData. You can receive the dataChanged() signal by hooking up to MPlotAbstractSeries::signalSource().
/*! To allow classes that implement MPlotAbstractSeriesData to also inherit QObject, MPlotAbstractSeriesData does NOT inherit QObject.  However, it still needs a way to emit signals notifying of changes to the data, which is the role of this class.

  Implementations must do two things:
  1) Implement the virtual functions x(), y(), and count()
  2) Call emitDataChanged() whenever the count() or x/y values have changed.
  */
class MPLOTSHARED_EXPORT MPlotSeriesDataSignalSource : public QObject {
    Q_OBJECT
public:
    MPlotAbstractSeriesData& seriesData() const { return data_; }
protected:
    MPlotSeriesDataSignalSource(MPlotAbstractSeriesData& parent);
    void emitDataChanged() { emit dataChanged(); }

    MPlotAbstractSeriesData& data_;
    friend class MPlotAbstractSeriesData;

signals:
    void dataChanged();
};

/// This defines the interface for classes which may be used for Series (XY scatter) plot data.
class MPLOTSHARED_EXPORT MPlotAbstractSeriesData  {

public:
    MPlotAbstractSeriesData();
    virtual ~MPlotAbstractSeriesData();

    /// Use this object to receive signals from the data when the data has changed in any way (ie: new points, deleted points, or values changed)
    MPlotSeriesDataSignalSource* signalSource() const { return signalSource_; }

    /// Return the x-value at index.  You can assume that \c index is valid (< count()).
    virtual qreal x(unsigned index) const = 0;
    /// Copy all the x values from \c indexStart to \c indexEnd (inclusive) into \c outputValues. You can assume that the indexes are valid.
    virtual void xyValues(unsigned indexStart,
                          unsigned indexEnd,
                          QVector<qreal>& outputValuesX,
                          QVector<qreal>&outputValuesY) const = 0;

    /// Return the y-value at index.  You can assume that \c index is valid (< count()).
    virtual qreal y(unsigned index) const = 0;


    /// Return the number of data points.
    virtual int count() const = 0;

    /// Return the bounds of the data (the rectangle containing the max/min x- and y-values). It should be expressed as: QRectF(left, top, width, height) = QRectF(minX, minY, maxX-minX, maxY-minY);
    /*! \todo Should we change this so that the QRectF's "top()" is actually maxY instead of minY?

The base class implementation does a linear search through the data for the maximum and minimum values. It caches the result, and invalidates this result whenever the data changes (ie: emitDataChanged() is called). If you have a faster way of determining the bounds of the data, be sure to re-implement this. */
    virtual QRectF boundingRect() const;

private:
    MPlotSeriesDataSignalSource* signalSource_;
    friend class MPlotSeriesDataSignalSource;

protected:
    /// Implementing classes should call this when their x- y- data changes in any way (ie: points added, points removed, or even values changed such that the bounds of the plot might be different.)
    void emitDataChanged() { cachedDataRectUpdateRequired_ = true; signalSource_->emitDataChanged(); }

protected:
    /// Implements caching for the search-based version of boundingRect().
    mutable QRectF cachedDataRect_;
    /// Implements caching for the search-based version of boundingRect().
    mutable bool cachedDataRectUpdateRequired_;
    /// Search for minimum Y value. Call only when count() > 0.
    virtual qreal searchMinY() const = 0;
    /// Search for extreme value. Call only when count() > 0.
    virtual qreal searchMaxY() const = 0;
    /// Search for extreme value. Call only when count() > 0.
    virtual qreal searchMinX() const = 0;
    /// Search for extreme value. Call only when count() > 0.
    virtual qreal searchMaxX() const = 0;



    // todo: to support multi-threading, consider a
    // void pauseUpdates();	// to tell nothing to redraw using the plot because the data is currently invalid; a dataChanged will be emitted when it is valid again.
                            // This would need to be deterministic, so maybe we need to use function calls instead of signals.
};


#endif // MPLOTABSTRACTSERIESDATA_H
