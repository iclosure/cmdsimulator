#include "precomp.h"
#include <QDomDocument>
#include <QDomElement>
#include "dataquerywidget.h"
#include "jsplitter.h"
#include "jcurve.h"
#include "jcurvewidget.h"
#include "jcurveview.h"
#include "jxmltable.h"
#include "jfiltertableview.h"
#include "data_manager/dataqueryanddisplay.h"
#include "jsqldatamgr.h"
#include "jsqltablemgr.h"
#include "siftareasettings.h"
#include "globalconfig.h"
#include "jmyexcel.h"

DataQueryWidget::DataQueryWidget(QWidget *parent) :
    QWidget(parent),
    q_enabledContentChange(false)
{
    QHBoxLayout *horiLayoutMain = new QHBoxLayout(this);
    horiLayoutMain->setContentsMargins(2, 2, 2, 2);

    JSplitter *splitterMain = new JSplitter();
    splitterMain->setHandleWidth(6);
    splitterMain->setHandleColor(QColor(100, 100, 100, 30));
    splitterMain->setScales(QList<double>() << 1 << 2.5);
    horiLayoutMain->addWidget(splitterMain);

    // left - 查询条件

    q_widgetAreaLeft = new QWidget(this);
    splitterMain->addWidget(q_widgetAreaLeft);

    QVBoxLayout *vertLayoutLeft = new QVBoxLayout(q_widgetAreaLeft);
    vertLayoutLeft->setContentsMargins(0, 0, 0, 0);

    QGroupBox *groupBoxSift = new QGroupBox(QStringLiteral("查询条件"), q_widgetAreaLeft);
    vertLayoutLeft->addWidget(groupBoxSift);

    QVBoxLayout *vertLayoutSift = new QVBoxLayout(groupBoxSift);

    QFormLayout *formLayoutSift = new QFormLayout();
    vertLayoutSift->addLayout(formLayoutSift);

    q_comboBoxJX = new QComboBox(groupBoxSift);
    q_comboBoxJH = new QComboBox(groupBoxSift);
    q_dateEditMinRQ = new QDateEdit(groupBoxSift);
    q_dateEditMaxRQ = new QDateEdit(groupBoxSift);
    q_dateEditMinRQ->setCalendarPopup(true);
    q_dateEditMaxRQ->setCalendarPopup(true);
    q_dateEditMinRQ->setDisplayFormat(QStringLiteral("yyyy 年 MM 月 dd 日"));
    q_dateEditMaxRQ->setDisplayFormat(QStringLiteral("yyyy 年 MM 月 dd 日"));
    q_comboBoxJC = new QComboBox(groupBoxSift);

    formLayoutSift->addRow(QStringLiteral("机型："), q_comboBoxJX);
    formLayoutSift->addRow(QStringLiteral("机号："), q_comboBoxJH);

    QWidget *widgetRQ = new QWidget(groupBoxSift);
    QHBoxLayout *horiLayoutRQ = new QHBoxLayout(widgetRQ);
    horiLayoutRQ->setContentsMargins(0, 0, 0, 0);
    horiLayoutRQ->setSpacing(3);
    horiLayoutRQ->addWidget(q_dateEditMinRQ);
    QLabel *labelRQ = new QLabel(QStringLiteral("~"), widgetRQ);
    labelRQ->setFixedWidth(QFontMetrics(labelRQ->font()).width(labelRQ->text()));
    horiLayoutRQ->addWidget(labelRQ);
    horiLayoutRQ->addWidget(q_dateEditMaxRQ);
    formLayoutSift->addRow(QStringLiteral("日期："), widgetRQ);
    formLayoutSift->addRow(QStringLiteral("架次："), q_comboBoxJC);

    q_buttonUpdate = new QPushButton(QStringLiteral("查询并显示"), groupBoxSift);
    q_buttonUpdate->setMinimumWidth(120);
    vertLayoutSift->addWidget(q_buttonUpdate, 0, Qt::AlignRight);

    //
    groupBoxSift->adjustSize();
    groupBoxSift->setFixedHeight(groupBoxSift->sizeHint().height());

    // left - M-H区域微调

    QGroupBox *groupBoxAreaSettings = new QGroupBox(QStringLiteral("(M,H)区域调整"), q_widgetAreaLeft);
    vertLayoutLeft->addWidget(groupBoxAreaSettings);

    QVBoxLayout *vertLayoutAreaSettings = new QVBoxLayout(groupBoxAreaSettings);
    vertLayoutAreaSettings->setContentsMargins(2, 2, 2, 2);

    //
    q_tableArea = new JXmlTable(groupBoxAreaSettings);
    q_tableArea->setWindowTitle(QStringLiteral("(M,H)筛选区域"));
    q_tableArea->setFixedHeight(88);
    vertLayoutAreaSettings->addWidget(q_tableArea);

    //
    groupBoxAreaSettings->adjustSize();
    groupBoxAreaSettings->setFixedHeight(groupBoxAreaSettings->sizeHint().height());

    // left - 区域设置

    QGroupBox *groupBoxRestrain = new QGroupBox(QStringLiteral("区域约束条件"), q_widgetAreaLeft);
    vertLayoutLeft->addWidget(groupBoxRestrain);

    QVBoxLayout *vertLayoutRestrain = new QVBoxLayout(groupBoxRestrain);
    vertLayoutRestrain->setContentsMargins(2, 2, 2, 2);

    //
    q_siftAreaSettings = new SiftAreaSettings(groupBoxRestrain);
    vertLayoutRestrain->addWidget(q_siftAreaSettings);

    // right

    q_widgetAreaRight = new QWidget(this);
    splitterMain->addWidget(q_widgetAreaRight);

    QVBoxLayout *vertLayoutRight = new QVBoxLayout(q_widgetAreaRight);
    vertLayoutRight->setContentsMargins(0, 0, 0, 0);

    q_splitterTop = new JSplitter(Qt::Vertical, this);
    q_splitterTop->setHandleWidth(6);
    q_splitterTop->setHandleColor(QColor(100, 100, 100, 30));
    q_splitterTop->setScales(QList<double>() << 1.5 << 1);
    vertLayoutRight->addWidget(q_splitterTop);

    // right - top

    q_curveView = new JCurveWidget(q_splitterTop, true);
    q_splitterTop->addWidget(q_curveView);

    // right - bottom

    q_tabWidgetTable = new QTabWidget(q_splitterTop);
    q_splitterTop->addWidget(q_tabWidgetTable);

    q_buttonExport = new QPushButton(QStringLiteral("导出数据"), q_splitterTop);
    q_buttonExport->setMinimumWidth(120);
    q_buttonExport->setContentsMargins(0, 3, 3, 3);
    vertLayoutRight->addWidget(q_buttonExport, 0, Qt::AlignRight);

    //
    connect(q_comboBoxJX, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](){
        //
        updateCoord();
    });
    connect(q_comboBoxJH, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](){
        //
        updateCoord();
    });
    connect(q_dateEditMinRQ, &QDateEdit::dateChanged, [=](){
        //
        updateCoord();
    });
    connect(q_dateEditMaxRQ, &QDateEdit::dateChanged, [=](){
        //
        updateCoord();
    });
    connect(q_comboBoxJC, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](){
        //
        updateCoord();
        //
        updateSiftAreaSettings();
    });
    connect(q_buttonUpdate, SIGNAL(clicked(bool)), SLOT(_emit_buttonUpdateClicked()));
    connect(q_siftAreaSettings, &SiftAreaSettings::siftAreaChanged, [=](const QRectF &range){
        //
        q_curveView->addHotSpot(range, false);
        //
        updateSiftResult(true);
    });
    connect(q_curveView, SIGNAL(doubleClicked(bool)),
            this, SLOT(_emit_curveDoubleClicked()));
    connect(q_curveView, SIGNAL(selectedChanged(int,JCurve*)),
            this, SLOT(_emit_selectedChanged(int,JCurve*)));
    connect(q_curveView, SIGNAL(siftAreaChanged(int,QRectF)),
            this, SLOT(_emit_siftAreaChanged(int,QRectF)));
    connect(q_buttonExport, SIGNAL(clicked(bool)),
            this, SLOT(_emit_buttonExportClicked()));

    //
    setEnabledContentChange(true);

    //
    if (!GlobalConfig::instance()->canReadDatabase()) {
        q_buttonUpdate->hide();
    }

    //
    if (!GlobalConfig::instance()->canExportData()) {
        q_buttonExport->hide();
        q_curveView->setExportVisible(false);
    }
}

