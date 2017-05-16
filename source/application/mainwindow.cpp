﻿#include "precomp.h"
#include "mainwindow.h"
#include "centralwidget.h"
#include "statusbarmgr.h"
#include "jstylesheet.h"
#include "jmyexcel.h"
#include "globalconfig.h"
#include "jbusyindicator.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , q_busyIndicator(0)
{
    resize(1024, 768);

    q_centralWidget = new CentralWidget(this);
    q_centralWidget->setObjectName("centralWidget");
    setCentralWidget(q_centralWidget);

    q_statusBarMgr = new StatusBarMgr(this);
    q_statusBarMgr->setObjectName("statusBarMgr");
    setStatusBar(q_statusBarMgr);

    // stylesheet
    setStyleSheet(JStyleSheet::styleSheet("mainwidget"));

    connect(q_statusBarMgr, &StatusBarMgr::currentModelChanged,
            q_centralWidget, &CentralWidget::setCurrentModel);
    connect(q_statusBarMgr, &StatusBarMgr::currentIndexChanged,
            q_centralWidget, &CentralWidget::setCurrentIndex);
    connect(q_centralWidget, &CentralWidget::currentModelChanged,
            q_statusBarMgr, &StatusBarMgr::setCurrentModel);

    //
    qApp->setProperty("mainWindow",
                      QVariant::fromValue<void *>(static_cast<QWidget *>(this)));
    q_busyIndicator = new JBusyIndicator(false, this);
}

MainWindow::~MainWindow()
{
    //
    JWidgetsCore::releaseInstance();

    // release single object
    JStyleSheet::releaseInstance();

    //
    JLogManager::releaseInstance();

    //
    JFlyRecordGenerater::releaseInstance();

    //
    JRuleDict::releaseInstance();

    //
    JSqlTableMgr::releaseInstance();

    //
    JSqlTableMgr::releaseInstance();

    //
    JMyExcel::releaseInstance();
}

bool MainWindow::init()
{
    bool result = true;

    // 初始化航线数据（入库）
    result = result && JMyExcel::instance()->saveBaseLineToSql(
                GlobalConfig::instance()->baseLineFilePath());

    // 初始化核心模块
    result = result && q_centralWidget->init();

    // 初始化状态栏
    result = result && q_statusBarMgr->init();

    return result;
}

bool MainWindow::isBusyIndicatorVisible() const
{
    if (!q_busyIndicator) {
        return false;
    }
    return q_busyIndicator->isVisible();
}

void MainWindow::setBusyIndicatorVisible(bool visible)
{
    if (q_busyIndicator) {
        q_busyIndicator->setVisible(visible);
    }
    setEnabled(!visible);
}

bool MainWindow::isMainWindowEnabled() const
{
    return isEnabled();
}

void MainWindow::setMainWindowEnabled(bool enabled)
{
    setEnabled(enabled);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    int result = QMessageBox::warning(this, QStringLiteral("警告"),
                                      QStringLiteral("是否要关闭软件？点击【退出】即可"),
                                      QStringLiteral("退出"), QStringLiteral("取消"));
    if (result == 1) {
        event->ignore();
        return;
    } else {
        QMainWindow::closeEvent(event);
    }
}