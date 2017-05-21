#ifndef JPROTOCOL_FRAME_H
#define JPROTOCOL_FRAME_H

#include "jprotocol_item.h"

namespace JProtocol {

class FramePrivate;

class Frame : public Item
{
    Q_OBJECT
public:
    explicit Frame(QObject *parent = 0);
    virtual ~Frame();

    Frame(const Frame &);
    Frame &operator=(const Frame &);
    virtual Frame *clone() const;

signals:

public slots:

private:
    J_DECLARE_PRIVATE(Frame)
};

} // end of namespace JProtocol 

#endif // JPROTOCOL_FRAME_H