bool DataQueryWidget::init()
{
    bool result = true;

    //
    q_tableArea->setConfig(QApplication::applicationDirPath()
                           .append("/../config/xmltable.xml"));
    q_tableArea->setTableName(QStringLiteral("筛选区域调整"));
    q_tableArea->setSelectionMode(QAbstractItemView::NoSelection);
    q_tableArea->verticalHeader()->setStretchLastSection(true);
    q_tableArea->verticalHeader()->setDefaultSectionSize(30);

    //
    q_curveView->setEnableDbClick(true);
    q_curveView->setBaseLineVisible(true);
    q_curveView->setBaseLineEnabled(true);
    q_curveView->setAxisXYType(JCurveWidget::AxisXY_Value_Value);

    //
    if (GlobalConfig::instance()->canReadDatabase()) {
        result = result && q_siftAreaSettings->init();
    }

    //
    q_buttonExport->setEnabled(false);

    //
    updateBaseLine();

    return result;
}

JFileNameInfo DataQueryWidget::fileNameInfo() const
{
    // 获取筛选基本信息
    JFileNameInfo fileNameInfo;
    fileNameInfo.JX = DataQueryAndDisplay::parseSiftJX(q_comboBoxJX->currentText());
    fileNameInfo.JH = DataQueryAndDisplay::parseSiftJH(q_comboBoxJH->currentText());
    fileNameInfo.minRQ = q_dateEditMinRQ->date();
    fileNameInfo.maxRQ = q_dateEditMaxRQ->date();
    fileNameInfo.JC = DataQueryAndDisplay::parseSiftJC(q_comboBoxJC->currentText());
    return fileNameInfo;
}

