#ifndef JPROTOCOL_RANGE_H
#define JPROTOCOL_RANGE_H

#include <QObject>
#include "jprotocol_global.h"

namespace JProtocol {

class RangePrivate;

class Range : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal minimum READ minimum WRITE setMinimum NOTIFY minimumChanged)
    Q_PROPERTY(qreal maximum READ maximum WRITE setMaximum NOTIFY maximumChanged)
    Q_PROPERTY(bool minimumEdge READ minimumEdge WRITE setMinimumEdge NOTIFY minimumEdgeChanged)
    Q_PROPERTY(bool maximumEdge READ maximumEdge WRITE setMaximumEdge NOTIFY maximumEdgeChanged)
    Q_PROPERTY(bool minimumInf READ minimumInf WRITE setMinimumInf NOTIFY minimumInfChanged)
    Q_PROPERTY(bool maximumInf READ maximumInf WRITE setMaximumInf NOTIFY maximumInfChanged)
public:
    explicit Range(QObject *parent = 0);
    virtual ~Range();

    Range(const Range &);
    Range &operator=(const Range &);
    virtual Range *clone() const;

    qreal minimum() const;
    qreal maximum() const;
    bool minimumEdge() const;
    bool maximumEdge() const;
    bool minimumInf() const;
    bool maximumInf() const;

signals:
    void minimumChanged(qreal value);
    void maximumChanged(qreal value);
    void minimumEdgeChanged(bool value);
    void maximumEdgeChanged(bool value);
    void minimumInfChanged(bool value);
    void maximumInfChanged(bool value);

public slots:
    void setMinimum(qreal value);
    void setMaximum(qreal value);
    void setMinimumEdge(bool value);
    void setMaximumEdge(bool value);
    void setMinimumInf(bool value);
    void setMaximumInf(bool value);

private:
    J_DECLARE_PRIVATE(Range)
};

} // end of namespace JProtocol 

#endif // JPROTOCOL_RANGE_H
