#ifndef __MPlotSeriesData_H__
#define __MPlotSeriesData_H__

#include "MPlot/MPlot_global.h"

#include <QAbstractTableModel>
#include <QQueue>
#include <QList>
#include <QRectF>
#include <QVector>

#include <limits>

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
	MPlotAbstractSeriesData* seriesData() const { return data_; }
protected:
	MPlotSeriesDataSignalSource(MPlotAbstractSeriesData* parent);
	void emitDataChanged() { emit dataChanged(); }

	MPlotAbstractSeriesData* data_;
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
    virtual void xyValues(unsigned indexStart, unsigned indexEnd, QVector<qreal>& outputValuesX,QVector<qreal>&outputValuesY) const = 0;

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

template <class T>
class MPLOTSHARED_EXPORT MPlotVectorRefSeriesData : public MPlotAbstractSeriesData {

protected:
    const QVector<T> & _yValues;

    // MPlotAbstractSeriesData interface
public:
    MPlotVectorRefSeriesData(const QVector<T> & y);
    qreal x(unsigned index) const;
    void xyValues(unsigned indexStart, unsigned indexEnd, QVector<qreal> &outputValuesX, QVector<qreal> &outputValuesY) const;
    qreal y(unsigned index) const;
    int count() const;

protected:
    qreal searchMinY() const;
    qreal searchMaxY() const;
    qreal searchMinX() const;
    qreal searchMaxX() const;
};
template <class T>
MPlotVectorRefSeriesData<T>::MPlotVectorRefSeriesData(const QVector<T> & y) :
    _yValues(y)
{

}
template <class T>
qreal MPlotVectorRefSeriesData<T>::x(unsigned index) const
{
    return index;
}
template <class T>
void MPlotVectorRefSeriesData<T>::xyValues(unsigned indexStart,
                                           unsigned indexEnd,
                                           QVector<qreal> &outputValuesX,
                                           QVector<qreal> &outputValuesY) const
{
    unsigned int idx = 0;
    for (unsigned int i=indexStart;i<=indexEnd;i++) {
        outputValuesX[idx] = i;
        outputValuesY[idx] = _yValues[i];
        idx++;
    }
}
template <class T>
qreal MPlotVectorRefSeriesData<T>::y(unsigned index) const
{
    return _yValues[index];
}
template <class T>
int MPlotVectorRefSeriesData<T>::count() const
{
    return _yValues.size();
}
template <class T>
qreal MPlotVectorRefSeriesData<T>::searchMinY() const
{
    return *std::min(_yValues.begin(),_yValues.end());
}
template <class T>
qreal MPlotVectorRefSeriesData<T>::searchMaxY() const
{
    return *std::max(_yValues.begin(),_yValues.end());
}
template <class T>
qreal MPlotVectorRefSeriesData<T>::searchMinX() const
{
    return 0;
}
template <class T>
qreal MPlotVectorRefSeriesData<T>::searchMaxX() const
{
    return _yValues.count();
}

/// This class provides a Qt TableModel implementation of XY data.  It is optimized for fast storage of real-time data.
/*! It provides fast (usually constant-time) lookups of the min and max values for each axis, which is important for plotting so that
	// boundingRect() and autoscaling calls run quickly.

When using for real-time data, calling insertPointFront and insertPointBack is very fast.
  */
class MPLOTSHARED_EXPORT MPlotRealtimeModel : public QAbstractTableModel, public MPlotAbstractSeriesData {

	Q_OBJECT

public:
	MPlotRealtimeModel(QObject *parent = 0);

	int rowCount(const QModelIndex & /*parent*/) const;
	virtual int count() const;
	int columnCount(const QModelIndex & /*parent*/) const;

	virtual qreal x(unsigned index) const;
    virtual qreal y(unsigned index) const;
    virtual void xyValues(unsigned indexStart, unsigned indexEnd, QVector<qreal> &outputValuesX,QVector<qreal>&outputValuesY) const;




	QVariant data(const QModelIndex &index, int role) const;

	QVariant headerData(int section, Qt::Orientation orientation, int role) const;

	bool setData(const QModelIndex &index, const QVariant &value, int role);

	// This allows editing of values within range (for ex: in a QTableView)
	Qt::ItemFlags flags(const QModelIndex &index) const;

	// This allows you to add data points at the beginning:
	void insertPointFront(qreal x, qreal y);

	// This allows you to add data points at the end:
	void insertPointBack(qreal x, qreal y);

	// Remove a point at the front (Returns true if successful).
	bool removePointFront();

	// Remove a point at the back (returns true if successful)
	bool removePointBack();

	virtual QRectF boundingRect() const;

	// TODO: add properties: set and read axis names

protected:

	// Members: Data arrays:
	QQueue<qreal> xval_;
	QQueue<qreal> yval_;

	// Max/min index tracking:
	int minYIndex_, maxYIndex_, minXIndex_, maxXIndex_;

	//
	QString xName_, yName_;


	// Helper functions:
	// Check if an added point @ index is the new min. or max record holder:
	// Must call this AFTER adding both x and y to the xval_ and y_val lists.
	void minMaxAddCheck(qreal x, qreal y, int index);

	// Check if a point modified at index causes us to lose our record holders, or is a new record holder.
	// Inserts the point (modifies the data array).
	void minMaxChangeCheckX(qreal newVal, int index);
	void minMaxChangeCheckY(qreal newVal, int index);

	int searchMaxIndex(const QList<qreal>& list);

	int searchMinIndex(const QList<qreal>& list);

	// Warning: only call these if the list is not empty:
	qreal minY() const;
	qreal maxY() const;
	qreal minX() const;
	qreal maxX() const;


    // MPlotAbstractSeriesData interface
protected:
    qreal searchMinY() const;
    qreal searchMaxY() const;
    qreal searchMinX() const;
    qreal searchMaxX() const;
};





#endif
