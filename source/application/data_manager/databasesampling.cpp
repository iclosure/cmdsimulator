#include "precomp.h"
#include "databasesampling.h"
#include "jflyrecord.h"
#include "../globalconfig.h"

// - class DatabaseSampling -

DatabaseSampling::DatabaseSampling(QWidget *parent) :
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
    if (GlobalConfig::instance()->canWriteDatabase()) {
        q_listWidgetSignalTable->setContextMenuPolicy(Qt::ActionsContextMenu);
    }
    QAction *actionRemoveTable = new QAction(QStringLiteral("删除此表"), q_listWidgetSignalTable);
    q_listWidgetSignalTable->addAction(actionRemoveTable);
    q_toolBoxSignalTables->addItem(q_listWidgetSignalTable, QStringLiteral("表名"));

    q_buttonReload = new QPushButton(QStringLiteral("重新加载数据库"), widgetLeftArea);
    q_buttonReload->setMinimumWidth(250);
    vertLayoutLeft->addWidget(q_buttonReload, 0, Qt::AlignRight);

    // right

    QWidget *widgetRightArea = new QWidget(this);
    splitter->addWidget(widgetRightArea);

    QVBoxLayout *vertLayoutRight = new QVBoxLayout(widgetRightArea);
    vertLayoutRight->setContentsMargins(2, 2, 2, 2);
    vertLayoutRight->setSpacing(6);

    QGroupBox *groupBoxSampling= new QGroupBox(QStringLiteral("采样"), widgetRightArea);
    vertLayoutRight->addWidget(groupBoxSampling);

    QVBoxLayout *vertLayoutSampling = new QVBoxLayout(groupBoxSampling);

    QFormLayout *formLayoutSampling= new QFormLayout();
    formLayoutSampling->setLabelAlignment(Qt::AlignRight);
    vertLayoutSampling->addLayout(formLayoutSampling);
#if 0
    q_timeEditInterval = new QTimeEdit(groupBoxSampling);
    q_timeEditInterval->setDisplayFormat("hh:mm:ss,zzz");
    formLayoutSampling->addRow(QStringLiteral("时间间隔："), q_timeEditInterval);
#else
    q_spinBoxInterval = new QSpinBox(groupBoxSampling);
    q_spinBoxInterval->setRange(1, 0x7fffffff);
    formLayoutSampling->addRow(QStringLiteral("采样间隔："), q_spinBoxInterval);
