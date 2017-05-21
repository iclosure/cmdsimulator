#ifndef JPROTOCOL_Header_H
#define JPROTOCOL_Header_H

#include "jprotocol_item.h"

namespace JProtocol {

class HeaderPrivate;

class Header : public QObject
{
    Q_OBJECT
public:
    explicit Header(QObject *parent = 0);
    virtual ~Header();

    Header(const Header &);
    Header &operator=(const Header &);
    virtual Header *clone() const;

signals:

public slots:

private:
    J_DECLARE_PRIVATE(Header)
};

} // end of namespace JProtocol 

#endif // JPROTOCOL_Header_H
