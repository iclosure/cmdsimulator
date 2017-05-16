#include "precomp.h"
#include "siftareasettings.h"
#include "jxmltable.h"
#include "jsqldatamgr.h"

// - class SiftAreaSettings -

SiftAreaSettings::SiftAreaSettings(QWidget *parent) :
    QWidget(parent),
    q_enabledContentChange(true)
{
    QVBoxLayout *vertLayoutMain = new QVBoxLayout(this);
    vertLayoutMain->setContentsMargins(0, 0, 0, 0);
    vertLayoutMain->setSpacing(2);

    q_xmlTable = new JXmlTable(this);
    q_xmlTable->setWindowTitle(QStringLiteral("附加筛选约束条件"));
    vertLayoutMain->addWidget(q_xmlTable);

    // context-menu
    q_xmlTable->addAction(new QAction(QStringLiteral("删除"), q_xmlTable));

    QFormLayout *formLayoutBottom = new QFormLayout();
    formLayoutBottom->setContentsMargins(0, 0, 0, 0);
    vertLayoutMain->addLayout(formLayoutBottom);

    q_comboBoxSuffix = new QComboBox(this);
    formLayoutBottom->addRow(QStringLiteral("信号表："), q_comboBoxSuffix);

    q_comboBoxSymbol = new QComboBox(this);
    formLayoutBottom->addRow(QStringLiteral("信号量："), q_comboBoxSymbol);

    q_buttonAdd = new QPushButton(QStringLiteral("增加"), this);
    q_buttonAdd->setMinimumWidth(120);
    vertLayoutMain->addWidget(q_buttonAdd, 0, Qt::AlignRight);

    //
    connect(q_comboBoxSuffix, SIGNAL(currentIndexChanged(int)),
            SLOT(_emit_currentSuffixChanged(int)));
    connect(q_comboBoxSymbol, SIGNAL(currentIndexChanged(int)),
            SLOT(_emit_symbolChanged(int)));
    connect(q_xmlTable, SIGNAL(itemSelectionChanged()), SLOT(_emit_xmlTableItemSelectedChanged()));
    connect(q_xmlTable->actions().at(0), SIGNAL(triggered(bool)),
            SLOT(_emit_removeXmlTableItems()));
    connect(q_buttonAdd, SIGNAL(clicked(bool)), SLOT(_emit_buttonAddClicked()));

    //
    setEnabledContentChange(true);
}

bool SiftAreaSettings::init()
{
    bool result = true;

    //
    q_xmlTable->setConfig(QApplication::applicationDirPath()
                          .append("/../config/xmltable.xml"));
    q_xmlTable->setTableName(QStringLiteral("筛选区域约束"));
    q_xmlTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
    q_xmlTable->verticalHeader()->setDefaultSectionSize(25);

    //
    q_buttonAdd->setEnabled(false);

    return result;
}

void SiftAreaSettings::setFileNameInfo(const JFileNameInfo &fileNameInfo)
{
    // 清空旧数据
    this->clear();

    //
    q_fileNameInfo = fileNameInfo;

    // 获取所有表后缀名
    QStringList suffixes;
    if (JSqlDataMgr::instance()->readSubTableSuffix(fileNameInfo, suffixes)) {
        QStringListIterator citerSuffixes(suffixes);
        while (citerSuffixes.hasNext()) {
            const QString &suffix = citerSuffixes.next();
            q_comboBoxSuffix->addItem(suffix);
        }
        q_comboBoxSuffix->setCurrentIndex(0);
    }
}

void SiftAreaSettings::setBaseRange(const QRectF &range, bool notify)
{
    //
    q_comboBoxSymbol->clear();

    // 获取当前信号表名称
    const QString currentSignalTable = q_comboBoxSuffix->currentText();

    //
    if (currentSignalTable.isEmpty()) {
        return;
    }

    // 获取信号表所有信号量
    QList<stSignalInfo> signalInfos;
    if (!JSqlDataMgr::instance()->readSignalData(
                JSqlDataMgr::instance()->signalTableNamePrefix()
                .append(currentSignalTable), signalInfos)) {
        return;
    }

    //
    const bool oldEnabled = setEnabledContentChange(false);

    // 过滤已添加的信号量
    QListIterator<stSignalInfo> citerSignalInfos(signalInfos);
    while (citerSignalInfos.hasNext()) {
        const stSignalInfo &signalInfo = citerSignalInfos.next();
        //
        if (signalInfo.BM == QStringLiteral("时间")
                || signalInfo.BM == QStringLiteral("马赫数")) {
            continue;
        }
        //
        if (signalInfo.BM.isEmpty()) {
            q_comboBoxSymbol->addItem(signalInfo.YXHM % " (?)");
        } else {
            q_comboBoxSymbol->addItem(signalInfo.YXHM % " (" % signalInfo.BM % ")");
        }
    }

    //
    q_xmlTable->clearContents();

    //
    q_buttonAdd->setEnabled(q_comboBoxSymbol->count() > 0);

    //
    q_baseRange = range;

    //
    setEnabledContentChange(oldEnabled);

    //
    if (notify) {
        updateSiftArea();
    }
}

