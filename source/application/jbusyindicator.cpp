#include "precomp.h"
#include "jbusyindicator.h"
#include <QQuickView>

JBusyIndicator::JBusyIndicator(bool visible, QObject *parent) :
    QObject(parent),
    q_view(0),
    q_rootItem(0)
{
    q_view = new QQuickView();
    q_view->resize(200, 200);
    q_view->setFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    q_view->setColor(QColor(Qt::transparent));
    q_view->setSource(QUrl(QStringLiteral("qrc:/application/qml/JBusyIndicator.qml")));
    if (visible) {
        q_view->show();
    }
    //
    q_rootItem = q_view->rootObject();
    if (q_rootItem) {
        //
    }
}

JBusyIndicator::~JBusyIndicator()
{
    q_view->deleteLater();
    q_view = 0;
}

bool JBusyIndicator::isVisible() const
{
    if (!q_rootItem) {
        return false;
    }

    return q_view->isVisible();
}

void JBusyIndicator::setVisible(bool visible)
{
    if (q_view) {
        q_view->setVisible(visible);
    }
}
