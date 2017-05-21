#ifndef JPROTOCOL_COMPLEX_H
#define JPROTOCOL_COMPLEX_H

#include "jprotocol_item.h"

namespace JProtocol {

class ComplexPrivate;

class Complex : public Item
{
    Q_OBJECT
public:
    explicit Complex(QObject *parent = 0);
    virtual ~Complex();

    Complex(const Complex &);
    Complex &operator=(const Complex &);
    virtual Complex *clone() const;

signals:

public slots:

private:
    J_DECLARE_PRIVATE(Complex)
};

} // end of namespace JProtocol 

#endif // JPROTOCOL_COMPLEX_H
