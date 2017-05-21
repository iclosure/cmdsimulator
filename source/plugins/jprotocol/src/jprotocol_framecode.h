#ifndef JPROTOCOL_FRAMECODE_H
#define JPROTOCOL_FRAMECODE_H

#include "jprotocol_item.h"

namespace JProtocol {

class FrameCodePrivate;

class FrameCode : public Item
{
    Q_OBJECT
public:
    explicit FrameCode(QObject *parent = 0);
    virtual ~FrameCode();

    FrameCode(const FrameCode &);
    FrameCode &operator=(const FrameCode &);
    virtual FrameCode *clone() const;

signals:

public slots:

private:
    J_DECLARE_PRIVATE(FrameCode)
};

} // end of namespace JProtocol 

#endif // JPROTOCOL_FRAMECODE_H
