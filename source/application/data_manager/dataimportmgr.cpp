#include "precomp.h"
#include "dataimportmgr.h"
#include "jsplitter.h"
#include "jtableview.h"
#include "jflyrecord.h"
#include "jruledict.h"
#include "rulemgrwidget.h"
#include "../globalconfig.h"

// - class DataImportMgr -

DataImportMgr::DataImportMgr(QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout *horiLayoutMain = new QHBoxLayout(this);
    horiLayoutMain->setContentsMargins(2, 2, 2, 2);

    JSplitter *splitter = new JSplitter(this);
    splitter->setHandleWidth(6);
    splitter->setHandleColor(QColor(100, 100, 100, 30));
    splitter->setScales(QList<double>() << 1 << 2);
    horiLayoutMain->addWidget(splitter);

    QWidget *widgetLeftArea = new QWidget(this);
    splitter->addWidget(widgetLeftArea);

    QVBoxLayout *vertLayoutLeft = new QVBoxLayout(widgetLeftArea);
    vertLayoutLeft->setContentsMargins(2, 2, 2, 2);

    // 规则模式

    QGroupBox *groupBoxRuleMode = new QGroupBox(QStringLiteral("规则模式设置"), this);
    vertLayoutLeft->addWidget(groupBoxRuleMode);

    QHBoxLayout *horiLayoutRuleMode = new QHBoxLayout(groupBoxRuleMode);

    q_radioButtonGlobalRule = new QRadioButton(QStringLiteral("按全局提供"), groupBoxRuleMode);
    q_radioButtonJXRule = new QRadioButton(QStringLiteral("按机型提供"), groupBoxRuleMode);
    horiLayoutRuleMode->addWidget(q_radioButtonGlobalRule);
    horiLayoutRuleMode->addWidget(q_radioButtonJXRule);

    // adjustSize

    // 数据文件导入

    QGroupBox *groupBoxImport = new QGroupBox(QStringLiteral("数据文件导入"), this);
    vertLayoutLeft->addWidget(groupBoxImport);

    QVBoxLayout *vertLayoutImport = new QVBoxLayout(groupBoxImport);
    vertLayoutImport->setContentsMargins(2, 2, 2, 2);

    q_buttonImport = new QPushButton(QStringLiteral("导入文件"), groupBoxImport);
    q_buttonImport->setMinimumWidth(150);
    vertLayoutImport->addWidget(q_buttonImport, 0, Qt::AlignRight);

    JSplitter *splitterFilePaths = new JSplitter(Qt::Vertical, groupBoxImport);
    splitterFilePaths->setHandleWidth(6);
    splitterFilePaths->setHandleColor(QColor(100, 100, 100, 30));
    splitterFilePaths->setScales(QList<double>() << 1 << 1);
    vertLayoutImport->addWidget(splitterFilePaths);

    q_treeWidgetFilePaths = new QTreeWidget(groupBoxImport);
    q_treeWidgetFilePaths->setHeaderLabel(QStringLiteral("数据文件"));
    splitterFilePaths->addWidget(q_treeWidgetFilePaths);

    q_tableSymbols = new JTableView(groupBoxImport);
    splitterFilePaths->addWidget(q_tableSymbols);

    // 变量筛选

    QWidget *widgetRightArea = new QWidget(this);
    splitter->addWidget(widgetRightArea);

    //

    QVBoxLayout *vertLayoutRight = new QVBoxLayout(widgetRightArea);
    vertLayoutRight->setContentsMargins(2, 2, 2, 2);

    QGroupBox *groupBoxSift = new QGroupBox(QStringLiteral("变量筛选"), widgetRightArea);
    vertLayoutRight->addWidget(groupBoxSift);

    QHBoxLayout *horiLayoutSift = new QHBoxLayout(groupBoxSift);
    horiLayoutSift->setContentsMargins(2, 2, 2, 2);

    JSplitter *splitterSift = new JSplitter(groupBoxSift);
    splitterSift->setHandleWidth(6);
    splitterSift->setHandleColor(QColor(100, 100, 100, 30));
    splitterSift->setScales(QList<double>() << 1.5 << 1);
    horiLayoutSift->addWidget(splitterSift);

    q_treeWidgetSiftBefore = new QTreeWidget(groupBoxSift);
    q_treeWidgetSiftAfter = new QTreeWidget(groupBoxSift);
    splitterSift->addWidget(q_treeWidgetSiftBefore);
    splitterSift->addWidget(q_treeWidgetSiftAfter);

    //
    QHBoxLayout *horiLayoutSave = new QHBoxLayout();
    vertLayoutRight->addLayout(horiLayoutSave);

    horiLayoutSave->addStretch();

    QFormLayout *formLayoutSaveMode = new QFormLayout();
    formLayoutSaveMode->setContentsMargins(0, 0, 0, 0);
    q_comboBoxImportMode = new QComboBox(widgetRightArea);
    q_comboBoxImportMode->setMinimumWidth(120);
    q_comboBoxImportMode->addItem(QStringLiteral("覆盖"));
    q_comboBoxImportMode->addItem(QStringLiteral("融合"));
    formLayoutSaveMode->addRow(QStringLiteral("导入方式："), q_comboBoxImportMode);
    horiLayoutSave->addLayout(formLayoutSaveMode);

    horiLayoutSave->addSpacing(20);

    q_checkBoxIgnoreSift = new QCheckBox(QStringLiteral("忽略筛选"), widgetRightArea);
    horiLayoutSave->addWidget(q_checkBoxIgnoreSift);

    horiLayoutSave->addSpacing(20);

    q_buttonSave = new QPushButton(QStringLiteral("存入数据库"), widgetRightArea);
    horiLayoutSave->addWidget(q_buttonSave);
    if (!GlobalConfig::instance()->canImportData()) {
        q_buttonSave->hide();
    }

    //
    connect(q_radioButtonGlobalRule, &QRadioButton::clicked, this, &DataImportMgr::_emit_ruleModeChanged);
    connect(q_radioButtonJXRule, &QRadioButton::clicked, this, &DataImportMgr::_emit_ruleModeChanged);
    connect(q_buttonImport, &QPushButton::clicked, this, &DataImportMgr::_emit_buttonImportClicked);
    connect(q_treeWidgetFilePaths, &QTreeWidget::itemSelectionChanged,
            this, &DataImportMgr::_emit_treeFilePathsItemSelectChanged);
    connect(q_treeWidgetSiftBefore, &QTreeWidget::itemChanged, this, &DataImportMgr::updateTreeWidgetState);
    connect(q_comboBoxImportMode, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](){
        //
        if (GlobalConfig::instance()->canImportData()) {
            q_buttonSave->setEnabled(true);
        }
    });
    connect(q_checkBoxIgnoreSift, &QCheckBox::stateChanged, [=](){
        if (GlobalConfig::instance()->canImportData()) {
            q_buttonSave->setEnabled(true);
        }
    });
    connect(q_buttonSave, &QPushButton::clicked, this, &DataImportMgr::_emit_buttonSaveClicked);

    //
    q_futureWatcher = new QFutureWatcher<bool>(this);

    //
    connect(q_futureWatcher, SIGNAL(finished()), SLOT(onImportFinished()));

    //
    q_buttonSave->setEnabled(false);

    //
    if (!GlobalConfig::instance()->canImportData()) {
        q_buttonImport->hide();
        q_buttonSave->hide();
    }
}

