#include "MPlotSeriesBasic.h"
#include <QDebug>
/////////////////////////////
// MPlotSeriesBasic
////////////////////////////

MPlotSeriesBasic::MPlotSeriesBasic(const MPlotAbstractSeriesData* data)
    : MPlotAbstractSeries() {

    // Set style defaults:
    setDefaults();

    setModel(data);
}

MPlotSeriesBasic::~MPlotSeriesBasic() {

}

// Required functions:
//////////////////////////

QRectF MPlotSeriesBasic::boundingRect() const {
    QRectF br = MPlotAbstractSeries::boundingRect();

    if(br.isValid()) {
        // create rectangle at least as big as our selection highlight, and if we have a marker, the marker size.
        QRectF hs = QRectF(0, 0, MPLOT_SELECTION_LINEWIDTH, MPLOT_SELECTION_LINEWIDTH);
        // expand by marker size (expressed in pixels)
        if(marker())
            hs |= QRectF(0,0, marker()->size(), marker()->size());


        // these sizes so far are in pixels (hopefully scene coordinates... trusting on an untransformed view.) Converting to local (data) coordinates.
        // no longer necessary in new drawing coordinate system: hs = mapRectFromScene(hs);

        // really we just need 1/2 the marker size and 1/2 the selection highlight width. But extra doesn't hurt.
        br.adjust(-hs.width(),-hs.height(),hs.width(), hs.height());
    }

    return br;
}

// Paint:
void MPlotSeriesBasic::paint(QPainter* painter,
                             const QStyleOptionGraphicsItem* option,
                             QWidget* widget) {

    Q_UNUSED(option);
    Q_UNUSED(widget);

    if(!yAxisTarget() || !xAxisTarget()) {
        qWarning() << "MPlotSeriesBasic: No axis scale set. Abandoning painting because we don't know what scale to use.";
        return;
    }
    // Plot the markers. Here what makes sense is one marker per data point.  This will be slow for large datasets.
    // use plot->setMarkerShape(MPlotMarkerShape::None) for large sets.
    /////////////////////////////////////////

    if(marker_) {
        painter->setPen(marker_->pen());
        painter->setBrush(marker_->brush());
        paintMarkers(painter);
    }

    // Render lines (this runs fairly fast, even for large datasets, due to subsampling)
    ////////////////////////////////////////////
    if(selected()) {
        painter->setPen(selectedPen_);
        paintLines(painter);
    }
    painter->setPen(linePen_);
    paintLines(painter);
}

void MPlotSeriesBasic::paintLines(QPainter* painter) {

    if(data_ && data_->count() > 0) {
        QVector<qreal> x;
        QVector<qreal> y;
        QTransform wt = painter->deviceTransform();	// equivalent to worldTransform and combinedTransform
        qreal xinc = 1.0 / wt.m11() / MPLOT_MAX_LINES_PER_PIXEL;	// will just be 1/MPLOT_MAX_LINES_PER_PIXEL = 0.5 as long as not using a scaled/transformed painter.
        qreal width = xAxisTarget()->drawingSize().width()/xinc;
        int min = std::max(this->xAxisTarget()->min(),(double)0) ;
        int max =  std::min(this->xAxisTarget()->max(),(double)data_->count()) ;
        int datacount = (width*10)+1;
        x.resize(datacount);
        y.resize(datacount);
        xxyyValues(min, max, x,y);

        for (unsigned int i=0;i<datacount;i++) {
            x[i] = this->mapX(x[i]);
            y[i] = this->mapY(y[i]);
        }

        // should we just draw normally and quickly? Do that if the number of data points is less than the number of x-pixels in the drawing space (or half-pixels, in the conservative case where MPLOT_MAX_LINES_PER_PIXEL = 2).
        if(x.size() < xAxisTarget()->drawingSize().width()/xinc) {

            for (int i = 1, count = data_->count(); i < count; i++)
                painter->drawLine(QPointF(x.at(i-1), y.at(i-1)), QPointF(x.at(i), y.at(i)));
        }

        else {	// do sub-pixel simplification.
            // Instead of drawing lines between all these data points, we'll just plot the max and min value within every xinc range.  This ensures that if there is noise/jumps within a subsample (xinc) range, we'll still see it on the plot.

            qreal xstart;
            qreal ystart, ymin, ymax;

            xstart = x.at(0);
            ymin = ymax = ystart = y.at(0);

            // move through the datapoints along x. (Note that x could be jumping forward or backward here... it's not necessarily sorted)
            for(int i=1, count = x.size(); i < count; i++) {

                // if within the range around xstart: update max/min to be representative of this range
                if(fabs(x.at(i) - xstart) < xinc) {
                    qreal yYI = y.at(i);

                    if(yYI > ymax)
                        ymax = yYI;
                    if(yYI < ymin)
                        ymin = yYI;
                }
                // otherwise draw the lines and move on to next range...
                // The first line represents everything within the range [xstart, xstart+xinc).  Note that these will all be plotted at same x-pixel.
                // The second line connects this range to the next.  Note that (if the x-axis point spacing is not uniform) x(i) may be many pixels from xstart, to the left or right. All we know is that it's outside of our 1px range. If it _is_ far outside the range, to get the slope of the connecting line correct, we need to connect it to the last point preceding it. The point (x_(i-1), y_(i-1)) is within the 1px range [xstart, x_(i-1)] represented by the vertical line.
                // (Brain hurt? imagine a simple example: (0,2) (0,1) (0,0), (5,0).  It should be a vertical line from (0,2) to (0,0), and then a horizontal line from (0,0) to (5,0).  The xinc range is from i=0 (xstart = x(0)) to i=2. The point outside is i=3.
                // For normal/small datasets where the x-point spacing is >> pixel spacing , what will happen is ymax = ymin = ystart (all the same point), and (x(i), y(i)) is the next point.
                else {
                    if(ymin != ymax)
                        painter->drawLine(QPointF(xstart, ymin), QPointF(xstart, ymax));

                    painter->drawLine(QPointF(x.at(i-1), y.at(i-1)), QPointF(x.at(i), y.at(i)));
                    //NOT: painter->drawLine(QPointF(xstart, ystart), QPointF(mapX(xx(i)), mapY(yy(i))));

                    xstart = x.at(i);
                    ymin = ymax = ystart = y.at(i);
                }
            }
        }
    }
}

void MPlotSeriesBasic::paintMarkers(QPainter* painter) {
    return;
    if(data_ && marker_) {

        int dataCount = data_->count();
        QVector<qreal> x = QVector<qreal>(dataCount);
        QVector<qreal> y = QVector<qreal>(dataCount);

        xxyyValues(0, dataCount-1, x,y);


        QVector<qreal> mappedX = QVector<qreal>(dataCount);
        QVector<qreal> mappedY = QVector<qreal>(dataCount);

        mapXValues(mappedX.size(), x.constData(), mappedX.data());
        mapYValues(mappedY.size(), y.constData(), mappedY.data());

        for (int i = data_->count()-1; i >= 0; i--){

            painter->translate(mappedX.at(i), mappedY.at(i));
            marker_->paint(painter);
            painter->translate(-mappedX.at(i), -mappedY.at(i));
        }
    }
}


// re-implemented from MPlotItem base to draw an update if we're now selected (with our selection highlight)
void MPlotSeriesBasic::setSelected(bool selected) {

    bool wasSelected = isSelected();
    MPlotItem::setSelected(selected);
    if(isSelected() != wasSelected)
        update();

}

// All the specific re-drawing we need to do when the data changes (or a new model is set) is contained in update().
void MPlotSeriesBasic::onDataChanged() {
    update();
}
