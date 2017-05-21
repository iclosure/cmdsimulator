#include "precomp.h"
#include "jprotocol_frame.h"

namespace JProtocol {

// FramePrivate

class FramePrivate
{
public:
    FramePrivate(Frame *q)
        : q_ptr(q)
    {

    }

    void init();

private:
    J_DECLARE_PUBLIC(Frame)
};

void FramePrivate::init()
{

}

// Frame

Frame::Frame(QObject *parent)
    : Item(parent)
    , d_ptr(new FramePrivate(this))
{
    Q_D(Frame);
    d->init();
}

Frame::~Frame()
{
    Q_D(Frame);
    delete d;
}

Frame::Frame(const Frame &rhs)
{
    operator =(rhs);
}

Frame &Frame::operator=(const Frame &rhs)
{
    if (this == &rhs) {
        return *this;
    }
    //
    return *this;
}

Frame *Frame::clone() const
{
    return new Frame(*this);
}

}   // end of namespace JProtocol 
