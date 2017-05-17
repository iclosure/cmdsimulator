#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>

class QStackedWidget;
class MainPageWidget;

class CentralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CentralWidget(QWidget *parent = 0);

    bool init();

    MainPageWidget *mainPage() const;

Q_SIGNALS:

public Q_SLOTS:

private:
    QStackedWidget *d_stackedWidget;
    MainPageWidget *d_mainPage;
};

#endif // CENTRALWIDGET_H