bool DataImportMgr::init()
{
    //
    q_radioButtonGlobalRule->setChecked(true);
    q_comboBoxImportMode->setCurrentIndex(0);   // 默认采用覆盖模式
    q_checkBoxIgnoreSift->setChecked(true);    // 默认不忽略筛选

    //
    q_tableSymbols->setVisible(false);
    q_tableSymbols->setColumnCount(1);
    q_tableSymbols->horizontalHeader()->setStretchLastSection(true);
    q_tableSymbols->setHorizontalHeaderLabels(QStringList() << QStringLiteral("信号名称"));
    q_tableSymbols->setEditTriggers(QTreeView::NoEditTriggers);

    //
    q_treeWidgetSiftBefore->setColumnCount(2);
    q_treeWidgetSiftBefore->setColumnWidth(0, 400);
    q_treeWidgetSiftBefore->setHeaderLabels(QStringList()
                                            << QStringLiteral("信号量（筛选前）")
                                            << QStringLiteral("别名"));

    //
    q_treeWidgetSiftAfter->setColumnCount(2);
    q_treeWidgetSiftAfter->setColumnWidth(0, 400);
    q_treeWidgetSiftAfter->setHeaderLabels(QStringList()
                                           << QStringLiteral("信号量（筛选后）")
                                           << QStringLiteral("别名"));

    //
    q_buttonSave->setEnabled(false);

    return true;
}

