#include "precomp.h"
#include "centralwidget.h"
#include "mainpage/mainpagewidget.h"

CentralWidget::CentralWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *horiLayoutMain = new QHBoxLayout(this);
    horiLayoutMain->setContentsMargins(0, 0, 0, 0);

    d_stackedWidget = new QStackedWidget(this);
    horiLayoutMain->addWidget(d_stackedWidget);

    d_mainPage = new MainPageWidget(this);

    d_stackedWidget->addWidget(d_mainPage);

    //
    d_stackedWidget->setCurrentIndex(0);

    // initialize
}

bool CentralWidget::init()
{
    bool result = true;

    //
    result = result && d_mainPage->init();

    return result;
}

MainPageWidget *CentralWidget::mainPage() const
{
    return d_mainPage;
}
