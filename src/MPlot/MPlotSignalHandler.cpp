#include "MPlotSignalHandler.h"


MPlotSignalHandler::MPlotSignalHandler(MPlot* parent)
    : QObject(0) {
    plot_ = parent;
}

void MPlotSignalHandler::onBoundsChanged() {
    MPlotItemSignalSource* source = qobject_cast<MPlotItemSignalSource*>(sender());
    if(source)
        plot_->onBoundsChanged(source->plotItem());
}

void MPlotSignalHandler::onSelectedChanged(bool isSelected) {
    MPlotItemSignalSource* source = qobject_cast<MPlotItemSignalSource*>(sender());
    if(source)
        plot_->onSelectedChanged(source->plotItem(), isSelected);
}

void MPlotSignalHandler::onPlotItemLegendContentChanged() {
    MPlotItemSignalSource* source = qobject_cast<MPlotItemSignalSource*>(sender());
    if(source)
        plot_->onPlotItemLegendContentChanged(source->plotItem());
    else
        plot_->onPlotItemLegendContentChanged(0);
}