void DataImportMgr::_emit_ruleModeChanged()
{
    // 更新信号量筛选前树
    updateTreeWidgetSiftBefore();

    // 更新信号量筛选后树
    updateTreeWidgetSiftAfter();

    // 先隐藏信号列表
    q_tableSymbols->setVisible(false);
}

void DataImportMgr::_emit_buttonImportClicked()
{
    QStringList filePaths = QFileDialog::getOpenFileNames(this, QStringLiteral("选择数据文件"),
                                                          QApplication::applicationDirPath().append("/../data"),
                                                          "Text File (*.txt);;Database File (*.db)");
    if (filePaths.isEmpty()) {
        return;
    }

    // 初始化文件展示树
    updateTreeWidgetFilePaths(filePaths);

    // 更新信号量筛选前树
    updateTreeWidgetSiftBefore();

    // 更新信号量筛选后树
    updateTreeWidgetSiftAfter();

    // 先隐藏信号列表
    q_tableSymbols->setVisible(false);
}

void DataImportMgr::_emit_treeFilePathsItemSelectChanged()
{
    // 先隐藏信号列表
    q_tableSymbols->setVisible(false);

    // 清空列表内容
    q_tableSymbols->clearContents();

    // 获取文件路径树当前选中节点
    QTreeWidgetItem *currentTreeItem = q_treeWidgetFilePaths->currentItem();
    if (!currentTreeItem) {
        return;         // 选中节点无效
    }

    // 获取节点类型
    QVariant type = currentTreeItem->data(0, Qt::UserRole);
    if (type.isNull()) {
        return;         // 节点类型无效
    }

    // 只处理文件路径类型节点
    if (type.toUInt() != TreeItemFilePath) {
        return;         // 条件不满足
    }

    // 获取文件路径
    QString filePath = currentTreeItem->data(0, Qt::UserRole + 1).toString();

    // 解析文件头信息（信号行）
    QStringList symbols;
    if (!JFlyRecord::instance()->parseFileHeader(filePath, symbols)) {
        return;         // 解析失败
    }

    // 检查是否有信号需要显示
    if (symbols.isEmpty()) {
        return;         // 没有
    }

    // 初始化表头
    q_tableSymbols->setHorizontalHeaderLabels(
                QStringList() << QStringLiteral("%1 # 信号量")
                .arg(QFileInfo(filePath).fileName()));

    // 初始化
    int rowIndex = 0;
    q_tableSymbols->setRowCount(symbols.count());
    foreach (const QString &symbol, symbols) {
        q_tableSymbols->setItemData(rowIndex++, 0, symbol);
    }

    // 显示信号列表
    q_tableSymbols->setVisible(true);
}

void DataImportMgr::_emit_buttonSaveClicked()
{
    //
    int result = QMessageBox::warning(this, QStringLiteral("数据导入"),
                                      QStringLiteral("数据库将被修改！是否继续？"),
                                      QStringLiteral("继续"), QStringLiteral("取消"));
    if (result == 1) {
        return; // 取消修改
    }
    // 获取隐藏的根节点
    QTreeWidgetItem *invRootItem = q_treeWidgetSiftBefore->invisibleRootItem();
    if (invRootItem == 0) {
        return;     // 不存在
    }

    //
    JAutoCursor autoCursor(Qt::BusyCursor);

    /// 获取信号量

    QMap<QString/*JX*/, QList<stSignalSiftInfo> > mapJXSift;

    // 机型节点
    int countJX = invRootItem->childCount();
    for (int i = 0; i < countJX; ++i) {
        // 获取节点
        QTreeWidgetItem *itemJX = invRootItem->child(i);
        if (itemJX == 0) {
            continue;       // 节点无效
        }
        // 获取机型
        QString JX = itemJX->data(0, Qt::UserRole + 1).toString();
        // 信号量节点
        QList<stSignalSiftInfo> signalSifts;
        int symbolCount = itemJX->childCount();
        for (int j = 0; j < symbolCount; ++j) {
            QTreeWidgetItem *itemSymbol = itemJX->child(j);
            if (itemSymbol == 0) {
                continue;   // 结点无效
            }
            //
            stSignalSiftInfo signalSift;
            signalSift.YXHM = itemSymbol->text(0);
            signalSift.BM = itemSymbol->text(1);
            if (signalSift.BM == "<?>") {
                signalSift.BM.clear();
            }
            signalSift.SXBZ = (itemSymbol->checkState(0) == Qt::Checked);
            //
            signalSifts.append(signalSift);
        }
        //
        mapJXSift[JX] = signalSifts;
    }

    /// 数据入库

    // 获取规则模式
    bool isGlobalMode = q_radioButtonGlobalRule->isChecked();

    // 获取数据导入标志
    int importMode = q_comboBoxImportMode->currentIndex();

    // 获取忽略筛选标志
    bool isIgnoreSift = q_checkBoxIgnoreSift->isChecked();

    // 显示忙界面
    QMetaObject::invokeMethod(static_cast<QWidget *>(qApp->property("mainWindow").value<void *>()),
                              "setBusyIndicatorVisible", Q_ARG(bool, true));

    //
    QFuture<bool> future = QtConcurrent::run(taskImportData,
                                             this,
                                             isGlobalMode,
                                             importMode,
                                             isIgnoreSift,
                                             mapJXSift);
    q_futureWatcher->setFuture(future);

    //
    q_buttonSave->setEnabled(false);
}

