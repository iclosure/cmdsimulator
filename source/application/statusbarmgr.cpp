﻿#include "precomp.h"
#include "statusbarmgr.h"
#include "modelmgrwidget.h"

// - class StatusBarMgr -

StatusBarMgr::StatusBarMgr(QWidget *parent) :
    QStatusBar(parent)
{
    setSizeGripEnabled(true);
    //setFixedHeight(30);

    // information
    q_modelMgrWidget = new ModelMgrWidget(this);
    q_modelMgrWidget->setObjectName("modelMgrWidget");
    insertWidget(0, q_modelMgrWidget);

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
    connect(q_modelMgrWidget, SIGNAL(currentModelChanged(QString)),
            SIGNAL(currentModelChanged(QString)));
    connect(q_modelMgrWidget, SIGNAL(currentIndexChanged(QString)),
            SIGNAL(currentIndexChanged(QString)));
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

    // 初始化模式切换模块
    result = result && q_modelMgrWidget->init();

    return result;
}

void StatusBarMgr::setCurrentModel(const QString &model)
{
    q_modelMgrWidget->setCurrentModel(model);
}

void StatusBarMgr::setMessage(const QString &text)
{
    q_labelInfo->setText(text);
}
