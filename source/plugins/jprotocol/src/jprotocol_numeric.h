#ifndef JPROTOCOL_NUMERIC_H
#define JPROTOCOL_NUMERIC_H

#include "jprotocol_item.h"

namespace JProtocol {

class NumericPrivate;

class Numeric : public Item
{
    Q_OBJECT
public:
    explicit Numeric(QObject *parent = 0);
    virtual ~Numeric();

    Numeric(const Numeric &);
    Numeric &operator=(const Numeric &);
    virtual Numeric *clone() const;

signals:

public slots:

private:
    J_DECLARE_PRIVATE(Numeric)
};

} // end of namespace JProtocol 

#endif // JPROTOCOL_NUMERIC_H
