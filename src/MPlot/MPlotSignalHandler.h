#ifndef MPLOTSIGNALHANDLER_H
#define MPLOTSIGNALHANDLER_H
#include "MPlot.h"
#include <QObject>

/**
 * @brief The MPlotSignalHandler class
 * This class handles signals as a proxy for MPlot.  You should never need to use this class directly.
 * To avoid restrictions on multiple inheritance, MPlot does not inherit QObject.
 * Still, it needs a way to respond to events from MPlotItems (such as re-scale and selected events).
 * This QObject receives signals from MPlotItem and calls the appropriate functions within MPlot.
 *
*/
class MPlotSignalHandler : public QObject {
    Q_OBJECT
protected:
    /// Constructor.  Builds a signal source for the MPlot given by \param parent.
    MPlotSignalHandler(MPlot* parent);
    /// Giving the signal handler access to MPlots protected members.
    friend class MPlot;

protected slots:
    /// Catches the boundsChanged signal and calls MPlot::onBoundsChanged method with with the pointer of whichever MPlotItem emitted the signal.
    void onBoundsChanged();
    /// \note This slot does nothing.  It calls MPlot::onSelectedChanged, but that is an unimplemented class.
    void onSelectedChanged(bool);
    /// Catches the plotItemLegendContentChanged signal and calls MPlot::onPlotItemLegendContentsChanged with the pointer to item that called it.  If the qobject_cast fails, then a 0 is passed.
    void onPlotItemLegendContentChanged();

    /// Catches the doDelayedAutoscale signal.  This calls MPlot::doDelayedAutoscale which causes the plot to update if the any item within the plot has flagged to the plot that the plot needs to be re-autoscaled.
    void doDelayedAutoscale();
    /// This slot flags the plot that it needs to perform an autoscale the next time it returns to the event loop.
    void onAxisScaleAutoScaleEnabledChanged(bool enabled);

signals:
    /// Notifer that the data position has been updated.  Passes the index of item inside the position indicator tool and the new position (in data coordinates).  Only emitted when the MPlotDataPositionTool has been added to the plot.
    void dataPositionChanged(const QPointF &point);
    /// Notifier of the size of the data rectangle that has been drawn once it is finished.  Only emitted when the MPlotDataPositionTool has been added to the plot.
    void selectedDataRectChanged(const QRectF &rect);

protected:
    /// The instance of MPlot that this signal source is connected to.
    MPlot* plot_;
};
#endif // MPLOTSIGNALHANDLER_H