void DataQueryWidget::addOnePageTable(const QString &suffix)
{
    // 添加一页表，并初始化表头
    JLiteTableView *tableView = new JLiteTableView(q_tabWidgetTable);
    tableView->tableView()->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //
    q_tabWidgetTable->addTab(tableView, suffix);

    //
    connect(tableView->tableView(), SIGNAL(itemDoubleClicked(QStandardItem*)),
            SLOT(_emit_tableItemDoubleClicked(QStandardItem*)));
    connect(tableView, SIGNAL(pageIndexChanged(int)),
            SLOT(_emit_tablePageIndexChanged(int)));
}

QList<QRectF> DataQueryWidget::siftAreas() const
{
    return q_curveView->siftAreas();
}

bool DataQueryWidget::setEnabledContentChange(bool enabled)
{
    bool oldEnabled = q_enabledContentChange;

    disconnect(q_tableArea, SIGNAL(contentChanged()),
               this, SLOT(_emit_tableAreaContentChanged()));

    if (enabled) {
        connect(q_tableArea, SIGNAL(contentChanged()),
                this, SLOT(_emit_tableAreaContentChanged()));
    }

    q_enabledContentChange = enabled;

    return oldEnabled;
}

bool DataQueryWidget::updateSift(const JTableSelInfo &info)
{
    if (!GlobalConfig::instance()->canReadDatabase()) {
        return true;
    }

    // 清空旧数据
    q_siftAreaSettings->clear();
    q_tableArea->clearContents();
    q_tabWidgetTable->clear();
    q_curveView->clearCurve();
    q_curveView->clearHotSpot();

    // JX
    q_comboBoxJX->clear();
    q_comboBoxJX->addItem(QStringLiteral("全部"));
    q_comboBoxJX->addItems(JSqlDataMgr::instance()->readAllJX());
    q_comboBoxJX->setCurrentText(info.baseInfo.JX);
    // JH
    q_comboBoxJH->clear();
    q_comboBoxJH->addItem(QStringLiteral("全部"));
    q_comboBoxJH->addItems(JSqlDataMgr::instance()->readAllJH());
    q_comboBoxJH->setCurrentText(info.baseInfo.JH);
    // RQ
    q_dateEditMinRQ->setDate(info.baseInfo.minRQ);
    q_dateEditMaxRQ->setDate(info.baseInfo.minRQ);
    // JC
    q_comboBoxJC->clear();
    q_comboBoxJC->addItem(QStringLiteral("全部"));
    q_comboBoxJC->addItems(JSqlDataMgr::instance()->readAllJCAsString());
    q_comboBoxJC->setCurrentText(QString::number(info.baseInfo.JC));

    //
    q_buttonUpdate->setEnabled(true);

    return true;
}

