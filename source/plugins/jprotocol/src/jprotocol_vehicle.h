#ifndef JPROTOCOL_VEHICLE_H
#define JPROTOCOL_VEHICLE_H

#include "jprotocol_object.h"

namespace JProtocol {

class VehiclePrivate;

class Vehicle : public Object
{
    Q_OBJECT
public:
    explicit Vehicle(QObject *parent = 0);
    virtual ~Vehicle();

    Vehicle(const Vehicle &);
    Vehicle &operator=(const Vehicle &);
    virtual Vehicle *clone() const;

signals:

public slots:

private:
    J_DECLARE_PRIVATE(Vehicle)
};

} // end of namespace JProtocol 

#endif // JPROTOCOL_VEHICLE_H
