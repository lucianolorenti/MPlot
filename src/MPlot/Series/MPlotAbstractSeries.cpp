#include "MPlotAbstractSeries.h"


MPlotSeriesSignalHandler::MPlotSeriesSignalHandler(MPlotAbstractSeries &parent)
    : QObject(0),
      series_(parent){

}

void MPlotSeriesSignalHandler::onDataChanged() {
    series_.onDataChangedPrivate();
}

MPlotAbstractSeries::MPlotAbstractSeries() :
    MPlotItem()
{
    data_ = 0;
    marker_ = 0;
    dataChangedUpdateNeeded_ = true;

    /// Scale and shift factors
    sx_ = sy_ = 1.0;
    dx_ = dy_ = 0.0;
    offset_ = QPointF(0.0,0.0);

    /// Indicates whether normalization is on:
    yAxisNormalizationOn_ = xAxisNormalizationOn_ = false;

    /// Normalization ranges:
    normYMin_ = normXMin_ = 0.0;
    normYMax_ = normXMax_ = 1.0;

    signalHandler_ = new MPlotSeriesSignalHandler(*this);

}

MPlotAbstractSeries::~MPlotAbstractSeries() {
    // If we have a model, need to disconnect from its updates before we get deleted.
    if(data_) {
        QObject::disconnect(data_->signalSource(), 0, signalHandler_, 0);
        if(ownsModel_) {
            delete data_;
            data_ = 0;
        }
    }
    delete signalHandler_;
    signalHandler_ = 0;
}

// Properties:
void MPlotAbstractSeries::setLinePen(const QPen& pen) {
    linePen_ = pen;
    emitLegendContentChanged(); // this changes the legendColor();
    update();
}

// Returns the current marker, which can be used to access it's pen, brush, and size.
// If the plot has no marker (or MPlotMarkerShape::None), then this will be a null pointer. Must check before setting.
MPlotAbstractMarker* MPlotAbstractSeries::marker() const {
    return marker_;
}

void MPlotAbstractSeries::setMarker(MPlotMarkerShape::Shape shape, qreal size, const QPen& pen, const QBrush& brush) {
    if(marker_)
        delete marker_;

    marker_ = MPlotMarker::create(shape, size, pen, brush);
    update();
}

// Sets this series to view the model in 'data';
void MPlotAbstractSeries::setModel(const MPlotAbstractSeriesData* data, bool ownsModel) {

    // efficiency check: if new model is the same one as old model, don't change anything.
    if(data == data_) {
        ownsModel_ = ownsModel;
        return;
    }

    // Changing models.

    // If there was an old model, disconnect old signals.  Delete the old model if ownsModel_ was set.
    if(data_) {
        QObject::disconnect(data_->signalSource(), 0, signalHandler_, 0);
        if(ownsModel_)
            delete data_;
    }

    // new data from here:
    data_ = data;
    ownsModel_ = ownsModel;

    dataChangedUpdateNeeded_ = true;
    prepareGeometryChange();

    // If there's a new valid model:
    if(data_) {
        QObject::connect(data_->signalSource(), SIGNAL(dataChanged()), signalHandler_, SLOT(onDataChanged()));
    }

    emitBoundsChanged();
    onDataChanged();

}

const MPlotAbstractSeriesData* MPlotAbstractSeries::model() const { return data_; }

void MPlotAbstractSeries::xxyyValues(unsigned start,
                                     unsigned end,
                                     QVector<qreal> &outputValuesX,
                                     QVector<qreal> &outputValuesY) const
{
    qreal offset = offset_.x();


    data_->xyValues(start, end, outputValuesX,outputValuesY);


    for (int i = 0; i < outputValuesX.size(); i++) {
        outputValuesX[i] = outputValuesX[i]*sx_ + dx_ + offset;
        outputValuesY[i] = outputValuesY[i]*sy_ + dy_ + offset;
    }
}



// Required functions:
//////////////////////////

// Data rect: also reported in our PlotSeries coordinates, which are the actual data coordinates. This is used by the auto-scaling to figure out the range of our data on an axis.
QRectF MPlotAbstractSeries::dataRect() const {
    // this indicates that the raw data's bounding rectangle has changed, and we might need to calculate new normalizations.
    if(dataChangedUpdateNeeded_) {
        if(data_) {
            cachedDataRect_ = data_->boundingRect();

            if(yAxisNormalizationOn_) {
                //				sy_ = (normYMax_ - normYMin_)/(qMax(MPLOT_MIN_NORMALIZATION_RANGE, cachedDataRect_.height()));
                sy_ = (normYMax_ - normYMin_)/(qMax(MPLOT_MIN_NORMALIZATION_RANGE, cachedDataRect_.height()) == MPLOT_MIN_NORMALIZATION_RANGE ? 1.0 : cachedDataRect_.height());

                if (sy_ == 0)
                    sy_ = 1.0; // Because there is no practical reason to have a scale of 0.

                dy_ = normYMin_ - cachedDataRect_.top()*sy_;
            }

            if(xAxisNormalizationOn_) {
                //				sx_ = (normXMax_ - normXMin_)/(qMax(MPLOT_MIN_NORMALIZATION_RANGE, cachedDataRect_.width()));
                sx_ = (normXMax_ - normXMin_)/(qMax(MPLOT_MIN_NORMALIZATION_RANGE, cachedDataRect_.width()) == MPLOT_MIN_NORMALIZATION_RANGE ? 1.0 : cachedDataRect_.width());

                if (sx_ == 0)
                    sx_ = 1.0; // Because there is no practical reason to have a scale of 0.

                dx_ = normXMin_ - cachedDataRect_.left()*sx_;
            }
            cachedDataRect_ = completeTransform().mapRect(cachedDataRect_);

        }
        else {
            cachedDataRect_ = QRectF();
        }

        dataChangedUpdateNeeded_ = false;
    }

    // in all other cases, simply return this cached bounding rectangle for performance
    return cachedDataRect_;
}

