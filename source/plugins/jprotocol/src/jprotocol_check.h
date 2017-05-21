#ifndef JPROTOCOL_ChECK_H
#define JPROTOCOL_ChECK_H

#include "jprotocol_item.h"

namespace JProtocol {

class CheckPrivate;

class Check : public Item
{
    Q_OBJECT
public:
    explicit Check(QObject *parent = 0);
    virtual ~Check();

    Check(const Check &);
    Check &operator=(const Check &);
    virtual Check *clone() const;

signals:

public slots:

private:
    J_DECLARE_PRIVATE(Check)
};

} // end of namespace JProtocol 

#endif // JPROTOCOL_ChECK_H
