#include "precomp.h"
#include "centralwidget.h"
#include "data_manager/datamgrwidget.h"
#include "data_manager/dataqueryanddisplay.h"
#include "data_query/dataquerywidget.h"
#include "data_analyse/dataanalysewidget.h"

CentralWidget::CentralWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *horiLayoutMain = new QHBoxLayout(this);
    horiLayoutMain->setContentsMargins(0, 0, 0, 0);

    q_stackedWidget = new QStackedWidget(this);
    horiLayoutMain->addWidget(q_stackedWidget);

    q_dataMgrWidget = new DataMgrWidget(this);
    q_dataQueryWidget = new DataQueryWidget(this);
    q_dataAnalyseWidget = new DataAnalyseWidget(this);

    q_stackedWidget->addWidget(q_dataMgrWidget);
    q_stackedWidget->addWidget(q_dataQueryWidget);
    q_stackedWidget->addWidget(q_dataAnalyseWidget);

    //
    connect(q_dataMgrWidget, &DataMgrWidget::switchManagerToQuery, [=](const JTableSelInfo &info){
        q_stackedWidget->setCurrentIndex(1);
        //
        DataQueryAndDisplay *dataQueryAndDisplay = q_dataMgrWidget->dataQueryAndDisplay();
        if (dataQueryAndDisplay == 0) {
            Q_ASSERT(false);
            return;
        }
        // 更新查询模式下的筛选信息
        q_dataQueryWidget->updateSift(info);

        //
        Q_EMIT currentModelChanged(QStringLiteral("数据查询"));
    });
    //
    connect(q_dataQueryWidget, &DataQueryWidget::switchQueryToAnalyze, [=](const JTableSelInfo &info){
        q_stackedWidget->setCurrentIndex(2);
        //
        q_dataAnalyseWidget->clearCurves();
        q_dataAnalyseWidget->clearCoords();
        // 设置筛选区域
        q_dataAnalyseWidget->setSiftAreas(q_dataQueryWidget->siftAreas());
        // 更新分析模式下的筛选信息
        q_dataAnalyseWidget->updateSift(info);

        //
        Q_EMIT currentModelChanged(QStringLiteral("数据分析"));
    });

    //
    q_stackedWidget->setCurrentIndex(0);

    // initialize
}

bool CentralWidget::init()
{
    bool result = true;

    // 初始化数据管理模块
    result = result && q_dataMgrWidget->init();

    // 初始化数据查询模块
    result = result && q_dataQueryWidget->init();

    // 初始化数据分析模块
    result = result && q_dataAnalyseWidget->init();

    return result;
}

void CentralWidget::setCurrentModel(const QString &model)
{
    int currentIndex = q_stackedWidget->currentIndex();
    if (model == QStringLiteral("数据管理")) {
        if (currentIndex == 0) {
            return;     // 模式未改变
        }
        //
        q_dataAnalyseWidget->hidePopupWidget();
        //
        q_stackedWidget->setCurrentIndex(0);
    } else if (model == QStringLiteral("数据查询")) {
        if (currentIndex == 1) {
            return;     // 模式未改变
        }
        //
        q_dataAnalyseWidget->hidePopupWidget();
        //
        q_stackedWidget->setCurrentIndex(1);
    } else if (model == QStringLiteral("数据分析")) {
        if (currentIndex == 2) {
            return;     // 模式未改变
        }
        q_stackedWidget->setCurrentIndex(2);
    }

    //
    Q_EMIT currentModelChanged(model);
}

void CentralWidget::setCurrentIndex(const QString &model)
{
    int currentIndex = q_stackedWidget->currentIndex();
    if (model == QStringLiteral("数据管理")) {
        if (currentIndex == 0) {
            return;     // 模式未改变
        }
        //
        q_dataAnalyseWidget->hidePopupWidget();
        //
        q_stackedWidget->setCurrentIndex(0);
    } else if (model == QStringLiteral("数据查询")) {
        if (currentIndex == 1) {
            return;     // 模式未改变
        }
        //
        q_dataAnalyseWidget->hidePopupWidget();
        //
        q_stackedWidget->setCurrentIndex(1);
    } else if (model == QStringLiteral("数据分析")) {
        if (currentIndex == 2) {
            return;     // 模式未改变
        }
        q_stackedWidget->setCurrentIndex(2);
    }
}
