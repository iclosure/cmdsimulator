#ifndef MAINPAGEWIDGET_H
#define MAINPAGEWIDGET_H

#include <QWidget>

class MainNavWidget;
class MainViewWidget;

class MainPageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainPageWidget(QWidget *parent = 0);

    bool init();

    MainNavWidget *mainNav() const;
    MainViewWidget *mainView() const;

signals:

public slots:

private:
    MainNavWidget *d_mainNav;
    MainViewWidget *d_mainView;
};

#endif // MAINPAGEWIDGET_H