void DataImportMgr::updateTreeWidgetState(QTreeWidgetItem *item, int column)
{
    bool diff = false;

    if(column != 0 && column != -1) {
        return;
    }

    //
    if(item->childCount() != 0
            && item->checkState(0) != Qt::PartiallyChecked
            && column !=-1 ) {
        Qt::CheckState checkState = item->checkState(0);
        for (int i = 0; i < item->childCount(); ++i) {
            item->child(i)->setCheckState(0, checkState);
        }
    } else if (item->childCount() == 0 || column == -1) {
        if(item->parent() == 0) {
            return;
        }

        //
        for (int j = 0; j < item->parent()->childCount(); ++j) {
            if(j != item->parent()->indexOfChild(item)
                    && item->checkState(0) != item->parent()->child(j)->checkState(0)) {
                diff = true;
            }
        }

        //
        if(diff) {
            item->parent()->setCheckState(0, Qt::PartiallyChecked);
        } else {
            item->parent()->setCheckState(0, item->checkState(0));
        }

        if(item->parent() != 0) {
            updateTreeWidgetState(item->parent(), -1);
        }
    }

    //
    updateTreeWidgetSiftAfter();
}

void DataImportMgr::onImportFinished()
{
    // 显示忙界面
    QMetaObject::invokeMethod(static_cast<QWidget *>(qApp->property("mainWindow").value<void *>()),
                              "setBusyIndicatorVisible", Q_ARG(bool, false));

    //
    if (!q_futureWatcher->result()) {
        QMessageBox::warning(this, QStringLiteral("导入结果提示"),
                             QStringLiteral("导入失败！失败原因请查看最新日志文件"));
    } else {
        QMessageBox::information(this, QStringLiteral("导入结果提示"),
                                 QStringLiteral("导入成功！"));
    }
}

void DataImportMgr::updateTreeWidgetFilePaths(const QStringList &filePaths)
{
    // 清空树
    q_treeWidgetFilePaths->clear();

    // 参数有效性检测
    if (filePaths.isEmpty()) {
        return;     // 没有可处理的文件
    }

    // 获取隐藏的根节点
    QTreeWidgetItem *invRootItem = q_treeWidgetFilePaths->invisibleRootItem();
    if (!invRootItem) {
        return;     // 不存在
    }

    // 按机型分类文件
    QMap<QString/*JX*/, QStringList/*filePaths*/> mapFilePaths;

    // 初始化树表
    QStringListIterator citerFilePaths(filePaths);
    while (citerFilePaths.hasNext()) {
        const QString filePath = citerFilePaths.next();
        // 解析文件基本信息
        QString JX;
        if (!JFlyRecord::instance()->parseFileName(filePath, JX)) {
            continue;       // 失败。继续
        }
        // 按机型分类文件
        mapFilePaths[JX].append(filePath);
    }

    // 初始化树节点
    QMapIterator<QString, QStringList> citerMapFilePaths(mapFilePaths);
    while (citerMapFilePaths.hasNext()) {
        citerMapFilePaths.next();
        // 挂载机型节点
        QTreeWidgetItem *itemJX = new QTreeWidgetItem(
                    QStringList() << QStringLiteral("机型：") % citerMapFilePaths.key());
        itemJX->setData(0, Qt::UserRole, TreeItemJX);
        itemJX->setData(0, Qt::UserRole + 1, citerMapFilePaths.key());
        invRootItem->addChild(itemJX);
        foreach (const QString &filePath, citerMapFilePaths.value()) {
            // 挂载文件名称节点
            QTreeWidgetItem *itemFileName = new QTreeWidgetItem(
                        QStringList()<< QStringLiteral("文件：") % QFileInfo(filePath).fileName());
            itemFileName->setToolTip(0, filePath);
            itemFileName->setData(0, Qt::UserRole, TreeItemFilePath);
            itemFileName->setData(0, Qt::UserRole + 1, filePath);
            itemJX->addChild(itemFileName);
        }
    }

    // 展开所有节点
    q_treeWidgetFilePaths->expandAll();
}

