#ifndef JPROTOCOL_COUNTER_H
#define JPROTOCOL_COUNTER_H

#include "jprotocol_item.h"

namespace JProtocol {

class CounterPrivate;

class Counter : public Item
{
    Q_OBJECT
public:
    explicit Counter(QObject *parent = 0);
    virtual ~Counter();

    Counter(const Counter &);
    Counter &operator=(const Counter &);
    virtual Counter *clone() const;

signals:

public slots:

private:
    J_DECLARE_PRIVATE(Counter)
};

} // end of namespace JProtocol 

#endif // JPROTOCOL_COUNTER_H