bool SiftAreaSettings::setEnabledContentChange(bool enabled)
{
    bool oldEnabled = q_enabledContentChange;

    disconnect(q_xmlTable, SIGNAL(contentChanged()), this, SLOT(updateSiftArea()));

    if (enabled) {
        connect(q_xmlTable, SIGNAL(contentChanged()), this, SLOT(updateSiftArea()));
    }

    q_enabledContentChange = enabled;

    return oldEnabled;
}

const JTableView *SiftAreaSettings::tableView() const
{
    return q_xmlTable;
}

void SiftAreaSettings::updateSiftArea()
{
    //
    const int rowCount = q_xmlTable->rowCount();

    // 统计表中的所有区域
    QMap<QString/*suffix*/, QMap<QString/*symbol*/, QPointF/*range*/> > mapRange;
    for (int row = 0; row < rowCount; ++row) {
        const QString suffix = q_xmlTable->itemData(row, 0, Qt::UserRole).toString();
        const QString symbol = q_xmlTable->itemData(row, 0, Qt::UserRole + 1).toString();
        const qreal center = q_xmlTable->itemData(row, 1).toReal();
        const qreal delta = q_xmlTable->itemData(row, 2).toReal();
        mapRange[suffix][symbol] = QPointF(center - delta, center + delta);
    }

    //
    if (mapRange.isEmpty()) {
        return;     //
    }

    // 读取并合并所有区域，取交集
    QRectF range;
    if (!JSqlDataMgr::instance()->readRecordDataV(mapRange, range)) {
        return;     // 获取失败
    }

    // 有效性检测
    if (range.isEmpty()) {
        return;     // 没有交集
    }

    // 合并区域，取交集
    range = range.intersected(q_baseRange);

    //
    if (range != q_baseRange) {
        q_baseRange = range;
        // 通知筛选区域已变更
        Q_EMIT siftAreaChanged(range);
    }
}

void SiftAreaSettings::clear()
{
    q_comboBoxSuffix->clear();
    q_comboBoxSymbol->clear();
    q_xmlTable->clearContents();
}

void SiftAreaSettings::_emit_currentSuffixChanged(int index)
{
    // 清空旧数据
    q_comboBoxSymbol->clear();

    //
    if (index == -1) {
        return;
    }

    // 获取当前表后缀名
    const QString currentSuffix = q_comboBoxSuffix->currentText();

    // 获取信号表所有信号量
    QList<stSignalInfo> signalInfos;
    if (!JSqlDataMgr::instance()->readSignalData(
                JSqlDataMgr::instance()->signalTableNamePrefix()
                .append(currentSuffix), signalInfos)) {
        return;
    }

    // 过滤已添加的信号量
    const int rowCount = q_xmlTable->rowCount();
    QListIterator<stSignalInfo> citerSignalInfos(signalInfos);
    while (citerSignalInfos.hasNext()) {
        const stSignalInfo &signalInfo = citerSignalInfos.next();
        //
        if (signalInfo.BM == QStringLiteral("时间")
                || signalInfo.BM == QStringLiteral("马赫数")) {
            continue;
        }
        //
        int i = 0;
        for (; i < rowCount; ++i) {
            const QString suffix = q_xmlTable->itemData(i, 0, Qt::UserRole).toString();
            if (suffix == currentSuffix) {
                const QString symbol = q_xmlTable->itemData(i, 0, Qt::UserRole + 1).toString();
                if (symbol == signalInfo.YXHM) {
                    break;
                }
            }
        }
        //
        if (i < rowCount) {
            continue;
        }
        //
        if (signalInfo.BM.isEmpty()) {
            q_comboBoxSymbol->addItem(signalInfo.YXHM % " (?)");
        } else {
            q_comboBoxSymbol->addItem(signalInfo.YXHM % " (" % signalInfo.BM % ")");
        }
    }

    //
    q_buttonAdd->setEnabled(q_comboBoxSymbol->count() > 0);
}

void SiftAreaSettings::_emit_symbolChanged(int index)
{
    //
    q_buttonAdd->setEnabled(index >= 0);
}

