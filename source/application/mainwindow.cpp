#include "precomp.h"
#include "mainwindow.h"
#include "centralwidget.h"
#include "statusbarmgr.h"
#include "jstylesheet.h"
#include "globalconfig.h"

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

    //
    qApp->setProperty("mainWindow",
                      QVariant::fromValue<void *>(static_cast<QWidget *>(this)));
}

MainWindow::~MainWindow()
{
    //
    JWidgetsCore::releaseInstance();

    // release single object
    JStyleSheet::releaseInstance();

    //
    JLogManager::releaseInstance();
}

bool MainWindow::init()
{
    bool result = true;

    // 初始化核心模块
    result = result && q_centralWidget->init();

    // 初始化状态栏
    result = result && q_statusBarMgr->init();

    return result;
}

bool MainWindow::isBusyIndicatorVisible() const
{
    return false;
}

void MainWindow::setBusyIndicatorVisible(bool visible)
{
    Q_UNUSED(visible);
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
    //TEST
    QMainWindow::closeEvent(event);
    return;
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