void DataImportMgr::updateTreeWidgetSiftBefore()
{
    // 清空树
    q_treeWidgetSiftBefore->clear();

    // 获取隐藏的根节点
    QTreeWidgetItem *invRootItemFilePaths = q_treeWidgetFilePaths->invisibleRootItem();
    if (!invRootItemFilePaths) {
        return;     // 不存在
    }

    // 按机型分类文件
    QMap<QString/*JX*/, QStringList/*filePaths*/> mapFilePaths;

    // 获取选中的文件
    int JXCount = invRootItemFilePaths->childCount();
    for (int i = 0; i < JXCount; ++i) {
        QTreeWidgetItem *itemJX = invRootItemFilePaths->child(i);
        if (!itemJX) {
            continue;       // 节点无效
        }
        // 获取机型名称
        QString JX = itemJX->data(0, Qt::UserRole + 1).toString();
        if (JX.isEmpty()) {
            continue;   // 机型无效
        }
        //
        int fileNameCount = itemJX->childCount();
        for (int j = 0; j < fileNameCount; ++j) {
            QTreeWidgetItem *itemFileName = itemJX->child(j);
            if (!itemFileName) {
                continue;   // 节点无效
            }
            mapFilePaths[JX].append(itemFileName->data(0, Qt::UserRole + 1).toString());
        }
    }

    // 检查执行条件
    if (mapFilePaths.isEmpty()) {
        return;     // 条件不满足
    }

    // 获取规则模式
    bool isGlobalMode = q_radioButtonGlobalRule->isChecked();

    // 获取隐藏的根节点
    QTreeWidgetItem *invRootItemSiftBefore = q_treeWidgetSiftBefore->invisibleRootItem();
    if (!invRootItemSiftBefore) {
        return;     // 不存在
    }

    // 初始化树节点
    QMapIterator<QString, QStringList> citerMapFilePaths(mapFilePaths);
    while (citerMapFilePaths.hasNext()) {
        citerMapFilePaths.next();
        const QString &JX = citerMapFilePaths.key();
        // 统计信号量与各文件的关系（取并集）
        QMap<QString/*symbol*/, QStringList/*fileNames*/> mapSymbolFileName;
        foreach (const QString &filePath, citerMapFilePaths.value()) {
            // 解析文件信号量
            QStringList symbols;
            if (!JFlyRecord::instance()->parseFileHeader(filePath, symbols)) {
                continue;   // 解析失败
            }
            // 去掉时间信号量（说明：时间信号量在每个数据文件中都存在，不能进行筛选）
            symbols.removeFirst();
            // 统计信号量
            const QString &fileName = QFileInfo(filePath).fileName();
            foreach (const QString &symbol, symbols) {
                if (mapSymbolFileName.contains(symbol)) {
                    // 已存在。再看文件名是否已映射
                    QStringList &filePaths = mapSymbolFileName[symbol];
                    if (!filePaths.contains(fileName)) {
                        filePaths.append(fileName);     // 不存在，需要映射
                    }
                } else {
                    // 不存在。直接建立新的映射
                    mapSymbolFileName[symbol].append(fileName);
                }
            }
        }
        // 挂载机型节点
        QTreeWidgetItem *itemJX = new QTreeWidgetItem(
                    QStringList() << QStringLiteral("机型：") % JX);
        itemJX->setData(0, Qt::UserRole, TreeItemJX);
        itemJX->setData(0, Qt::UserRole + 1, JX);
        itemJX->setFlags(itemJX->flags() | Qt::ItemIsUserCheckable);
        invRootItemSiftBefore->addChild(itemJX);
        // 检查执行条件
        if (mapSymbolFileName.isEmpty()) {
            itemJX->setText(0, QStringLiteral("机型：") % JX % QStringLiteral(" (无信号量)"));
            continue;       // 条件不满足
        }
        QMapIterator<QString, QStringList> citerMapSymbolFilePath(mapSymbolFileName);
        while (citerMapSymbolFilePath.hasNext()) {
            citerMapSymbolFilePath.next();
            const QString &symbol = citerMapSymbolFilePath.key();
            // 获取信号量的别名
            QString name = JRuleDict::instance()->nameOfSymbol(JX, symbol);
            if (name.isEmpty() && isGlobalMode) {
                name = JRuleDict::instance()->nameOfSymbol(symbol);
            }
            if (name.isEmpty()) {
                name = "<?>";
            }
            // 挂载信号量节点
            QTreeWidgetItem *itemSymbol = new QTreeWidgetItem(
                        QStringList() << symbol << name);
            itemSymbol->setToolTip(0, citerMapSymbolFilePath.value().join('\n'));
            itemSymbol->setData(0, Qt::UserRole, TreeItemSymbol);
            itemSymbol->setData(0, Qt::UserRole + 1, symbol);
            itemSymbol->setFlags(itemSymbol->flags() | Qt::ItemIsUserCheckable);
            itemJX->addChild(itemSymbol);
        }

        // 默认选上
        itemJX->setCheckState(0, Qt::Checked);
    }

    // 默认不选中没有别名的信号量
    int countJX = invRootItemSiftBefore->childCount();
    for (int i = 0; i < countJX; ++i) {
        QTreeWidgetItem *itemJX = invRootItemSiftBefore->child(i);
        if (!itemJX) {
            continue;
        }
        //
        int countSymbol = itemJX->childCount();
        for (int j = 0; j < countSymbol; ++j) {
            QTreeWidgetItem *itemSymbol = itemJX->child(j);
            if (!itemSymbol) {
                continue;
            }
            if (itemSymbol->text(1) == "<?>") {
                itemSymbol->setCheckState(0, Qt::Unchecked);
            }
        }
    }

    // 展开所有节点
    q_treeWidgetSiftBefore->expandAll();
}

