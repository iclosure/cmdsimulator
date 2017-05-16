#include "precomp.h"
#include "dataqueryanddisplay.h"
#include "jxmltable.h"
#include "jsqltablemgr.h"
#include "jflyrecord.h"
#include "jprecitimer.h"
#include "globalconfig.h"

JLiteTableView::JLiteTableView(QWidget *parent)
    : QWidget(parent)
    , q_pageRowCount(1000)
    , q_totalRowCount(0)
{
    QVBoxLayout *vertLayoutMain = new QVBoxLayout(this);
    vertLayoutMain->setContentsMargins(0, 0, 0, 0);
    vertLayoutMain->setSpacing(0);

    q_tableView = new JTableView(this);
    q_tableView->setObjectName("tableView");
    q_tableView->horizontalHeader()->setDefaultSectionSize(100);
    q_tableView->verticalHeader()->setDefaultSectionSize(30);
    q_tableView->verticalHeader()->setMinimumWidth(100);
    vertLayoutMain->addWidget(q_tableView);

    QHBoxLayout *horiLayoutBottom = new QHBoxLayout();
    horiLayoutBottom->setContentsMargins(2, 2, 2, 2);
    horiLayoutBottom->setSpacing(10);
    vertLayoutMain->addLayout(horiLayoutBottom);

    q_buttonPrev = new QPushButton(QStringLiteral("上一页"), this);
    q_buttonNext = new QPushButton(QStringLiteral(" 下一页"), this);
    q_spinBoxPage = new QSpinBox(this);
    q_spinBoxPage->setButtonSymbols(QAbstractSpinBox::NoButtons);
    q_labelTotalRowCount = new QLabel(this);

    horiLayoutBottom->addStretch();
    horiLayoutBottom->addWidget(q_buttonPrev);
    horiLayoutBottom->addWidget(q_spinBoxPage);
    horiLayoutBottom->addWidget(q_buttonNext);
    horiLayoutBottom->addWidget(q_labelTotalRowCount);
    horiLayoutBottom->addStretch();

    //
    connect(q_buttonPrev, &QPushButton::clicked, [=](){
        q_spinBoxPage->setValue(q_spinBoxPage->value() - 1);
    });
    connect(q_buttonNext, &QPushButton::clicked, [=](){
        q_spinBoxPage->setValue(q_spinBoxPage->value() + 1);
    });
    connect(q_spinBoxPage, SIGNAL(valueChanged(int)),
            SIGNAL(pageIndexChanged(int)));
}

void JLiteTableView::setTotalRowCount(int count)
{
    q_totalRowCount = count;
    if (count > 0) {
        q_spinBoxPage->setRange(1, (q_totalRowCount / q_pageRowCount)
                                + ((q_totalRowCount % q_pageRowCount) ? 1 : 0));
    } else {
        q_spinBoxPage->setRange(0, 0);
    }
    q_spinBoxPage->setSuffix(QString(" / %1").arg(q_spinBoxPage->maximum()));
    q_labelTotalRowCount->setText(QString(QStringLiteral("总行数：%1"))
                                  .arg(count));
    //
    setCurrentPage(1);
}

void JLiteTableView::setCurrentPage(int page)
{
    q_spinBoxPage->setValue(page);
}

JTableView *JLiteTableView::tableView()
{
    return q_tableView;
}

int JLiteTableView::rowOffset() const
{
    return (q_spinBoxPage->value() - 1) *  q_pageRowCount; }

int JLiteTableView::pageCount() const
{
    return q_spinBoxPage->maximum();
}

int JLiteTableView::pageRowCount() const
{
    return q_pageRowCount;
}

int JLiteTableView::totalRowCount() const
{
    return q_totalRowCount;
}

