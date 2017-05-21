#ifndef JPROTOCOL_BIT_H
#define JPROTOCOL_BIT_H

#include "jprotocol_item.h"

namespace JProtocol {

class BitPrivate;

class Bit : public Item
{
    Q_OBJECT
public:
    explicit Bit(QObject *parent = 0);
    virtual ~Bit();

    Bit(const Bit &);
    Bit &operator=(const Bit &);
    virtual Bit *clone() const;

    virtual qreal data() const;
    virtual void setData(qreal value);
    virtual QString dataString() const;

    int bitStart() const;
    int bitCount() const;
    int typeSize() const;
    quint64 mask() const;

    bool testBit(int offset) const;

    QMap<quint64, QString> specs();
    const QMap<quint64, QString> specs() const;
    void addSpec(quint64 key, const QString &spec);
    void removeSpec(quint64 key);

signals:

public slots:
    void setBitStart(int value);
    void setBitCount(int value);
    void setTypeSize(int value);

private:
    J_DECLARE_PRIVATE(Bit)
};

} // end of namespace JProtocol 

#endif // JPROTOCOL_BIT_H
