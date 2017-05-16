#include "precomp.h"
#include "databasemgr.h"
#include "../globalconfig.h"

// - class DatabaseMgr -

DatabaseMgr::DatabaseMgr(QWidget *parent) :
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
    vertLayoutLeft->setSpacing(6);
    vertLayoutLeft->setContentsMargins(2, 2, 2, 2);

    // left

    q_toolBoxSignalTables = new QToolBox(widgetLeftArea);
    q_toolBoxSignalTables->layout()->setSpacing(3);
    vertLayoutLeft->addWidget(q_toolBoxSignalTables);

    q_listWidgetSignalTable = new QListWidget(q_toolBoxSignalTables);
    q_toolBoxSignalTables->addItem(q_listWidgetSignalTable, QStringLiteral("信号表名"));

    q_buttonReload = new QPushButton(QStringLiteral("重新加载数据库"), widgetLeftArea);
    q_buttonReload->setMinimumWidth(250);
    vertLayoutLeft->addWidget(q_buttonReload, 0, Qt::AlignRight);

    // right

    QWidget *widgetRightArea = new QWidget(this);
    splitter->addWidget(widgetRightArea);

    QVBoxLayout *vertLayoutRight = new QVBoxLayout(widgetRightArea);
    vertLayoutRight->setContentsMargins(2, 2, 2, 2);
    vertLayoutRight->setSpacing(6);

    q_tableSignalSiftBefore = new JFilterTableView(widgetRightArea);
    vertLayoutRight->addWidget(q_tableSignalSiftBefore);

    QHBoxLayout *horiLayoutBottom = new QHBoxLayout();
    vertLayoutRight->addLayout(horiLayoutBottom);

    //
    horiLayoutBottom->addWidget(new QLabel(QStringLiteral("注：上表中橘黄色行号的为选中状态")));

    horiLayoutBottom->addStretch();

    q_checkSelectAll = new QCheckBox(QStringLiteral("全选"), widgetRightArea);
    q_buttonRestore = new QPushButton(QStringLiteral("还原"), widgetRightArea);
    q_buttonSave = new QPushButton(QStringLiteral("更新到数据库"), widgetRightArea);
    q_buttonRestore->setMinimumWidth(200);
    q_buttonSave->setMinimumWidth(200);

    if (!GlobalConfig::instance()->canWriteDatabase()) {
        q_buttonSave->hide();
    }

    horiLayoutBottom->addWidget(q_checkSelectAll);
    horiLayoutBottom->addSpacing(10);
    horiLayoutBottom->addWidget(q_buttonRestore);
    horiLayoutBottom->addWidget(q_buttonSave);

    //
    connect(q_listWidgetSignalTable, &QListWidget::currentTextChanged, [=](const QString &text) {
        updateToolBoxItemText(0, QStringLiteral("信号表名"), text);
    });
    connect(q_buttonReload, &QPushButton::clicked, [=](){
        // 更新信号表列表
        updateSignalTable();
    });
    //
    connect(q_tableSignalSiftBefore->view(), SIGNAL(itemSelectionChanged()),
            SLOT(_emit_tableSignalSiftSelectChanged()));
    connect(q_checkSelectAll, &QPushButton::toggled, [=](bool checked){
        //
        signalSiftSelectAll(checked);
    });
    connect(q_buttonRestore, &QPushButton::clicked, [=](){
        //
        updateTableSignalSift();
    });
    connect(q_buttonSave, &QPushButton::clicked, [=](){
        //
        int result = QMessageBox::warning(this, QStringLiteral("筛选信号量"),
                                          QStringLiteral("数据库将被修改！是否继续？"),
                                          QStringLiteral("继续"), QStringLiteral("取消"));
        if (result == 1) {
            return; // 取消修改
        }
        //
        _emit_buttonSaveClicked();
    });

    //
    if (!GlobalConfig::instance()->canReadDatabase()) {
        q_buttonReload->hide();
        q_buttonRestore->hide();
    }
    if (!GlobalConfig::instance()->canWriteDatabase()) {
        q_buttonSave->hide();
    }
}

bool DatabaseMgr::init()
{
    // tableView
    q_tableSignalSiftBefore->view()->setConfig(QApplication::applicationDirPath()
                                               .append("/../config/xmltable.xml"));
    q_tableSignalSiftBefore->view()->setTableName(QStringLiteral("信号筛选表"));
    q_tableSignalSiftBefore->view()->setSelectionMode(QAbstractItemView::MultiSelection);

    //
    updateSignalTable();

    return true;
}

