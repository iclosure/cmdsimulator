#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>

class QStackedWidget;

class CentralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CentralWidget(QWidget *parent = 0);

    bool init();

Q_SIGNALS:

public Q_SLOTS:

private:
    QStackedWidget *q_stackedWidget;
};

#endif // CENTRALWIDGET_H
