#include "precomp.h"
#include "jprotocol_framecode.h"

namespace JProtocol {

// FrameCodePrivate

class FrameCodePrivate
{
public:
    FrameCodePrivate(FrameCode *q)
        : q_ptr(q)
    {

    }

    void init();

private:
    J_DECLARE_PUBLIC(FrameCode)
};

void FrameCodePrivate::init()
{

}

// FrameCode

FrameCode::FrameCode(QObject *parent)
    : Item(parent)
    , d_ptr(new FrameCodePrivate(this))
{
    Q_D(FrameCode);
    d->init();
}

FrameCode::~FrameCode()
{
    Q_D(FrameCode);
    delete d;
}

FrameCode::FrameCode(const FrameCode &rhs)
{
    operator =(rhs);
}

FrameCode &FrameCode::operator=(const FrameCode &rhs)
{
    if (this == &rhs) {
        return *this;
    }
    //
    return *this;
}

FrameCode *FrameCode::clone() const
{
    return new FrameCode(*this);
}

}   // end of namespace JProtocol 