DataQueryAndDisplay::DataQueryAndDisplay(QWidget *parent) :
    QWidget(parent),
    q_updateTableEnabled(true)
{
    QHBoxLayout *horiLayoutMain = new QHBoxLayout(this);
    horiLayoutMain->setContentsMargins(2, 2, 2, 2);

    JSplitter *splitter = new JSplitter(this);
    splitter->setHandleWidth(6);
    splitter->setHandleColor(QColor(100, 100, 100, 30));
    splitter->setScales(QList<double>() << 1 << 3);
    horiLayoutMain->addWidget(splitter);

    // left

    QGroupBox *groupBox = new QGroupBox(QStringLiteral("查询"), this);
    splitter->addWidget(groupBox);

    QVBoxLayout *vertLayoutGroup = new QVBoxLayout(groupBox);
    vertLayoutGroup->setContentsMargins(3, vertLayoutGroup->contentsMargins().top(), 3, 3);

    q_toolBoxSift = new QToolBox(groupBox);
    q_toolBoxSift->layout()->setSpacing(3);
    vertLayoutGroup->addWidget(q_toolBoxSift);

    q_listWidgetJX = new QListWidget(this);
    q_toolBoxSift->addItem(q_listWidgetJX, QStringLiteral("机型"));

    q_listWidgetJH = new QListWidget(this);
    q_toolBoxSift->addItem(q_listWidgetJH, QStringLiteral("机号"));

    q_listWidgetRQ = new QListWidget(this);
    q_toolBoxSift->addItem(q_listWidgetRQ, QStringLiteral("日期"));

    q_listWidgetJC = new QListWidget(this);
    q_toolBoxSift->addItem(q_listWidgetJC, QStringLiteral("架次"));

    QHBoxLayout *horiLayoutButton = new QHBoxLayout();
    vertLayoutGroup->addLayout(horiLayoutButton);

    horiLayoutButton->addStretch();

    q_buttonReload = new QPushButton(QStringLiteral("加载数据库"), this);
    q_buttonReload->setMinimumWidth(100);
    horiLayoutButton->addWidget(q_buttonReload);

    q_buttonUpdate = new QPushButton(QStringLiteral("更新查询"), this);
    q_buttonUpdate->setMinimumWidth(100);
    horiLayoutButton->addWidget(q_buttonUpdate);

    // right
    q_tabWidgetTable = new QTabWidget(this);
    q_tabWidgetTable->setObjectName("tabWidgetTable");
    splitter->addWidget(q_tabWidgetTable);

    //
    connect(q_listWidgetJX, &QListWidget::currentTextChanged, [=](const QString &text) {
        updateToolBoxItemText(0, QStringLiteral("机型"), text); });
    connect(q_listWidgetJH, &QListWidget::currentTextChanged, [=](const QString &text) {
        updateToolBoxItemText(1, QStringLiteral("机号"), text); });
    connect(q_listWidgetRQ, &QListWidget::currentTextChanged, [=](const QString &text) {
        updateToolBoxItemText(2, QStringLiteral("日期"), text); });
    connect(q_listWidgetJC, &QListWidget::currentTextChanged, [=](const QString &text) {
        updateToolBoxItemText(3, QStringLiteral("架次"), text); });
    //
    connect(q_buttonReload, &QPushButton::clicked, [=](){
        if (!GlobalConfig::instance()->canReadDatabase()) {
            return;
        }
        // 禁用数据更新
        bool oldEnabled = setUpdateTableEnabled(false);
        //
        q_tabWidgetTable->clear();
        //
        updateSift();
        // 还原数据更新
        setUpdateTableEnabled(oldEnabled);
    });
    //
    connect(q_buttonUpdate, &QPushButton::clicked, [=](){
        //q_buttonUpdate->setEnabled(false);
        updateTable();
    });

    //
    setUpdateTableEnabled(true);

    //
    if (!GlobalConfig::instance()->canReadDatabase()) {
        q_buttonReload->hide();
        q_buttonUpdate->hide();
    }
}

bool DataQueryAndDisplay::init()
{
    //
    JAutoCursor autoCursor(Qt::BusyCursor);

    bool result = true;

    // 禁用数据更新
    bool oldEnabled = setUpdateTableEnabled(false);

    //
    if (GlobalConfig::instance()->canReadDatabase()) {
        updateSift();
    }

    // 还原数据更新
    setUpdateTableEnabled(oldEnabled);

    //
    //q_buttonUpdate->setEnabled(false);

    return result;
}

bool DataQueryAndDisplay::setUpdateTableEnabled(bool enabled)
{
    if (!GlobalConfig::instance()->canReadDatabase()) {
        q_updateTableEnabled = false;
        return false;
    }

    bool oldEnabled = q_updateTableEnabled;
    q_updateTableEnabled = enabled;
    return oldEnabled;
}

void DataQueryAndDisplay::addOnePageTable(const QString &suffix)
{
    //
    QString DYXHB = JSqlTableMgr::signalTableNamePrefix().append(suffix);

    // 读取对应信号表数据
    QStringList columnNames = JSqlDataMgr::instance()->readSignalTableColumnName(DYXHB);
    if (columnNames.isEmpty()) {
        return;
    }

    //
    QString DYZB = JSqlTableMgr::recordTableNamePrefix().append(suffix);

    // 添加一页表，并初始化表头
    JLiteTableView *tableView = new JLiteTableView(q_tabWidgetTable);
    tableView->setWindowTitle(JSqlTableMgr::signalTableNamePrefix().append(suffix));
    tableView->tableView()->setWindowTitle(DYZB);
    tableView->tableView()->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->tableView()->setHorizontalHeaderLabels(columnNames);
    q_tabWidgetTable->addTab(tableView, QString(suffix).replace('_', '-'));

    //
    connect(tableView->tableView(), SIGNAL(itemDoubleClicked(QStandardItem*)),
            SLOT(_emit_tableItemDoubleClicked(QStandardItem*)));
    connect(tableView, SIGNAL(pageIndexChanged(int)),
            SLOT(_emit_tablePageIndexChanged(int)));

    //
    tableView->setTotalRowCount(JSqlTableMgr::instance()->rowCountOfTable(DYZB));
}