#endif
    q_buttonConvert = new QPushButton(QStringLiteral("压缩数据库"), groupBoxSampling);
    q_buttonConvert->setMinimumWidth(200);
    vertLayoutSampling->addSpacing(20);
    vertLayoutSampling->addWidget(q_buttonConvert, 0, Qt::AlignRight);

    if (!GlobalConfig::instance()->canWriteDatabase()) {
        q_buttonConvert->hide();
    }

    QGroupBox *groupBoxRemoveJC = new QGroupBox(QStringLiteral("删除数据"), widgetRightArea);
    groupBoxRemoveJC->hide();
    vertLayoutRight->addWidget(groupBoxRemoveJC);

    vertLayoutRight->addStretch();

    QVBoxLayout *vertLayoutRemoveJC = new QVBoxLayout(groupBoxRemoveJC);
    vertLayoutRemoveJC->setContentsMargins(0, 0, 0, 0);

    q_toolBoxJC = new QToolBox(widgetLeftArea);
    q_toolBoxJC->layout()->setSpacing(3);
    vertLayoutRemoveJC->addWidget(q_toolBoxJC);

    q_listWidgetJC = new QListWidget(q_toolBoxJC);
    q_listWidgetJC->setObjectName("listWidgetJC");
    q_listWidgetJC->setSelectionMode(QAbstractItemView::MultiSelection);
    q_toolBoxJC->addItem(q_listWidgetJC, QStringLiteral("架次"));

    //vertLayoutRemoveJC->addSpacing(20);

    QHBoxLayout *horiLayoutBottom = new QHBoxLayout();
    horiLayoutBottom->setContentsMargins(0, 0, 0, 0);
    vertLayoutRemoveJC->addLayout(horiLayoutBottom);

    horiLayoutBottom->addStretch();

    q_buttonRestore = new QPushButton(QStringLiteral("还原"), groupBoxRemoveJC);
    q_buttonRestore->setMinimumWidth(200);
    horiLayoutBottom->addWidget(q_buttonRestore);

    q_buttonRemoveJC = new QPushButton(QStringLiteral("删除数据"), groupBoxRemoveJC);
    q_buttonRemoveJC->setMinimumWidth(200);
    horiLayoutBottom->addWidget(q_buttonRemoveJC);

    q_progressBar = new QProgressBar(widgetRightArea);
    q_progressBar->setUpdatesEnabled(true);
    q_progressBar->setRange(0, 100);
    vertLayoutRight->addWidget(q_progressBar);

    //
    connect(q_listWidgetSignalTable, SIGNAL(currentRowChanged(int)),
            SLOT(onSignalTableChanged(int)));
    connect(actionRemoveTable, SIGNAL(triggered(bool)), SLOT(onActionRemoveTable()));
    connect(q_listWidgetJC, &QListWidget::itemSelectionChanged, [&](){
        q_buttonRemoveJC->setEnabled(true);
    });
    connect(q_spinBoxInterval, SIGNAL(valueChanged(int)), SLOT(onIntervalChanged(int)));
    connect(q_buttonReload, SIGNAL(clicked(bool)), SLOT(onButtonReloadClicked()));
    connect(q_buttonConvert, SIGNAL(clicked(bool)), SLOT(onButtonConvertClicked()));
    connect(q_buttonRestore, SIGNAL(clicked(bool)), SLOT(onButtonRestoreClicked()));
    connect(q_buttonRemoveJC, SIGNAL(clicked(bool)), SLOT(onButtonRemoveJCClicked()));

    //
    q_futureWatcherSampling = new QFutureWatcher<bool>(this);
    q_futureWatcherRemoveJC = new QFutureWatcher<bool>(this);
    q_futureWatcherRemoveTable = new QFutureWatcher<bool>(this);

    //
    connect(q_futureWatcherSampling, SIGNAL(finished()), SLOT(onSamplingFinished()));
    connect(q_futureWatcherRemoveJC, SIGNAL(finished()), SLOT(onRemoveJCFnished()));
    connect(q_futureWatcherRemoveTable, SIGNAL(finished()), SLOT(onRemoveTableFnished()));

    //
    q_listWidgetJC->setStyleSheet("QListWidget#listWidgetJC::item:selected{"
                                  "background:#2d3f55;"
                                  "color:#ffaa77;"
                                  "font-style: italic;"
                                  "}");

    //
    if (!GlobalConfig::instance()->canWriteDatabase()) {
        q_buttonReload->hide();
        q_buttonConvert->hide();
    }
}

bool DatabaseSampling::init()
{
    bool result = true;

    //
    q_progressBar->hide();
    q_buttonConvert->setEnabled(false);
    q_buttonRemoveJC->setEnabled(false);
    q_buttonRestore->hide();

    //
    //q_timeEditInterval->setTime(QTime(1, 0));   // 默认为一小时
    q_spinBoxInterval->setValue(1); //

    //
    updateSignalTable();

    //
    updateJCList();

    return result;
}

void DatabaseSampling::onSignalTableChanged(int)
{
    //
    updateTolBoxItemText();

    //
    q_buttonConvert->setEnabled(true);
}

