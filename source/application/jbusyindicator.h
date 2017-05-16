#ifndef JBUSYINDICATOR_H
#define JBUSYINDICATOR_H

#include <QObject>

class QQuickView;
class QQuickItem;

class JBusyIndicator : public QObject
{
    Q_OBJECT
public:
    explicit JBusyIndicator(bool visible, QObject *parent = 0);
    ~JBusyIndicator();

    bool isVisible() const;
    void setVisible(bool visible);

Q_SIGNALS:

public Q_SLOTS:

private:
    QQuickView *q_view;
    QQuickItem *q_rootItem;
};

#endif // JBUSYINDICATOR_H