void DataImportMgr::updateTreeWidgetSiftAfter()
{
    // 清空树
    q_treeWidgetSiftAfter->clear();

    // 获取隐藏的根节点
    QTreeWidgetItem *invRootItemSiftBefore = q_treeWidgetSiftBefore->invisibleRootItem();
    if (!invRootItemSiftBefore) {
        return;     // 不存在
    }

    // 获取隐藏的根节点
    QTreeWidgetItem *invRootItemSiftAfter= q_treeWidgetSiftAfter->invisibleRootItem();
    if (!invRootItemSiftAfter) {
        return;     // 不存在
    }

    //
    int JXCount = invRootItemSiftBefore->childCount();
    for (int i = 0; i < JXCount; ++i) {
        QTreeWidgetItem *itemJXBefore = invRootItemSiftBefore->child(i);
        if (!itemJXBefore) {
            continue;       // 节点无效
        }
        // 检查节点是否选为勾选状态
        if (itemJXBefore->checkState(0) == Qt::Unchecked) {
            continue;   // 不是
        }
        // 挂载机型节点
        QTreeWidgetItem *itemJXAfter = new QTreeWidgetItem(QStringList()
                                                           << itemJXBefore->text(0)
                                                           << itemJXBefore->text(1));
        itemJXAfter->setToolTip(0, itemJXBefore->toolTip(0));
        itemJXAfter->setData(0, Qt::UserRole, itemJXBefore->data(0, Qt::UserRole));
        itemJXAfter->setData(0, Qt::UserRole + 1, itemJXBefore->data(0, Qt::UserRole + 1));
        invRootItemSiftAfter->addChild(itemJXAfter);
        int symbolCountBefore = itemJXBefore->childCount();
        for (int j = 0; j < symbolCountBefore; ++j) {
            QTreeWidgetItem *itemSymbolBefore = itemJXBefore->child(j);
            if (!itemSymbolBefore) {
                continue;   // 结点无效
            }
            // 检查节点是否选为勾选状态
            if (itemSymbolBefore->checkState(0) == Qt::Unchecked) {
                continue;   // 不是
            }
            // 挂载信号量节点
            QTreeWidgetItem *itemSymbolAfter = new QTreeWidgetItem(QStringList()
                                                                   << itemSymbolBefore->text(0)
                                                                   << itemSymbolBefore->text(1));
            itemSymbolAfter->setToolTip(0, itemSymbolBefore->toolTip(0));
            itemSymbolAfter->setData(0, Qt::UserRole, itemSymbolBefore->data(0, Qt::UserRole));
            itemSymbolAfter->setData(0, Qt::UserRole + 1, itemSymbolBefore->data(0, Qt::UserRole + 1));
            itemJXAfter->addChild(itemSymbolAfter);
        }
    }

    // 展开所有节点
    q_treeWidgetSiftAfter->expandAll();

    //
    q_buttonSave->setEnabled(true);
}

