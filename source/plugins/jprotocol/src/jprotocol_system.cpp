#include "precomp.h"
#include "jprotocol_system.h"

namespace JProtocol {

// SystemPrivate

class SystemPrivate
{
public:
    SystemPrivate(System *q)
        : q_ptr(q)
    {

    }

    void init();

private:
    J_DECLARE_PUBLIC(System)
};

void SystemPrivate::init()
{

}

// System

System::System(QObject *parent)
    : Object(parent)
    , d_ptr(new SystemPrivate(this))
{
    Q_D(System);
    d->init();
}

System::~System()
{
    Q_D(System);
    delete d;
}

System::System(const System &rhs)
{
    operator =(rhs);
}

System &System::operator=(const System &rhs)
{
    if (this == &rhs) {
        return *this;
    }
    //
    return *this;
}

System *System::clone() const
{
    return new System(*this);
}

}   // end of namespace JProtocol 
