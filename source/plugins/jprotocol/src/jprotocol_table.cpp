#include "precomp.h"
#include "jprotocol_table.h"

namespace JProtocol {

// TablePrivate

class TablePrivate
{
public:
    TablePrivate(Table *q)
        : q_ptr(q)
    {

    }

    void init();

private:
    J_DECLARE_PUBLIC(Table)
};

void TablePrivate::init()
{

}

// Table

Table::Table(QObject *parent)
    : Object(parent)
    , d_ptr(new TablePrivate(this))
{
    Q_D(Table);
    d->init();
}

Table::~Table()
{
    Q_D(Table);
    delete d;
}

Table::Table(const Table &rhs)
{
    operator =(rhs);
}

Table &Table::operator=(const Table &rhs)
{
    if (this == &rhs) {
        return *this;
    }
    //
    return *this;
}

Table *Table::clone() const
{
    return new Table(*this);
}

}   // end of namespace JProtocol 