void SiftAreaSettings::_emit_xmlTableItemSelectedChanged()
{
    QList<JTableViewSelectionRange> selectionRanges = q_xmlTable->selectRanges();
    if (q_xmlTable->rowCount() == 0 || selectionRanges.isEmpty()) {
        q_xmlTable->setContextMenuPolicy(Qt::NoContextMenu);
    } else {
        q_xmlTable->setContextMenuPolicy(Qt::ActionsContextMenu);
    }
}

void SiftAreaSettings::_emit_removeXmlTableItems()
{
    // 获取选中的所有行
    const QList<JTableViewSelectionRange> selectRanges = q_xmlTable->selectRanges();
    if (selectRanges.isEmpty()) {
        return;     // 没有选中任何行
    }

    // 弹窗提示
    QMessageBox::StandardButtons buttons = (QMessageBox::StandardButtons)
            QMessageBox::warning(this, QStringLiteral("删除操作"),
                                 QStringLiteral("是否要删除选中的所有区域？"),
                                 QMessageBox::Yes | QMessageBox::No);
    if (buttons == QMessageBox::No) {
        return;     // 取消删除
    }

    //
    const bool oldEnabled = setEnabledContentChange(false);

    // 获取当前信号表名称
    const QString currentSignalTable = q_comboBoxSuffix->currentText();

    // 将选中项先加入下拉列表再从表中删除
    QListIterator<JTableViewSelectionRange> citerSelectRanges(selectRanges);
    while (citerSelectRanges.hasNext()) {
        const JTableViewSelectionRange &selectRange = citerSelectRanges.next();
        for (int row = selectRange.bottomRow(); row >= selectRange.topRow(); --row) {
            //
            const QString signalTable = q_xmlTable->itemData(row, 0, Qt::UserRole).toString();
            if (signalTable != currentSignalTable) {
                continue;
            }
            //
            const QString symbol = q_xmlTable->itemData(row, 0, Qt::UserRole + 1).toString();
            const QString name = q_xmlTable->itemData(row, 0, Qt::UserRole + 2).toString();
            if (name.isEmpty()) {
                q_comboBoxSymbol->addItem(symbol % " (?)");
            } else {
                q_comboBoxSymbol->addItem(symbol % " (" % name % ")");
            }
            //
            q_xmlTable->removeRow(row);
        }
    }

    //
    setEnabledContentChange(oldEnabled);

    //
    updateSiftArea();
}

void SiftAreaSettings::_emit_buttonAddClicked()
{
    const int currentIndex = q_comboBoxSymbol->currentIndex();
    if (currentIndex == -1) {
        return;
    }

    //
    const QString currentText = q_comboBoxSymbol->currentText();
    const QString symbol = currentText.section(" (", 0, 0);
    QString name = currentText.section(" (", 1).remove(")");
    if (name == "?") {
        name.clear();
    }

    // 获取表后缀名
    const QString suffix = q_comboBoxSuffix->currentText();

    // 获取M-H信号名称
    QString machSection, heightSection;
    if (!JFlyRecord::instance()->readSignalMHSection(
                JSqlDataMgr::instance()->signalTableNamePrefix().append(suffix),
                machSection, heightSection)) {
        return;
    }

    //
    QPointF range;
    if (!JSqlDataMgr::instance()->readRecordDataV(
                JSqlDataMgr::instance()->recordTableNamePrefix().append(suffix),
                machSection, heightSection,
                symbol, q_baseRange, range)) {
        return;     // 获取失败
    }

    //
    const bool oldEnabled = setEnabledContentChange(false);

    //
    const int rowIndex = q_xmlTable->rowCount();
    q_xmlTable->insertRow(rowIndex);

    // 信号名
    q_xmlTable->setItemData(rowIndex, 0, name.isEmpty() ? symbol : name);
    q_xmlTable->setItemData(rowIndex, 0, suffix, Qt::UserRole);
    q_xmlTable->setItemData(rowIndex, 0, symbol, Qt::UserRole + 1);
    q_xmlTable->setItemData(rowIndex, 0, name, Qt::UserRole + 2);
    q_xmlTable->setItemData(rowIndex, 0,
                            suffix + " => " + symbol + " (" +
                            (name.isEmpty() ? "?" : name) + ")", Qt::ToolTipRole);
    // 基准值
    q_xmlTable->setItemData(rowIndex, 1, (range.x() + range.y()) / 2);
    // 偏差值
    q_xmlTable->setItemData(rowIndex, 2, (range.y() - range.x()) / 2);

    //
    q_comboBoxSymbol->removeItem(currentIndex);

    //
    setEnabledContentChange(oldEnabled);
}
