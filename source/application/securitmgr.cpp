#include "precomp.h"
#include "securitmgr.h"
#include "globalconfig.h"

SecuritMgr::SecuritMgr(QWidget *parent)
    : QDialog(parent, Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint)
{
    setWindowTitle(QStringLiteral("��ȫ����Ա��").append(GlobalConfig::instance()->userName())
                   .append(QStringLiteral("��")));

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

    QTableWidget *tableWidgetUser = new QTableWidget(widgetLeft);
    tableWidgetUser->horizontalHeader()->hide();
    tableWidgetUser->verticalHeader()->hide();
    tableWidgetUser->horizontalHeader()->setStretchLastSection(true);
    tableWidgetUser->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableWidgetUser->setColumnCount(1);
    vertLayoutLeft->addWidget(tableWidgetUser);

    QGroupBox *groupBoxSecurity = new QGroupBox(QStringLiteral("�û�Ȩ��"), this);
    splitter->addWidget(groupBoxSecurity);

    QFormLayout *formLayoutSecurity = new QFormLayout(groupBoxSecurity);
    formLayoutSecurity->setLabelAlignment(Qt::AlignRight);

    QCheckBox *checkBoxCanReadDatabase = new QCheckBox(QStringLiteral("�鿴���ݿ�"), groupBoxSecurity);
    QCheckBox *checkBoxCanWriteDatabase = new QCheckBox(QStringLiteral("�޸����ݿ�"), groupBoxSecurity);
    QCheckBox *checkBoxCanImportData = new QCheckBox(QStringLiteral("��������"), groupBoxSecurity);
    QCheckBox *checkBoxCanExportData = new QCheckBox(QStringLiteral("��������"), groupBoxSecurity);
    QCheckBox *checkBoxCanReadRuleFile = new QCheckBox(QStringLiteral("�鿴�����ļ�"), groupBoxSecurity);
    QCheckBox *checkBoxCanWriteRuleFile = new QCheckBox(QStringLiteral("�޸Ĺ����ļ�"), groupBoxSecurity);
    formLayoutSecurity->addRow(" ", checkBoxCanReadDatabase);
    formLayoutSecurity->addRow(" ", checkBoxCanWriteDatabase);
    formLayoutSecurity->addRow(" ", checkBoxCanImportData);
    formLayoutSecurity->addRow(" ", checkBoxCanExportData);
    formLayoutSecurity->addRow(" ", checkBoxCanReadRuleFile);
    formLayoutSecurity->addRow(" ", checkBoxCanWriteRuleFile);

    //
    connect(tableWidgetUser, &QTableWidget::currentItemChanged, this, [=](QTableWidgetItem *current){
        if (!current) {
            return;
        }
        unsigned int security = current->data(Qt::UserRole + 1).toUInt();
        // CanReadDatabase
        if (security & CanReadDatabase) {
            checkBoxCanReadDatabase->setChecked(true);
        } else {
            checkBoxCanReadDatabase->setChecked(false);
        }
        // CanWriteDatabase
        if (security & CanWriteDatabase) {
            checkBoxCanWriteDatabase->setChecked(true);
        } else {
            checkBoxCanWriteDatabase->setChecked(false);
        }
        // CanImportData
        if (security & CanImportData) {
            checkBoxCanImportData->setChecked(true);
        } else {
            checkBoxCanImportData->setChecked(false);
        }
        // CanExportData
        if (security & CanExportData) {
            checkBoxCanExportData->setChecked(true);
        } else {
            checkBoxCanExportData->setChecked(false);
        }
        // CanReadRuleFile
        if (security & CanReadRuleFile) {
            checkBoxCanReadRuleFile->setChecked(true);
        } else {
            checkBoxCanReadRuleFile->setChecked(false);
        }
        // CanWriteRuleFile
        if (security & CanWriteRuleFile) {
            checkBoxCanWriteRuleFile->setChecked(true);
        } else {
            checkBoxCanWriteRuleFile->setChecked(false);
        }
    });

    auto updateSecurity = [=]() -> bool {
        unsigned int security = 0;
        if (checkBoxCanReadDatabase->isChecked()) {
            security |= CanReadDatabase;
        }
        if (checkBoxCanWriteDatabase->isChecked()) {
            security |= CanWriteDatabase;
        }
        if (checkBoxCanImportData->isChecked()) {
            security |= CanImportData;
        }
        if (checkBoxCanExportData->isChecked()) {
            security |= CanExportData;
        }
        if (checkBoxCanReadRuleFile->isChecked()) {
            security |= CanReadRuleFile;
        }
        if (checkBoxCanWriteRuleFile->isChecked()) {
            security |= CanWriteRuleFile;
        }

        int row = tableWidgetUser->currentRow();
        if (row == -1) {
            return false;
        }
        QTableWidgetItem *item = tableWidgetUser->item(row, 0);
        if (!item) {
            return false;
        }

        const QString &userName = item->text();
        if (!JSqlTableMgr::instance()->setUserSecurity(userName, security)) {
            return false;
        }

        //
        item->setData(Qt::UserRole + 1, security);

        return true;
    };

    connect(checkBoxCanReadDatabase, &QCheckBox::toggled, this, [=](bool checked){
        if (!checked) {
            checkBoxCanWriteDatabase->setChecked(false);
        }
        updateSecurity();
    });
    connect(checkBoxCanWriteDatabase, &QCheckBox::toggled, this, [=](bool checked){
        if (checked) {
            checkBoxCanReadDatabase->setChecked(true);
        } else {
            checkBoxCanImportData->setChecked(false);
        }
        updateSecurity();
    });
    connect(checkBoxCanImportData, &QCheckBox::toggled, this, [=](){
        updateSecurity();
    });
    connect(checkBoxCanExportData, &QCheckBox::toggled, this, [=](){
        updateSecurity();
    });
    connect(checkBoxCanReadRuleFile, &QCheckBox::toggled, this, [=](bool checked){
        if (!checked) {
            checkBoxCanWriteRuleFile->setChecked(false);
        }
        updateSecurity();
    });
    connect(checkBoxCanWriteRuleFile, &QCheckBox::toggled, this, [=](bool checked){
        if (checked) {
            checkBoxCanReadRuleFile->setChecked(true);
        }
        updateSecurity();
    });

    //
    tableWidgetUser->clearContents();
    QList<JAccountInfo> accountInfos;
    if (JSqlTableMgr::instance()->readAllUser(accountInfos, AccountUserNormal)) {
        QListIterator<JAccountInfo> citer(accountInfos);
        while (citer.hasNext()) {
            const JAccountInfo &accountInfo = citer.next();
            int rowCount = tableWidgetUser->rowCount();
            tableWidgetUser->insertRow(rowCount);
            QTableWidgetItem *item = new QTableWidgetItem(accountInfo.userName);
            item->setData(Qt::UserRole + 1, accountInfo.security);
            tableWidgetUser->setItem(rowCount, 0, item);
        }
    }

    if (accountInfos.isEmpty()) {
        groupBoxSecurity->hide();
    } else {
        tableWidgetUser->setCurrentCell(0, 0);
    }
}

void SecuritMgr::done(int code)
{
    int result = QMessageBox::warning(this, QStringLiteral("����"),
                                      QStringLiteral("�Ƿ�Ҫ�ر������������˳�������"),
                                      QStringLiteral("�˳�"), QStringLiteral("ȡ��"));
    if (result == 0) {
        QDialog::done(code);
    }
}
