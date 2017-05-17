#include "precomp.h"
#include "mainpagewidget.h"
#include "mainnavwidget.h"
#include "mainviewwidget.h"

MainPageWidget::MainPageWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *horiLayoutMain = new QHBoxLayout(this);

    JSplitter *splitterMain = new JSplitter(this);
    splitterMain->setHandleWidth(6);
    splitterMain->setScales(QList<double>() << 1 << 3);
    splitterMain->setHandleColor(QColor(20, 20, 20, 20));
    horiLayoutMain->addWidget(splitterMain);

    d_mainNav = new MainNavWidget(this);
    splitterMain->addWidget(d_mainNav);

    d_mainView = new MainViewWidget(this);
    splitterMain->addWidget(d_mainView);
}

bool MainPageWidget::init()
{
    bool result = true;

    //
    result = result && d_mainNav->init();

    //
    result = result && d_mainView->init();

    return result;
}

MainNavWidget *MainPageWidget::mainNav() const
{
    return d_mainNav;
}

MainViewWidget *MainPageWidget::mainView() const
{
    return d_mainView;
}