void DatabaseSampling::onActionRemoveTable()
{
    QListWidgetItem *item = q_listWidgetSignalTable->currentItem();
    if (item == 0) {
        return;
    }

    //
    const QString tableName = item->text().section(" (", 0, 0);

    //
    int result = QMessageBox::warning(this, QStringLiteral("删除数据"),
                                      QStringLiteral("数据库将被修改！是否继续？"),
                                      QStringLiteral("继续"), QStringLiteral("取消"));
    if (result == 1) {
        return; // 取消修改
    }

    // 显示忙界面
    QMetaObject::invokeMethod(static_cast<QWidget *>(qApp->property("mainWindow").value<void *>()),
                              "setBusyIndicatorVisible", Q_ARG(bool, true));

    //
    QFuture<bool> future = QtConcurrent::run(taskRemoveTableData, tableName);
    q_futureWatcherRemoveTable->setFuture(future);
}

void DatabaseSampling::onIntervalChanged(int)
{
    q_buttonConvert->setEnabled(true);
}

void DatabaseSampling::onButtonReloadClicked()
{
    //
    updateSignalTable();

    //
    updateJCList();

    //
    q_buttonConvert->setEnabled(true);

    //
    q_buttonRemoveJC->setEnabled(true);
}

void DatabaseSampling::onButtonConvertClicked()
{
    //
    QListWidgetItem *currentItem = q_listWidgetSignalTable->currentItem();
    if (!currentItem) {
        return;     //
    }
    //
    int result = QMessageBox::warning(this, QStringLiteral("数据库数据压缩"),
                                      QStringLiteral("即将对数据库表[%1]进行采样压缩，是否继续？")
                                      .arg(currentItem->text().section(' ', 0, 0)),
                                      QStringLiteral("继续"), QStringLiteral("取消"));
    if (result == 1) {
        return; // 取消修改
    }
    //
    q_buttonConvert->setEnabled(false);

    // 显示忙界面
    QMetaObject::invokeMethod(static_cast<QWidget *>(qApp->property("mainWindow").value<void *>()),
                              "setBusyIndicatorVisible", Q_ARG(bool, true));

    //
    QFuture<bool> future = QtConcurrent::run(taskSamplingRecordData,
                                             currentItem->text().section(' ', 0, 0),
                                             q_spinBoxInterval->value());
    q_futureWatcherSampling->setFuture(future);
}

void DatabaseSampling::onButtonRestoreClicked()
{
    //
    updateJCList();
}

void DatabaseSampling::onButtonRemoveJCClicked()
{
    //
    QList<int> JCs;
    QList<QListWidgetItem *> items = q_listWidgetJC->selectedItems();
    QListIterator<QListWidgetItem *> citerItem(items);
    while (citerItem.hasNext()) {
        const QListWidgetItem *item = citerItem.next();
        if (item == 0) {
            continue;
        }
        //
        JCs.append(item->text().toInt());
    }
    //
    if (JCs.isEmpty()) {
        return; //
    }

    //
    int result = QMessageBox::warning(this, QStringLiteral("按架次删除数据"),
                                      QStringLiteral("数据库将被修改！是否继续？"),
                                      QStringLiteral("继续"), QStringLiteral("取消"));
    if (result == 1) {
        return; // 取消修改
    }
    //
    q_buttonRemoveJC->setEnabled(false);

    // 显示忙界面
    QMetaObject::invokeMethod(static_cast<QWidget *>(qApp->property("mainWindow").value<void *>()),
                              "setBusyIndicatorVisible", Q_ARG(bool, true));

    //
    QFuture<bool> future = QtConcurrent::run(taskRemoveJCData, JCs);
    q_futureWatcherRemoveJC->setFuture(future);
}

void DatabaseSampling::updateSignalTable()
{
    if (!GlobalConfig::instance()->canReadDatabase()) {
        return;
    }

    // 清空旧数据
    q_listWidgetSignalTable->clear();

    //
    QStringList signalTables = JSqlDataMgr::instance()->readAllSignalTableName();

    //
    QString signalTable, name, recordTableName;
    QStringListIterator citerSignalTables(signalTables);
    while (citerSignalTables.hasNext()) {
        signalTable = citerSignalTables.next();
        name = signalTable.section('_', 2);
        recordTableName = JSqlDataMgr::instance()->recordTableNamePrefix() + name;
        int count = JSqlTableMgr::instance()->rowCountOfTable(recordTableName);
        //
        q_listWidgetSignalTable->addItem(QStringLiteral("%1 (%2 行)")
                                         .arg(name).arg(count));
    }

    //
    q_listWidgetSignalTable->setCurrentRow(0);
}