void DatabaseMgr::updateSignalTable()
{
    if (!GlobalConfig::instance()->canReadDatabase()) {
        return;
    }

    // 清空旧数据
    q_listWidgetSignalTable->clear();

    //
    QStringList signalTables = JSqlDataMgr::instance()->readAllSignalTableName();

    //
    QStringListIterator citerSignalTables(signalTables);
    while (citerSignalTables.hasNext()) {
        const QString &signalTable = citerSignalTables.next();
        //
        q_listWidgetSignalTable->addItem(signalTable.section('_', 2));
    }

    //
    q_listWidgetSignalTable->setCurrentRow(0);
}

void DatabaseMgr::updateTableSignalSift()
{
    if (!GlobalConfig::instance()->canReadDatabase()) {
        return;
    }

    //
    q_tableSignalSiftBefore->view()->clearContents();

    //
    QListWidgetItem *item = q_listWidgetSignalTable->currentItem();
    if (!item) {
        return; // empty
    }

    //
    QString tableName = JSqlTableMgr::instance()->signalTableNamePrefix()
            .append(item->text());

    // 获取所有信号量信息
    QList<stSignalInfo> signalInfos;
    if (!JSqlDataMgr::instance()->readAllSignalData(tableName, signalInfos)) {
        return;
    }

    //
    signalInfos.removeFirst();      // 不对时间信号进行筛选
    int rowCount = signalInfos.count();
    q_tableSignalSiftBefore->view()->setRowCount(rowCount);
    for (int i = 0; i < rowCount; ++i) {
        const stSignalInfo &signalInfo = signalInfos[i];
        //
        q_tableSignalSiftBefore->view()->setItemData(i, 0, signalInfo.YXHM);
        q_tableSignalSiftBefore->view()->setItemData(i, 1, signalInfo.BM.isEmpty()
                                                     ? "<?>" : signalInfo.BM);
        if (signalInfo.SXBZ) {
            q_tableSignalSiftBefore->view()->setItemSelected(q_tableSignalSiftBefore->view()->item(i, 0), true);
            q_tableSignalSiftBefore->view()->setItemSelected(q_tableSignalSiftBefore->view()->item(i, 1), true);
        }
    }
}

void DatabaseMgr::signalSiftSelectAll(bool bSelectAll)
{
    //
    QStandardItem *item = 0;
    int rowCount = q_tableSignalSiftBefore->view()->rowCount();
    for (int i = 0; i < rowCount; ++i) {
        // column 0
        item = q_tableSignalSiftBefore->view()->item(i, 0);
        if (item) {
            q_tableSignalSiftBefore->view()->setItemSelected(item, bSelectAll);
        }
        // column 1
        item = q_tableSignalSiftBefore->view()->item(i, 1);
        if (item) {
            q_tableSignalSiftBefore->view()->setItemSelected(item, bSelectAll);
        }
    }
}

void DatabaseMgr::_emit_buttonSaveClicked()
{
    //
    if (q_listWidgetSignalTable->count() == 0) {
        return; // empty
    }

    // 获取选中项
    QMap<QString, bool> mapSignalSift;
    int rowCount = q_tableSignalSiftBefore->view()->rowCount();
    for (int i = 0; i < rowCount; ++i) {
        const QStandardItem *item = q_tableSignalSiftBefore->view()->item(i, 0);
        if (!item) {
            continue;
        }
        //
        mapSignalSift[item->text()] = q_tableSignalSiftBefore->view()->isItemSelected(item);
    }

    //
    if (mapSignalSift.isEmpty()) {
        return;
    }

    //
    QString tableName = JSqlTableMgr::instance()->signalTableNamePrefix()
            .append(q_listWidgetSignalTable->currentItem()->text());

    //
    if (!JSqlDataMgr::instance()->updateSignalSift(tableName, mapSignalSift)) {
        return;
    }

    //
    q_buttonSave->setEnabled(false);
}

void DatabaseMgr::_emit_tableSignalSiftSelectChanged()
{
    q_buttonSave->setEnabled(true);
}

void DatabaseMgr::updateToolBoxItemText(int index, const QString &prefix, const QString &value)
{
    q_toolBoxSignalTables->setItemText(index, prefix % (value.isEmpty()
                                                        ? "" : (QString("  <%1>").arg(value))));

    //
    updateTableSignalSift();
}

JFilterTableView *DatabaseMgr::getTableSignalSift() const
{
    return q_tableSignalSiftBefore;
}

void DatabaseMgr::setTableSignalSift(JFilterTableView *value)
{
    q_tableSignalSiftBefore = value;
}
