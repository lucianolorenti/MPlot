#ifndef MPLOTAXISRANGE_H
#define MPLOTAXISRANGE_H
#include "MPlot_global.h"
#include <QRectF>
class MPLOTSHARED_EXPORT MPlotAxisRange {
public:
    /// Constructs a null axis range
    MPlotAxisRange() { min_ = 0.0; max_ = 0.0; valid_ = false; }
    /// Constructs an axis range from \c min to \c max
    MPlotAxisRange(qreal min, qreal max) { setRange(min, max); }
    /// Constructs an axis range from the vertical or horizontal extent of a QRectF
    MPlotAxisRange(const QRectF& rect, Qt::Orientation orientation) {
        if(!rect.isValid())
            *this = MPlotAxisRange();

        else if(orientation == Qt::Vertical) {
            min_ = rect.top();
            max_ = rect.bottom();
            valid_ = true;
        }
        else {
            min_ = rect.left();
            max_ = rect.right();
            valid_ = true;
        }
    }

    // using default copy constructor and assignment operator

    /// A null MPlotAxisRange has never had its range set.  A valid MPlotAxisRange comes from setting the range, either in the constructor or in setRange().
    bool isValid() const { return valid_; }

    /// Returns a new MPlotAxisRange that is guaranteed to have min() < max(), by swapping max and min if required.
    MPlotAxisRange normalized() const {
        MPlotAxisRange rv(min_, max_);
        if(min_>max_)
            qSwap(rv.min_, rv.max_);
        return rv;
    }

    /// Ensures that this MPlotAxisRange is guaranteed to have min() < max(), by swapping max and min if required.
    void normalize() {
        *this = normalized();
    }

    /// Return the axis range formed by constraining this range within \c constraint.  The returned range will be shortened if necessary so that both min() and max() are inside of constraint.min() and constraint.max().
    MPlotAxisRange constrainedTo(const MPlotAxisRange& constraint) {
        qreal effectiveMin = qMin(constraint.min(), constraint.max());
        qreal effectiveMax = qMax(constraint.min(), constraint.max());

        return MPlotAxisRange(
                    qBound(effectiveMin, min_, effectiveMax),
                    qBound(effectiveMin, max_, effectiveMax));
    }

    /// Ensures that this MPlotAxisRange is within \c constraint.  Both min() and max() will be bound inside of constraint.min() and constraint.max().
    void constrainTo(const MPlotAxisRange& constraint) {
        *this = constrainedTo(constraint);
    }

    /// Returns the minimum value for this axis range.
    qreal min() const { return min_; }
    /// Returns the maximum value for this axis range.
    qreal max() const { return max_; }
    /// Returns the separation between the minimum and maximum in this axis range.
    qreal length() const { return max_ - min_; }

    /// Sets the minimum value to \param min for this axis range.
    void setMin(qreal min) { min_ = min; }
    /// Sets the maximum value to \param max for this axis range.
    void setMax(qreal max) { max_ = max; }

    /// Sets the range (min and max) for this axis range to \param min and \param max.
    void setRange(qreal min, qreal max) { min_ = min; max_ = max; valid_ = true; }

    /// Operator for MPlotAxisRanges.  Takes the lowest minimum and largest maximum from the current axis range and \param other.
    MPlotAxisRange& operator|=(const MPlotAxisRange& other) {
        if(!other.isValid())
            return *this;	// if other is not valid: don't change anything.

        if(!isValid()) {
            return (*this = other);	// if we weren't valid, but other is: take on values of other and become valid.
        }

        // normal situation, both valid. expand to min, max of both
        if(other.min_ < min_)
            min_ = other.min_;
        if(other.max_ > max_)
            max_ = other.max_;

        return *this;
    }

protected:
    /// Holds the minimum and maximum value for this axis range.
    qreal min_, max_;
    /// Holds whether or not this axis range is valid or not.
    bool valid_;
};

#endif // MPLOTAXISRANGE_H
