#include "precomp.h"
#include "jprotocol_bit.h"

namespace JProtocol {

// BitPrivate

class BitPrivate
{
public:
    BitPrivate(Bit *q)
        : q_ptr(q)
    {

    }

    void init();

private:
    J_DECLARE_PUBLIC(Bit)
};

void BitPrivate::init()
{

}

// Bit

Bit::Bit(QObject *parent)
    : Item(parent)
    , d_ptr(new BitPrivate(this))
{
    Q_D(Bit);
    d->init();
}

Bit::~Bit()
{
    Q_D(Bit);
    delete d;
}

Bit::Bit(const Bit &rhs)
{
    operator =(rhs);
}

Bit &Bit::operator=(const Bit &rhs)
{
    if (this == &rhs) {
        return *this;
    }
    //
    return *this;
}

Bit *Bit::clone() const
{
    return new Bit(*this);
}

}   // end of namespace JProtocol 
