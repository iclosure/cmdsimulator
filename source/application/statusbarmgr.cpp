#include "precomp.h"
#include "statusbarmgr.h"

// - class StatusBarMgr -

StatusBarMgr::StatusBarMgr(QWidget *parent) :
    QStatusBar(parent)
{
    setSizeGripEnabled(true);
    //setFixedHeight(30);

    // label info
    q_labelInfo = new QLabel(this);
    addWidget(q_labelInfo);

    // progressBar
    q_progressBar = new QProgressBar(this);
    q_progressBar->setObjectName("progressBar");
    q_progressBar->setMinimumWidth(150);
    addPermanentWidget(q_progressBar);

    // time
    QLabel *labelTime = new QLabel(this);
    labelTime->setObjectName("labelTime");
    labelTime->setAlignment(Qt::AlignCenter);
    addPermanentWidget(labelTime);

    QTimer *timer = new QTimer(this);
    timer->setInterval(1000);
    timer->start();
    connect(timer, &QTimer::timeout, [=](){
        labelTime->setText(QString("%1").arg(QDateTime::currentDateTime()
                                             .toString("yyyy/MM/dd hh:mm:ss")));
    });
    connect(q_progressBar, &QProgressBar::valueChanged, [=](int value){
        if (value == 0) {
            q_progressBar->setVisible(false);
        } else {
            q_progressBar->setVisible(true);
        }
    });
}

bool StatusBarMgr::init()
{
    bool result = true;

    //
    q_progressBar->setValue(0);

    return result;
}

void StatusBarMgr::setMessage(const QString &text)
{
    q_labelInfo->setText(text);
}