QPainterPath MPlotAbstractSeries::shape() const {

    QPainterPath shape;

    // If there's under 1000 points, we can return a detailed shape with ok performance.
    // Above 1000 points, let's just return the bounding box.
    if(data_ && data_->count() > MPLOT_EXACTSHAPE_POINT_LIMIT)
        shape.addRect(boundingRect());


    else if (data_ && data_->count() > 0){

        int dataCount = data_->count();

        QVector<qreal> x = QVector<qreal>(dataCount);
        QVector<qreal> y = QVector<qreal>(dataCount);

        xxyyValues(0, dataCount-1, x,y);


        QVector<qreal> mappedX = QVector<qreal>(dataCount);
        QVector<qreal> mappedY = QVector<qreal>(dataCount);

        mapXValues(mappedX.size(), x.constData(), mappedX.data());
        mapYValues(mappedY.size(), y.constData(), mappedY.data());

        shape.moveTo(mappedX.at(0), mappedY.at(0));

        for (int i = 0, count = data_->count(); i < count; i++)
            shape.lineTo(mappedX.at(i), mappedY.at(i));

        for (int i = data_->count()-2; i >= 0; i--)
            shape.lineTo(mappedX.at(i), mappedY.at(i));

        shape.moveTo(mappedX.at(0), mappedY.at(0));
    }

    return shape;
}

void	MPlotAbstractSeries::onDataChangedPrivate() {
    // flag cached bounding rect as dirty:
    dataChangedUpdateNeeded_ = true;
    // warn that bounding rect is going to change:
    prepareGeometryChange();
    // Our shape has probably changed, so the plot might need a re-autoscale
    emitBoundsChanged();

    // call any base-class specific re-drawing
    onDataChanged();
}

void MPlotAbstractSeries::setDefaults() {

    setLinePen(QPen(QColor(Qt::red)));	// Red solid lines on plot

    setMarker(MPlotMarkerShape::Square, 6, QPen(QColor(Qt::blue), 0), QBrush()); // Blue outlines on markers, No Brush


    QColor selectionColor = MPLOT_SELECTION_COLOR;
    selectionColor.setAlphaF(MPLOT_SELECTION_OPACITY);
    selectedPen_ = QPen(QBrush(selectionColor), MPLOT_SELECTION_LINEWIDTH);
}


// Use this function to apply a constant transformation to the series, on top of the underlying data. All data points are first scaled by (\c sx, \c sy) and then shifted by (\c dx, \c dy).
/* Calling this function will only have an effect on axes which do not have normalization enabled (using enableYAxisNormalization() or enableXAxisNormalization()). If you want your changes to stick, be sure to disable normalization first.*/
void MPlotAbstractSeries::applyTransform(qreal sx, qreal sy, qreal dx, qreal dy) {
    sx_ = sx;
    sy_ = sy;
    dx_ = dx;
    dy_ = dy;

    onDataChangedPrivate();	// will require an update to the bounding rect, geometry, and plot
}

// Call this function to keep the data normalized within a specified range.  When normalization is enabled, regardless of how the data source changes, the minimum value will always appear at \c min and the maximum value will always appear at \c max.  This effectively disables applyTransform() in the y-axis.
void MPlotAbstractSeries::enableYAxisNormalization(bool on, qreal min, qreal max) {
    yAxisNormalizationOn_ = on;
    if(on) {
        normYMin_ = min;
        normYMax_ = max;
    }
    else {
        sy_ = 1.0;
        dy_ = 0.0;
    }

    onDataChangedPrivate();
}

// Call this function to keep the data normalized within a specified range.  When normalization is enabled, regardless of how the data source changes, the minimum value will always appear at \c min and the maximum value will always appear at \c max.  This effectively disables applyTransform() in the x-axis.
void MPlotAbstractSeries::enableXAxisNormalization(bool on, qreal min, qreal max) {
    xAxisNormalizationOn_ = on;
    if(on) {
        normXMin_ = min;
        normXMax_ = max;
    }
    else {
        sx_ = 1.0;
        dx_ = 0.0;
    }

    onDataChangedPrivate();
}

void MPlotAbstractSeries::setOffset(qreal dx, qreal dy) {
    offset_ = QPointF(dx, dy);

    onDataChangedPrivate();
}
