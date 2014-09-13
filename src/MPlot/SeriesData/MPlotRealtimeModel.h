#ifndef MPLOTREALTIMEMODEL_H
#define MPLOTREALTIMEMODEL_H
#include "../MPlot_global.h"
#include <QAbstractTableModel>
#include "MPlotAbstractSeriesData.h"
#include <QQueue>
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
    virtual void xyValues(unsigned indexStart, unsigned indexEnd,  QVector<qreal> &outputValuesX, QVector<qreal>&outputValuesY) const;




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

#endif // MPLOTREALTIMEMODEL_H