bool DataQueryWidget::updateCoord()
{
    // 清空旧数据

    // 获取筛选基本信息
    const JFileNameInfo fileNameInfo = this->fileNameInfo();

    // 设置筛选区域设置模块
    q_siftAreaSettings->setFileNameInfo(fileNameInfo);

    return true;
}

bool DataQueryWidget::updateBaseLine()
{
    if (!GlobalConfig::instance()->canReadDatabase()) {
        return true;
    }

    // 获取母线图数据点
    QList<QVector<QPointF> > groupsData;
    if (!JMyExcel::instance()->readBaseLine(groupsData)) {
        return false;
    }

    //
    q_curveView->setBaseLineSamples(groupsData);

    return true;
}

bool DataQueryWidget::updateCurve()
{
    if (!GlobalConfig::instance()->canReadDatabase()) {
        return true;
    }

    //
    JAutoCursor autoCursor(Qt::BusyCursor);

    // 清空旧数据
    q_curveView->clearCurve();
    q_curveView->clearHotSpot();

    // 获取筛选基本信息
    JFileNameInfo fileNameInfo = this->fileNameInfo();

    // 获取所有信号表
    QStringList suffixes;
    if (!JSqlDataMgr::instance()->readSubTableSuffix(fileNameInfo, suffixes)) {
        return false;
    }

    //
    if (suffixes.isEmpty()) {
        return false;
    }

    //
    QRectF range;
    QStringListIterator citerSuffixes(suffixes);
    while (citerSuffixes.hasNext()) {
        //
        const QString &suffix = citerSuffixes.next();

        // 获取M-H信号名称
        QString machSection, heightSection;
        if (!JFlyRecord::instance()->readSignalMHSection(
                    JSqlDataMgr::instance()->signalTableNamePrefix().append(suffix),
                    machSection, heightSection)) {
            continue;
        }

        // 获取记录数据
        QVector<QPointF> data;
        if (!JSqlDataMgr::instance()->readRecordData(
                    JSqlDataMgr::instance()->recordTableNamePrefix().append(suffix),
                    machSection, heightSection, data)) {
            continue;   // 获取失败
        }

        // 检测是否有数据需要处理
        if (data.isEmpty()) {
            continue;   //
        }

        // 增加曲线图
        JCurve *curve = q_curveView->addCurve(QString("M-H") % " (" % suffix % ")");
        q_curveView->updateData(curve, data);

        // 获取热点区域
        QList<QRectF> ranges;
        if (!JSqlDataMgr::instance()->readHotSpotData(
                    JSqlTableMgr::instance()->hotSpotTableNamePrefix().append(suffix),
                    ranges)) {
            return false;
        }

        // 检测参数有效性
        if (ranges.isEmpty()) {
            continue;
        }

        // 合并区域，取并集
        range = range.united(ranges.first());
    }

    // 添加热点范围
    q_curveView->addHotSpot(range);

    // 更新筛选结果
    updateSiftResult(false);

    //
    updateSiftAreaSettings();

    return true;
}

void DataQueryWidget::updateSiftAreaSettings()
{
    // 获取所有筛选区域
    QList<QRectF> siftAreas = q_curveView->siftAreas();
    if (siftAreas.isEmpty()) {
        return;     // 没有有效筛选区域
    }

    // 更新基准筛选区域
    q_siftAreaSettings->setBaseRange(siftAreas.first(), false);
}

