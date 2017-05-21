#ifndef JPROTOCOL_SYSTEM_H
#define JPROTOCOL_SYSTEM_H

#include "jprotocol_object.h"

namespace JProtocol {

class SystemPrivate;

class System : public Object
{
    Q_OBJECT
public:
    explicit System(QObject *parent = 0);
    virtual ~System();

    System(const System &);
    System &operator=(const System &);
    virtual System *clone() const;

signals:

public slots:

private:
    J_DECLARE_PRIVATE(System)
};

} // end of namespace JProtocol 

#endif // JPROTOCOL_SYSTEM_H
