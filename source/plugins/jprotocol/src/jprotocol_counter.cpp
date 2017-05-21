#include "precomp.h"
#include "jprotocol_counter.h"

namespace JProtocol {

// CounterPrivate

class CounterPrivate
{
public:
    CounterPrivate(Counter *q)
        : q_ptr(q)
    {

    }

    void init();

private:
    J_DECLARE_PUBLIC(Counter)
};

void CounterPrivate::init()
{

}

// Counter

Counter::Counter(QObject *parent)
    : Item(parent)
    , d_ptr(new CounterPrivate(this))
{
    Q_D(Counter);
    d->init();
}

Counter::~Counter()
{
    Q_D(Counter);
    delete d;
}

Counter::Counter(const Counter &rhs)
{
    operator =(rhs);
}

Counter &Counter::operator=(const Counter &rhs)
{
    if (this == &rhs) {
        return *this;
    }
    //
    return *this;
}

Counter *Counter::clone() const
{
    return new Counter(*this);
}

}   // end of namespace JProtocol 
