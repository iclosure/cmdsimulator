#include "precomp.h"
#include "centralwidget.h"

CentralWidget::CentralWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *horiLayoutMain = new QHBoxLayout(this);
    horiLayoutMain->setContentsMargins(0, 0, 0, 0);

    q_stackedWidget = new QStackedWidget(this);
    horiLayoutMain->addWidget(q_stackedWidget);

    //q_stackedWidget->addWidget(q_dataMgrWidget);
    //

    //
    q_stackedWidget->setCurrentIndex(0);

    // initialize
}

bool CentralWidget::init()
{
    bool result = true;

    //

    return result;
}
