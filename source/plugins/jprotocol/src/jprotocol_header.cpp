#include "precomp.h"
#include "jprotocol_header.h"

namespace JProtocol {

// HeaderPrivate

class HeaderPrivate
{
public:
    HeaderPrivate(Header *q)
        : q_ptr(q)
    {

    }

    void init();

private:
    J_DECLARE_PUBLIC(Header)
};

void HeaderPrivate::init()
{

}

// Header

Header::Header(QObject *parent)
    : Item(parent)
    , d_ptr(new HeaderPrivate(this))
{
    Q_D(Header);
    d->init();
}

Header::~Header()
{
    Q_D(Header);
    delete d;
}

Header::Header(const Header &rhs)
{
    operator =(rhs);
}

Header &Header::operator=(const Header &rhs)
{
    if (this == &rhs) {
        return *this;
    }
    //
    return *this;
}

Header *Header::clone() const
{
    return new Header(*this);
}

}   // end of namespace JProtocol 
