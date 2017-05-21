#include "precomp.h"
#include "jprotocol_numeric.h"

namespace JProtocol {

// NumericPrivate

class NumericPrivate
{
public:
    NumericPrivate(Numeric *q)
        : q_ptr(q)
    {

    }

    void init();

private:
    J_DECLARE_PUBLIC(Numeric)
};

void NumericPrivate::init()
{

}

// Numeric

Numeric::Numeric(QObject *parent)
    : Item(parent)
    , d_ptr(new NumericPrivate(this))
{
    Q_D(Numeric);
    d->init();
}

Numeric::~Numeric()
{
    Q_D(Numeric);
    delete d;
}

Numeric::Numeric(const Numeric &rhs)
{
    operator =(rhs);
}

Numeric &Numeric::operator=(const Numeric &rhs)
{
    if (this == &rhs) {
        return *this;
    }
    //
    return *this;
}

Numeric *Numeric::clone() const
{
    return new Numeric(*this);
}

}   // end of namespace JProtocol 
