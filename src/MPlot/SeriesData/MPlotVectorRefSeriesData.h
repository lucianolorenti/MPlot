#ifndef MPLOTVECTORREFSERIESDATA_H
#define MPLOTVECTORREFSERIESDATA_H
#include "../MPlot_global.h"
#include <QVector>
#include "MPlotAbstractSeriesData.h"

template <class T>
class MPLOTSHARED_EXPORT MPlotVectorRefSeriesData : public MPlotAbstractSeriesData {

protected:
    const QVector<T> & _yValues;

    // MPlotAbstractSeriesData interface
public:
    MPlotVectorRefSeriesData(const QVector<T> & y);
    qreal x(unsigned index) const;
    void xyValues(unsigned indexStart,
                  unsigned indexEnd,
                  QVector<qreal> &outputValuesX, QVector<qreal> &outputValuesY) const;
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
    unsigned step = ((indexEnd-indexStart+1)/((double)outputValuesX.size()-1)+1);
    for (unsigned int i=indexStart;i<=indexEnd;i+=step) {
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
    return *std::min_element(_yValues.begin(),_yValues.end());
}
template <class T>
qreal MPlotVectorRefSeriesData<T>::searchMaxY() const
{
    return *std::max_element(_yValues.begin(),_yValues.end());
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
#endif // MPLOTVECTORREFSERIESDATA_H
