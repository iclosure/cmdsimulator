#include "precomp.h"
#include "logviewmgr.h"
#include "globalconfig.h"

LogViewMgr::LogViewMgr(QWidget *parent)
    : QDialog(parent, Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint)
{
    setWindowTitle(QStringLiteral("安全审计员（").append(GlobalConfig::instance()->userName())
                   .append(QStringLiteral("）")));

    QVBoxLayout *vertLayoutMain = new QVBoxLayout(this);
    vertLayoutMain->setContentsMargins(0, 0, 0, 0);

    JSplitter *splitter = new JSplitter(this);
    splitter->setHandleWidth(5);
    splitter->setHandleColor(QColor(32, 32, 32, 50));
    splitter->setScales(QList<double>() << 1 << 3);
    vertLayoutMain->addWidget(splitter);

    QWidget *widgetLeft = new QWidget(this);
    splitter->addWidget(widgetLeft);

    QVBoxLayout *vertLayoutLeft = new QVBoxLayout(widgetLeft);
    vertLayoutLeft->setContentsMargins(2, 2, 2, 2);

    d_listWidgetLog = new QListWidget(this);
    vertLayoutLeft->addWidget(d_listWidgetLog);

    QPushButton *buttonClear = new QPushButton(QStringLiteral("清空"), widgetLeft);
    buttonClear->setMinimumWidth(80);
    vertLayoutLeft->addWidget(buttonClear, 0, Qt::AlignRight);

    QTextEdit *textEdit = new QTextEdit(this);
    textEdit->setReadOnly(true);
    splitter->addWidget(textEdit);

    //
    connect(d_listWidgetLog, &QListWidget::currentRowChanged, this, [=](int row){
        textEdit->clear();
        QListWidgetItem *item = d_listWidgetLog->item(row);
        if (!item) {
            return;
        }
        const QString &filePath = item->data(Qt::UserRole + 1).toString();
        QFile file(filePath);
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            return;
        }
        while (!file.atEnd()) {
            textEdit->append(QString::fromLocal8Bit(file.readLine().trimmed()));
        }
        file.close();
    });
    connect(d_listWidgetLog, &QListWidget::itemPressed, this, [=](QListWidgetItem *item){
        onItemPressed(item);
    });
    connect(buttonClear, &QPushButton::clicked, this, [=](){
        //
        int ret = QMessageBox::warning(this, QStringLiteral("警告"),
                                       QStringLiteral("删除后将不可恢复，是否继续？"),
                                       QMessageBox::Yes, QMessageBox::No);
        if (ret == QMessageBox::No) {
            return;
        }
        //
        QDir dir(QApplication::applicationDirPath().append("/log/"));
        dir.setFilter(QDir::Files);
        int count = dir.count();
        for (int i = 0; i < count; ++i) {
            dir.remove(dir[i]);
        }
        d_listWidgetLog->clear();
    });

    //
    d_listWidgetLog->clear();
    QFileInfoList fileInfos = QDir(QApplication::applicationDirPath().append("/log/"))
            .entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::Readable,
                           QDir::Name);
    QListIterator<QFileInfo> citer(fileInfos);
    while (citer.hasNext()) {
        const QFileInfo &fileInfo = citer.next();
        QListWidgetItem *item = new QListWidgetItem(fileInfo.fileName());
        item->setData(Qt::UserRole + 1, fileInfo.filePath());
        d_listWidgetLog->addItem(item);
    }

    d_listWidgetLog->setCurrentRow(0);
}

void LogViewMgr::done(int code)
{
    int result = QMessageBox::warning(this, QStringLiteral("警告"),
                                      QStringLiteral("是否要关闭软件？点击【退出】即可"),
                                      QStringLiteral("退出"), QStringLiteral("取消"));
    if (result == 0) {
        QDialog::done(code);
    }
}

void LogViewMgr::onItemPressed(QListWidgetItem *item)
{
    if (!item) {
        return;
    }
    if (QApplication::mouseButtons() != Qt::RightButton) {
        return;
    }
    QMenu menu(d_listWidgetLog);
    menu.addAction(QStringLiteral("删除"), this, SLOT(onRemoveItem()));
    menu.exec(QCursor::pos());
}

void LogViewMgr::onRemoveItem()
{
    QListWidgetItem *item = d_listWidgetLog->currentItem();
    if (!item) {
        return;
    }
    int row = d_listWidgetLog->currentRow();

    int ret = QMessageBox::warning(this, QStringLiteral("警告"),
                                   QStringLiteral("删除后将不可恢复，是否继续？"),
                                   QMessageBox::Yes, QMessageBox::No);
    if (ret == QMessageBox::No) {
        return;
    }
    const QString &filePath = item->data(Qt::UserRole + 1).toString();
    QFile::remove(filePath);
    d_listWidgetLog->takeItem(row);
}
