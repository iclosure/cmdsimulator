#include "precomp.h"
#include "datamgrwidget.h"
#include "dataqueryanddisplay.h"
#include "dataimportmgr.h"
#include "databasesampling.h"
#include "databasemgr.h"
#include "rulemgrwidget.h"

DataMgrWidget::DataMgrWidget(QWidget *parent) :
    QWidget(parent)
{
    setObjectName("DataMgrWidget");

    QHBoxLayout *horiLayoutMain = new QHBoxLayout(this);
    horiLayoutMain->setContentsMargins(0, 0, 0, 0);

    q_tabWidget = new QTabWidget(this);
    horiLayoutMain->addWidget(q_tabWidget);

    q_dataQueryAndDisplay = new DataQueryAndDisplay(this);
    q_dataImportMgr = new DataImportMgr(this);
    q_databaseSampling = new DatabaseSampling(this);
    q_databaseMgr = new DatabaseMgr(this);
    q_ruleMgrWidget = new RuleMgrWidget(this);

    q_tabWidget->addTab(q_dataQueryAndDisplay, QStringLiteral("查询管理"));
    q_tabWidget->addTab(q_dataImportMgr, QStringLiteral("数据导入"));
    q_tabWidget->addTab(q_databaseSampling, QStringLiteral("数据编辑"));
    q_tabWidget->addTab(q_databaseMgr, QStringLiteral("信号筛选"));
    q_tabWidget->addTab(q_ruleMgrWidget, QStringLiteral("规则管理"));

    //
    connect(q_dataQueryAndDisplay, SIGNAL(tableItemDoubleClicked(JTableSelInfo)),
            SIGNAL(switchManagerToQuery(JTableSelInfo)));
}

bool DataMgrWidget::init()
{
    bool result = true;

    // 初始化查询和显示模块
    result = result && q_dataQueryAndDisplay->init();

    // 初始化数据入库管理模块
    result = result && q_dataImportMgr->init();

    // 初始化数据库采样管理模块
    result = result && q_databaseSampling->init();

    // 初始化数据库管理模块
    result = result && q_databaseMgr->init();

    // 初始化规则管理模块
    result = result && q_ruleMgrWidget->init();

    return true;
}

DataQueryAndDisplay *DataMgrWidget::dataQueryAndDisplay()
{
    return q_dataQueryAndDisplay;
}

DataImportMgr *DataMgrWidget::databaseMgr()
{
    return q_dataImportMgr;
}

RuleMgrWidget *DataMgrWidget::ruleMgrWidget()
{
    return q_ruleMgrWidget;
}