QString DataQueryAndDisplay::currentJX() const
{
    QListWidgetItem *item = q_listWidgetJX->currentItem();
    if (item == 0) {
        return QString::null;
    }

    return item->text();
}

QString DataQueryAndDisplay::currentJH() const
{
    QListWidgetItem *item = q_listWidgetJH->currentItem();
    if (item == 0) {
        return QString::null;
    }

    return item->text();
}

QString DataQueryAndDisplay::currentRQ() const
{
    QListWidgetItem *item = q_listWidgetRQ->currentItem();
    if (item == 0) {
        return QString::null;
    }

    return item->text();
}

QString DataQueryAndDisplay::currentJC() const
{
    QListWidgetItem *item = q_listWidgetJC->currentItem();
    if (item == 0) {
        return QString::null;
    }

    return item->text();
}

QString DataQueryAndDisplay::parseSiftJX(const QString &text)
{
    return (text == QStringLiteral("全部")) ? "" : text;
}

QString DataQueryAndDisplay::parseSiftJH(const QString &text)
{
    return (text == QStringLiteral("全部")) ? "" : text;
}

QString DataQueryAndDisplay::parseSiftRQ(const QString &text)
{
    return (text == QStringLiteral("全部")) ? ""
                                          : QDate::fromString(text, QStringLiteral("yyyy年MM月dd日"))
                                            .toString("yyyy/MM/dd");
}

int DataQueryAndDisplay::parseSiftJC(const QString &text)
{
    return (text == QStringLiteral("全部")) ? -1 : text.toInt();
}

bool DataQueryAndDisplay::updateSift()
{
    if (!GlobalConfig::instance()->canReadDatabase()) {
        return false;
    }

    // 禁用数据更新
    bool oldEnabled = setUpdateTableEnabled(false);

    //
    q_toolBoxSift->setCurrentIndex(0);

    // 获取所有机型
    q_listWidgetJX->clear();
    q_listWidgetJX->addItem(QStringLiteral("全部"));
    q_listWidgetJX->addItems(JSqlDataMgr::instance()->readAllJX());
    q_listWidgetJX->setCurrentRow(0);

    // 获取所有机号
    q_listWidgetJH->clear();
    q_listWidgetJH->addItem(QStringLiteral("全部"));
    q_listWidgetJH->addItems(JSqlDataMgr::instance()->readAllJH());
    q_listWidgetJH->setCurrentRow(0);

    // 获取所有日期
    q_listWidgetRQ->clear();
    q_listWidgetRQ->addItem(QStringLiteral("全部"));
    foreach (const QDate &date, JSqlDataMgr::instance()->readAllRQ()) {
        q_listWidgetRQ->addItem(date.toString(QStringLiteral("yyyy年MM月dd日")));
    }
    q_listWidgetRQ->setCurrentRow(0);

    // 获取所有架次
    q_listWidgetJC->clear();
    q_listWidgetJC->addItem(QStringLiteral("全部"));
    foreach (int item, JSqlDataMgr::instance()->readAllJC()) {
        q_listWidgetJC->addItem(QString::number(item));
    }
    q_listWidgetJC->setCurrentRow(0);

    // 启用数据更新
    setUpdateTableEnabled(oldEnabled);

    //
    updateTable();

    return true;
}

