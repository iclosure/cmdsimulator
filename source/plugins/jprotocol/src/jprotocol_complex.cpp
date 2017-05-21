#include "precomp.h"
#include "jprotocol_complex.h"

namespace JProtocol {

// ComplexPrivate

class ComplexPrivate
{
public:
    ComplexPrivate(Complex *q)
        : q_ptr(q)
    {

    }

    void init();

private:
    J_DECLARE_PUBLIC(Complex)
};

void ComplexPrivate::init()
{

}

// Complex

Complex::Complex(QObject *parent)
    : Item(parent)
    , d_ptr(new ComplexPrivate(this))
{
    Q_D(Complex);
    d->init();
}

Complex::~Complex()
{
    Q_D(Complex);
    delete d;
}

Complex::Complex(const Complex &rhs)
{
    operator =(rhs);
}

Complex &Complex::operator=(const Complex &rhs)
{
    if (this == &rhs) {
        return *this;
    }
    //
    return *this;
}

Complex *Complex::clone() const
{
    return new Complex(*this);
}

}   // end of namespace JProtocol 