bool DataQueryWidget::updateSiftResult(bool updateHotSpot)
{
    if (!GlobalConfig::instance()->canReadDatabase()) {
        return true;
    }

    //
    JAutoCursor autoCursor(Qt::BusyCursor);

    // 清空旧数据
    q_tabWidgetTable->clear();
    q_buttonExport->setEnabled(false);

    // 获取所有筛选区域
    QList<QRectF> siftAreas = q_curveView->siftAreas();
    if (siftAreas.isEmpty()) {
        return false;     // 没有有效筛选区域
    }

    // 获取筛选基本信息
    JFileNameInfo fileNameInfo = this->fileNameInfo();

    // 获取所有信号表
    QStringList suffixes;
    if (!JSqlDataMgr::instance()->readSubTableSuffix(fileNameInfo, suffixes)) {
        return false;
    }

    //
    QStringListIterator citerSuffixes(suffixes);
    while (citerSuffixes.hasNext()) {
        //
        const QString &suffix = citerSuffixes.next();

        // 获取M-H信号名称
        QString machSection, heightSection;
        if (!JFlyRecord::instance()->readSignalMHSection(
                    JSqlDataMgr::instance()->signalTableNamePrefix().append(suffix),
                    machSection, heightSection)) {
            continue;
        }

        // 生成表名
        const QString signalTableName = JSqlTableMgr::instance()->signalTableNamePrefix().append(suffix);
        const QString recordTableName = JSqlTableMgr::instance()->recordTableNamePrefix().append(suffix);
        const QString hotSpotTableName = JSqlTableMgr::instance()->hotSpotTableNamePrefix().append(suffix);

        // 读取对应信号表数据
        QStringList columnNames = JSqlDataMgr::instance()->readSignalTableColumnName(signalTableName);
        if (columnNames.isEmpty()) {
            continue;
        }

        // 增加一页数据列表
        addOnePageTable(QString(suffix).replace('_', '-'));

        // 获取增加的列表
        JLiteTableView *tableView =
                dynamic_cast<JLiteTableView *>(q_tabWidgetTable->widget(q_tabWidgetTable->count() - 1));
        if (!tableView) {
            continue;       // 获取失败
        }

        //
        tableView->setWindowTitle(signalTableName);
        tableView->tableView()->setWindowTitle(recordTableName);

        // 初始化表基本信息
        tableView->tableView()->setHorizontalHeaderLabels(columnNames);

        // 清除数据库中对应的旧的热点区域
        if (updateHotSpot
                && GlobalConfig::instance()->canWriteDatabase()
                && !JSqlTableMgr::instance()->clearTable(hotSpotTableName)) {
            continue;
        }

        //
        const QRectF &range = siftAreas.first();

        // 将热点区域入库
        if (updateHotSpot
                && GlobalConfig::instance()->canWriteDatabase()
                && !JSqlDataMgr::instance()->addHotSpotData(hotSpotTableName, range)) {
            continue;
        }

        //
        tableView->setProperty("machSection", machSection);
        tableView->setProperty("heightSection", heightSection);
        tableView->setProperty("range", range);

        //
        tableView->setTotalRowCount(JSqlDataMgr::instance()
                                    ->rowCountOfRecordDataV(recordTableName,
                                                            machSection,  heightSection, range));

    }

    //
    if (q_tabWidgetTable->count() > 0) {
        q_buttonExport->setEnabled(true);
    }

    //
    updateTableArea(siftAreas.first());

    return true;
}

void DataQueryWidget::updateTableArea(const QRectF &range)
{
    //
    setEnabledContentChange(false);

    //
    q_tableArea->clearContents();

    //
    q_tableArea->setRowCount(2);

    // 设置坐标名
    q_tableArea->setItemData(0, 0, QStringLiteral("X-马赫数"));
    q_tableArea->setItemData(1, 0, QStringLiteral("Y-高度"));

    // 设置坐标值
    q_tableArea->setItemData(0, 1, range.center().x());
    q_tableArea->setItemData(0, 2, range.width() / 2);
    q_tableArea->setItemData(1, 1, range.center().y());
    q_tableArea->setItemData(1, 2, range.height() / 2);

    //
    setEnabledContentChange(true);
}

void DataQueryWidget::_emit_buttonUpdateClicked()
{
    //
    updateCurve();
}

void DataQueryWidget::_emit_tableItemDoubleClicked(QStandardItem *item)
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
    const QString symbol = itemSymbol->text().section(' ', 0, 0);

    //
    JTableSelInfo info;
    info.baseInfo = this->fileNameInfo();
    info.symbol = symbol;

    //
    Q_EMIT switchQueryToAnalyze(info);
}

void DataQueryWidget::_emit_curveDoubleClicked()
{
    //
    bool _visible = q_widgetAreaLeft->isVisible();

    //
    q_widgetAreaLeft->setVisible(!_visible);
    q_tabWidgetTable->setVisible(!_visible);
    q_buttonExport->setVisible(!_visible);
}

