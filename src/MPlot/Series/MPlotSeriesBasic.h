#ifndef MPLOTSERIESBASIC_H
#define MPLOTSERIESBASIC_H

#include "MPlotAbstractSeries.h"




/// When drawing large datasets, we won't draw more than MPLOT_MAX_LINES_PER_PIXEL lines per x-axis pixel.
/// We sub-sample by plotting only the maximum and minimum values over the x-axis increment that corresponds to 1px.
/// The value that makes sense here is 1 (since you can't see any more... they would just look like vertical lines on top of each other anyway.)  When drawing anti-aliased, changing this to 2 makes smoother plots.
#define MPLOT_MAX_LINES_PER_PIXEL 1.0

/// If you're going to add a lot of points to the model (without caring about updates in between), recommend this for performance reasons:
/*!
 MPlotSeriesBasic series;
  ....
 series->setModel(0);	/// disconnect series from data model
 /// add points to model...
 series->setModel(model);	/// reconnect model to series
 */

/// MPlotSeriesBasic provides one drawing implementation for a 2D plot curve.  It is optimized to efficiently draw curves with 1,000,000+ data points along the x-axis, by only drawing as many lines as would be visible.

class MPLOTSHARED_EXPORT MPlotSeriesBasic : public MPlotAbstractSeries {

public:
    /// Constructor.  Builds a series of data.  This is a standard 1D line plot.
    MPlotSeriesBasic(const std::shared_ptr<MPlotAbstractSeriesData> data );
    /// Destructor.
    virtual ~MPlotSeriesBasic();


    /// Required functions:
    //////////////////////////
    /// boundingRect: using parent implementation, but adding extra room on edges for our selection highlight and markers.
    virtual QRectF boundingRect() const;

    /// Paint:
    virtual void paint(QPainter* painter,
                       const QStyleOptionGraphicsItem* option,
                       QWidget* widget);
    /// Helper function that paints the lines that connect the points of the series.
    virtual void paintLines(QPainter* painter);
    /// Helper function that paints the markers on the points that make up the series.
    virtual void paintMarkers(QPainter* painter);

    /// re-implemented from MPlotItem base to draw an update if we're now selected (with our selection highlight)
    virtual void setSelected(bool selected = true);

    void setSubsample(bool b);

protected: //"slots"

    /// Handle implementation-specific drawing updates
    virtual void onDataChanged();

protected:
    /// Subsample SeriesData
    bool _subsample;

    QPen getPenFor(const qreal x);

    /// Customize this if needed for MPlotSeries. For now we use the parent class implementation
    /*
  virtual void setDefaults() {

  }*/
};

#endif // MPLOTSERIESBASIC_H
