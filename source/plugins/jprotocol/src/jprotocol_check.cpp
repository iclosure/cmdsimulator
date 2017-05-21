#include "precomp.h"
#include "jprotocol_check.h"

namespace JProtocol {

// CheckPrivate

class CheckPrivate
{
public:
    CheckPrivate(Check *q)
        : q_ptr(q)
    {

    }

    void init();

private:
    J_DECLARE_PUBLIC(Check)
};

void CheckPrivate::init()
{

}

// Check

Check::Check(QObject *parent)
    : Item(parent)
    , d_ptr(new CheckPrivate(this))
{
    Q_D(Check);
    d->init();
}

Check::~Check()
{
    Q_D(Check);
    delete d;
}

Check::Check(const Check &rhs)
{
    operator =(rhs);
}

Check &Check::operator=(const Check &rhs)
{
    if (this == &rhs) {
        return *this;
    }
    //
    return *this;
}

Check *Check::clone() const
{
    return new Check(*this);
}

}   // end of namespace JProtocol 
