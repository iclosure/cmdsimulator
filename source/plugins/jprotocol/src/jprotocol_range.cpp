#include "precomp.h"
#include "jprotocol_range.h"

namespace JProtocol {

// RangePrivate

class RangePrivate
{
public:
    RangePrivate(Range *q)
        : q_ptr(q)
        , minimum(0)
        , maximum(0)
        , minimumEdge(false)
        , maximumEdge(false)
        , minimumInf(true)
        , maximumInf(true)
    {

    }

    void init();

private:
    J_DECLARE_PUBLIC(Range)
    qreal minimum;
    qreal maximum;
    bool minimumEdge;
    bool maximumEdge;
    bool minimumInf;
    bool maximumInf;
};

void RangePrivate::init()
{

}

// Range

Range::Range(QObject *parent)
    : QObject(parent)
    , d_ptr(new RangePrivate(this))
{
    Q_D(Range);
    d->init();
}

Range::~Range()
{
    Q_D(Range);
    delete d;
}

Range::Range(const Range &rhs)
{
    operator =(rhs);
}

Range &Range::operator=(const Range &rhs)
{
    if (this == &rhs) {
        return *this;
    }
    Q_D(Range);
    d->minimum = rhs.d_ptr->minimum;
    d->maximum = rhs.d_ptr->maximum;
    d->minimumEdge = rhs.d_ptr->minimumEdge;
    d->maximumEdge = rhs.d_ptr->maximumEdge;
    d->minimumInf = rhs.d_ptr->minimumInf;
    d->maximumInf = rhs.d_ptr->maximumInf;
    return *this;
}

Range *Range::clone() const
{
    return new Range(*this);
}

qreal Range::minimum() const
{
    Q_D(const Range);
    return d->minimum;
}

qreal Range::maximum() const
{
    Q_D(const Range);
    return d->maximum;
}

bool Range::minimumEdge() const
{
    Q_D(const Range);
    return d->minimumEdge;
}

bool Range::maximumEdge() const
{
    Q_D(const Range);
    return d->maximumEdge;
}

bool Range::minimumInf() const
{
    Q_D(const Range);
    return d->minimumInf;
}

bool Range::maximumInf() const
{
    Q_D(const Range);
    return d->maximumInf;
}

void Range::setMinimum(qreal value)
{
    Q_D(Range);
    if (value != d->minimum) {
        d->minimum = value;
        emit minimumChanged(value);
    }
}

void Range::setMaximum(qreal value)
{
    Q_D(Range);
    if (value != d->maximum) {
        d->maximum = value;
        emit maximumChanged(value);
    }
}

void Range::setMinimumEdge(bool value)
{
    Q_D(Range);
    if (value != d->minimumEdge) {
        d->minimumEdge = value;
        emit minimumEdgeChanged(value);
    }
}

void Range::setMaximumEdge(bool value)
{
    Q_D(Range);
    if (value != d->maximumEdge) {
        d->maximumEdge = value;
        emit maximumEdgeChanged(value);
    }
}

void Range::setMinimumInf(bool value)
{
    Q_D(Range);
    if (value != d->minimumInf) {
        d->minimumInf = value;
        emit minimumInfChanged(value);
    }
}

void Range::setMaximumInf(bool value)
{
    Q_D(Range);
    if (value != d->maximumInf) {
        d->maximumInf = value;
        emit maximumInfChanged(value);
    }
}

}   // end of namespace JProtocol 