void DataQueryWidget::_emit_selectedChanged(int index, JCurve *curve)
{
    Q_UNUSED(index);
    if (curve == 0) {
        return;
    }
#if 0
    const QPointF p = curve->sample(index);
    qDebug() << index << curve->title().text()
             << ": (" << p.x()
             << "," << p.y()
             << ")";
#endif
}

void DataQueryWidget::_emit_siftAreaChanged(int index, const QRectF &range)
{
    Q_UNUSED(index);
    Q_UNUSED(range);

    //
    updateSiftResult();

    //
    updateSiftAreaSettings();
}

void DataQueryWidget::_emit_buttonExportClicked()
{
    // 将数据生成报表
    QString filePath = QFileDialog::getSaveFileName(this, QStringLiteral("导出数据"),
                                                    QApplication::applicationDirPath().append("/../data")
                                                    .append(QStringLiteral("查询结果数据 - "))
                                                    .append(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss"))
                                                    .append(".xlsx"),
                                                    QStringLiteral("Excel Workbook (*.xlsx)"));
    if (filePath.isEmpty()) {
        return;
    }

    //
    JAutoCursor autoCursor(Qt::BusyCursor);

    // 常创建表单
    bool result = JMyExcel::instance()->create();
    if (!result) {
        qDebug() << "open excel file failure!";
    }

    // 设置表单标题
    result = JMyExcel::instance()->setTitle(fileNameInfo(),
                                            qMax(q_tableArea->columnCount(),
                                                 q_siftAreaSettings->tableView()->columnCount()) + 5);
    if (!result) {
        //
    }

    // 设置表单筛选区域数据
    result = JMyExcel::instance()->setSiftArea(q_tableArea);
    if (!result) {
        //
    }

    // 设置表单筛选区域数据
    result = JMyExcel::instance()->setSiftRestrain(q_siftAreaSettings->tableView());
    if (!result) {
        //
    }

    // 设置表单记录数据
    result = JMyExcel::instance()->setRecordData(q_tabWidgetTable);
    if (!result) {
        //
    }

    // 设置表单标题
    result = JMyExcel::instance()->saveAs(filePath);
    if (!result) {
        //
    }

    // 关闭表单
    result = JMyExcel::instance()->close();
    if (!result) {
        //
    }
#if defined(Q_OS_WIN) || defined(Q_WS_WIN)
    //
    int button = QMessageBox::information(this, QStringLiteral("提示"),
                                          QStringLiteral("数据已转换完成，是否需要呈现？"),
                                          QMessageBox::Yes | QMessageBox::No);
    if (button == QMessageBox::Yes) {
        // 显示生成的Excel表界面
        QFileInfo fileIfnfo(filePath);
        fileIfnfo.absoluteDir().cd(fileIfnfo.absolutePath());
        QProcess::startDetached("explorer.exe", QStringList() << "/select,"
                                + QDir::toNativeSeparators(filePath));
    }
#endif
}

void DataQueryWidget::_emit_tableAreaContentChanged()
{
    //
    const QPointF center = QPointF(q_tableArea->itemData(0, 1).toReal(),
                                   q_tableArea->itemData(1, 1).toReal());
    const QPointF delta = QPointF(q_tableArea->itemData(0, 2).toReal(),
                                  q_tableArea->itemData(1, 2).toReal());
    const QRectF range = QRectF(center.x() - delta.x(),
                                center.y() - delta.y(),
                                delta.x() * 2, delta.y() * 2);
    //
    q_curveView->addHotSpot(range, false);

    //
    updateSiftResult(true);
}

void DataQueryWidget::_emit_tablePageIndexChanged(int)
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
    // 读取处于热点区域的数据点
    QList<QVariant> data;
    if (!JSqlDataMgr::instance()->readRecordDataV(DYZB,
                                                  tableView->property("machSection").toString(),
                                                  tableView->property("heightSection").toString(),
                                                  tableView->property("range").toRectF(), columnSymbols,
                                                  data,
                                                  rowOffset,
                                                  tableView->pageRowCount(),
                                                  columnSymbols.first())) {
        return;       // 读取失败
    }

    // 设置行数
    tableView->tableView()->setRowCount(data.count());

    // 设置表数据
    int rowIndex = 0;
    QStringList vLabels;
    QListIterator<QVariant> citer(data);
    while (citer.hasNext()) {
        const QVariantList &rowVars = citer.next().toList();
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