bool DataQueryAndDisplay::updateTable()
{
    if (!GlobalConfig::instance()->canReadDatabase()) {
        return false;
    }

    if (!q_updateTableEnabled) {
        return false;
    }

    //
    JAutoCursor autoCursor(Qt::BusyCursor);

    // 清空显示数据
    q_tabWidgetTable->clear();

    QListWidgetItem *currentItemJX = q_listWidgetJX->currentItem();
    QListWidgetItem *currentItemJH = q_listWidgetJH->currentItem();
    QListWidgetItem *currentItemRQ = q_listWidgetRQ->currentItem();
    QListWidgetItem *currentItemJC = q_listWidgetJC->currentItem();

    //
    if (currentItemJX == 0
            || currentItemJH == 0
            || currentItemRQ == 0
            || currentItemJC == 0) {
        return false;
    }

    // 解析架次信息
    int nJC = -1;   // 默认读取全部架次数据（小于0表示“全部”）
    {
        QString sTemp = currentItemJC->text();
        if (sTemp != QStringLiteral("全部")) {
            nJC = sTemp.toInt();
        }
    }

    // 读取指定的对应信号表和值表
    QStringList suffixes;
    bool result = JSqlDataMgr::instance()->readSubTableSuffix(
                parseSiftJX(currentItemJX->text()),
                parseSiftJH(currentItemJH->text()),
                parseSiftRQ(currentItemRQ->text()),
                parseSiftJC(currentItemJC->text()),
                suffixes);
    if (!result) {
        qCritical() << QStringLiteral("读取指定的对应信号表和值表失败！");
        return false;
    }

    //
    if (suffixes.isEmpty()) {
        return true;    // 没有数据可读取
    }

    // 读取所有表数据并显示
    QStringListIterator citerSuffixes(suffixes);
    while (citerSuffixes.hasNext()) {
        addOnePageTable(citerSuffixes.next());
    }

    return true;
}

void DataQueryAndDisplay::_emit_tableItemDoubleClicked(QStandardItem *item)
{
    //
    if (item == 0) {
        return;     //
    }

    //
    JTableView *tableView = reinterpret_cast<JTableView *>(sender());
    if (tableView == 0) {
        return;     //
    }

    // 获取选中列别名
    QStandardItem *itemSymbol = tableView->sourceModel()->horizontalHeaderItem(item->column());
    if (itemSymbol == 0) {
        return;
    }
    QString nameOfSymbol = itemSymbol->text();

    // 获取当前表名
    QString tableName = q_tabWidgetTable->tabText(q_tabWidgetTable->currentIndex());

    // 解析名称
    JFileNameInfo fileNameInfo;
    if (!JFlyRecord::instance()->parseFileName(tableName, fileNameInfo)) {
        return;     // 解析失败
    }

    //
    JTableSelInfo info;
    info.baseInfo = fileNameInfo;

    // 读取别名对应的信号量名称
    if (!JSqlDataMgr::instance()->symbolOfName(fileNameInfo, nameOfSymbol, info.symbol)) {
        info.symbol = nameOfSymbol;     // 获取失败，则以别名代替信号名称（可能信号量没有对应的别名）
    }

    //
    Q_EMIT tableItemDoubleClicked(info);
}

void DataQueryAndDisplay::_emit_tablePageIndexChanged(int)
{
    JAutoCursor autoCursor(Qt::BusyCursor);

    JLiteTableView *tableView = qobject_cast<JLiteTableView *>(sender());
    if (tableView == 0) {
        return;
    }


    //
    tableView->tableView()->clearContents();

    //
    QString DYXHB = tableView->windowTitle();
    QString DYZB = tableView->tableView()->windowTitle();

    // 读取对应信号表数据
    QStringList columnSymbols = JSqlDataMgr::instance()->readSignalTableColumnSymbol(DYXHB);
    if (columnSymbols.isEmpty()) {
        return;
    }

    //
    int rowOffset = tableView->rowOffset();

    // 读取对应值表数据
    QList<QVariantList> data;
    if (!JSqlDataMgr::instance()->readRecordData(DYZB,
                                                 data,
                                                 columnSymbols,
                                                 rowOffset,
                                                 tableView->pageRowCount(),
                                                 columnSymbols.first())) {
        return;
    }
    // 设置行数
    tableView->tableView()->setRowCount(data.count());

    // 设置表数据
    int rowIndex = 0;
    QStringList vLabels;
    QListIterator<QVariantList> citer(data);
    while (citer.hasNext()) {
        const QVariantList &rowVars = citer.next();
        //
        vLabels.append(QString::number(rowOffset + rowIndex + 1));
        //
        int columnCount = rowVars.count();
        for (int c = 0; c < columnCount; ++c) {
            const QVariant &varient = rowVars[c];
            tableView->tableView()->setItemData(rowIndex, c, varient);
        }
        ++rowIndex;
    }
    //
    tableView->tableView()->setVerticalHeaderLabels(vLabels);
}

void DataQueryAndDisplay::updateToolBoxItemText(int index, const QString &prefix, const QString &value)
{
    //q_buttonUpdate->setEnabled(true);
    q_toolBoxSift->setItemText(index, prefix % (value.isEmpty()
                                                ? "" : (QString("  <%1>").arg(value))));

    //
    updateTable();

    //
    //q_buttonUpdate->setEnabled(false);
}