void DatabaseSampling::updateJCList()
{
    if (!GlobalConfig::instance()->canReadDatabase()) {
        return;
    }

    //
    q_listWidgetJC->clear();
    QList<int> allJC = JSqlDataMgr::instance()->readAllJC();
    foreach (int JC, allJC) {
        q_listWidgetJC->addItem(QString::number(JC));
    }
}

void DatabaseSampling::updateTolBoxItemText()
{
    //
    q_toolBoxSignalTables->setItemText(0, "");

    //
    QListWidgetItem *item = q_listWidgetSignalTable->currentItem();
    if (!item) {
        return;
    }

    //
    const QString suffix = item->text();

    //
    q_toolBoxSignalTables->setItemText(0, QStringLiteral("信号表名")
                                       % (suffix.isEmpty() ? "" : ( " <" + suffix + ">")));
}

void DatabaseSampling::onSamplingFinished()
{
    // 隐藏忙界面
    QMetaObject::invokeMethod(static_cast<QWidget *>(qApp->property("mainWindow").value<void *>()),
                              "setBusyIndicatorVisible", Q_ARG(bool, false));

    //
    if (!q_futureWatcherSampling->result()) {
        QMessageBox::warning(this, QStringLiteral("采样结果提示"),
                             QStringLiteral("采样处理失败！失败原因请查看最新日志文件"));
        return;
    } else {
        //
        updateSignalTable();
        //
        QMessageBox::information(this, QStringLiteral("采样结果提示"),
                                 QStringLiteral("采样处理完成！"));
    }
}

void DatabaseSampling::onRemoveJCFnished()
{
    // 显示忙界面
    QMetaObject::invokeMethod(static_cast<QWidget *>(qApp->property("mainWindow").value<void *>()),
                              "setBusyIndicatorVisible", Q_ARG(bool, false));

    //
    if (!q_futureWatcherRemoveJC->result()) {
        QMessageBox::warning(this, QStringLiteral("按架次删除数据提示"),
                             QStringLiteral("数据删除失败！失败原因请查看最新日志文件"));
        return;
    } else {
        //
        updateSignalTable();
        //
        updateJCList();
        //
        QMessageBox::information(this, QStringLiteral("按架次删除数据提示"),
                                 QStringLiteral("数据删除完成！"));
    }
}

void DatabaseSampling::onRemoveTableFnished()
{
    // 显示忙界面
    QMetaObject::invokeMethod(static_cast<QWidget *>(qApp->property("mainWindow").value<void *>()),
                              "setBusyIndicatorVisible", Q_ARG(bool, false));

    //
    if (!q_futureWatcherRemoveTable->result()) {
        QMessageBox::warning(this, QStringLiteral("删除数据提示"),
                             QStringLiteral("数据删除失败！失败原因请查看最新日志文件"));
        return;
    } else {
        //
        updateSignalTable();
        //
        updateJCList();
        //
        QMessageBox::information(this, QStringLiteral("删除数据提示"),
                                 QStringLiteral("数据删除完成！"));
    }
}

bool DatabaseSampling::taskSamplingRecordData(const QString &suffix, int interval)
{
    if (!JFlyRecord::instance()->samplingRecordData(suffix, interval)) {
        return false;
    }

    //
    return true;
}

bool DatabaseSampling::taskRemoveJCData(const QList<int> &jcs)
{
    //
    if (!JSqlDataMgr::instance()->removeRecordByJC(jcs)) {
        return false;
    }

    return true;
}

bool DatabaseSampling::taskRemoveTableData(const QString &suffix)
{
    //
    if (!JSqlDataMgr::instance()->removeTables(suffix)) {
        return false;
    }

    return true;
}
