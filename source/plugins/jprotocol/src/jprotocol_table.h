#ifndef JPROTOCOL_TABLE_H
#define JPROTOCOL_TABLE_H

#include "jprotocol_object.h"

namespace JProtocol {

class TablePrivate;

class Table : public Object
{
    Q_OBJECT
public:
    explicit Table(QObject *parent = 0);
    virtual ~Table();

    Table(const Table &);
    Table &operator=(const Table &);
    virtual Table *clone() const;

signals:

public slots:

private:
    J_DECLARE_PRIVATE(Table)
};

} // end of namespace JProtocol 

#endif // JPROTOCOL_TABLE_H