QStringList DataImportMgr::filePathsOfJX(const QString &JX) const
{
    // 获取隐藏的根节点
    QTreeWidgetItem *invRootItemFilePaths = q_treeWidgetFilePaths->invisibleRootItem();
    if (!invRootItemFilePaths) {
        return QStringList();     // 不存在
    }

    QTreeWidgetItem *itemJX = 0;
    int count = invRootItemFilePaths->childCount();
    for (int i = 0; i < count; ++i) {
        QTreeWidgetItem *tmpItemJX = invRootItemFilePaths->child(i);
        if (tmpItemJX == 0) {
            continue;
        }

        //
        if (tmpItemJX->data(0, Qt::UserRole + 1) == JX) {
            itemJX = tmpItemJX;
            break;  // 找到
        }
    }

    //
    if (itemJX == 0) {
        return QStringList();
    }

    QStringList filePaths;
    count = itemJX->childCount();
    for (int i = 0; i < count; ++i) {
        QTreeWidgetItem *itemFileName = itemJX->child(i);
        if (itemFileName == 0) {
            continue;
        }

        //
        filePaths.append(itemFileName->data(0, Qt::UserRole + 1).toString());
    }

    return filePaths;
}

bool DataImportMgr::taskImportData(DataImportMgr *_this,
                                   bool isGlobalMode,
                                   int importMode,
                                   bool isIgnoreSift,
                                   const QMap<QString, QList<stSignalSiftInfo> > &mapJXSift)
{
    //
    QMapIterator<QString/*JX*/, QList<stSignalSiftInfo> > citerMapJXSifts(mapJXSift);
    while (citerMapJXSifts.hasNext()) {
        citerMapJXSifts.next();
        const QList<stSignalSiftInfo> &signalSifts = citerMapJXSifts.value();

        // 获取指定机型下的所有文件名称
        QStringList filePaths = _this->filePathsOfJX(citerMapJXSifts.key());
        if (filePaths.isEmpty()) {
            continue;       // 没有需要处理的文件
        }

        // 文件分类（按主键特性）
        QList<QStringList> classifiedFilePaths;
        if (!JFlyRecord::instance()->classifyFile(filePaths, classifiedFilePaths)) {
            continue;       // 分类失败
        }

        // 按文件名分类存储
        QListIterator<QStringList> citerClassifiedFilePaths(classifiedFilePaths);
        while (citerClassifiedFilePaths.hasNext()) {
            const QStringList &_filePaths = citerClassifiedFilePaths.next();
            if (_filePaths.isEmpty()) {
                continue;   // 没有文件需要处理
            }

            // 如果需要覆盖保存，则先清除旧数据
            if (importMode == 0) {
                // 解析文件名
                JFileNameInfo fileNameInfo;
                if (!JFlyRecord::instance()->parseFileName(_filePaths.first(), fileNameInfo)) {
                    continue;       // 解析失败
                }
                // 清除旧数据
                if (!JSqlDataMgr::instance()->deleteAllRecordData(fileNameInfo)) {
                    qCritical() << QStringLiteral("清空数据表[%1]失败！").arg(fileNameInfo.toString());
                    continue;
                }
            }

            // 保存（更新）对应信号表（值表）数据
            if (!JFlyRecord::instance()->parseFile(_filePaths, signalSifts, isGlobalMode, isIgnoreSift)) {
                continue;
            }
        }
    }

    return true;
}
