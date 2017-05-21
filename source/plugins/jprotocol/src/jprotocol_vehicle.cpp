#include "precomp.h"
#include "jprotocol_vehicle.h"

namespace JProtocol {

// VehiclePrivate

class VehiclePrivate
{
public:
    VehiclePrivate(Vehicle *q)
        : q_ptr(q)
    {

    }

    void init();

private:
    J_DECLARE_PUBLIC(Vehicle)
};

void VehiclePrivate::init()
{

}

// Vehicle

Vehicle::Vehicle(QObject *parent)
    : Object(parent)
    , d_ptr(new VehiclePrivate(this))
{
    Q_D(Vehicle);
    d->init();
}

Vehicle::~Vehicle()
{
    Q_D(Vehicle);
    delete d;
}

Vehicle::Vehicle(const Vehicle &rhs)
{
    operator =(rhs);
}

Vehicle &Vehicle::operator=(const Vehicle &rhs)
{
    if (this == &rhs) {
        return *this;
    }
    //
    return *this;
}

Vehicle *Vehicle::clone() const
{
    return new Vehicle(*this);
}

}   // end of namespace JProtocol 
