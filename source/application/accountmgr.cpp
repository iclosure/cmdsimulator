#include "precomp.h"
#include "accountmgr.h"
#include "jsqltablemgr.h"
#include "globalconfig.h"

AccountMgr::AccountMgr(QWidget *parent)
    : QDialog(parent, Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint)
{
    setWindowTitle(QStringLiteral("").append(GlobalConfig::instance()->userName())
                   .append(QStringLiteral("")));

    QVBoxLayout *vertLayoutMain = new QVBoxLayout(this);
    vertLayoutMain->setContentsMargins(0, 0, 0, 0);

    JSplitter *splitter = new JSplitter(this);
    splitter->setHandleWidth(5);
    splitter->setHandleColor(QColor(32, 32, 32, 50));
    splitter->setScales(QList<double>() << 2 << 1);
    vertLayoutMain->addWidget(splitter);

    QWidget *widgetLeft = new QWidget(this);
    splitter->addWidget(widgetLeft);

    QVBoxLayout *vertLayoutLeft = new QVBoxLayout(widgetLeft);
    JTableView *tableView = new JTableView(this);
    vertLayoutLeft->addWidget(tableView);

    QHBoxLayout *horiLayoutLeftBottom = new QHBoxLayout();
    vertLayoutLeft->addLayout(horiLayoutLeftBottom);

    horiLayoutLeftBottom->addStretch();

    QPushButton *buttonAdd = new QPushButton(QStringLiteral("增加"), widgetLeft);
    QPushButton *buttonRemove = new QPushButton(QStringLiteral("删除"), widgetLeft);
    buttonAdd->setMinimumWidth(80);
    buttonRemove->setMinimumWidth(80);
    horiLayoutLeftBottom->addWidget(buttonAdd);
    horiLayoutLeftBottom->addWidget(buttonRemove);

    QWidget *widgetRight = new QWidget(this);
    splitter->addWidget(widgetRight);

    QVBoxLayout *vertLayoutRight = new QVBoxLayout(widgetRight);

    QGroupBox *groupBoxRight = new QGroupBox();
    vertLayoutRight->addWidget(groupBoxRight);

    QFormLayout *formLayoutRight = new QFormLayout(groupBoxRight);
    formLayoutRight->setLabelAlignment(Qt::AlignRight);

    QLineEdit *lineEditUserName = new QLineEdit(widgetRight);
    lineEditUserName->setMaxLength(80);
    formLayoutRight->addRow(QStringLiteral("用户名："), lineEditUserName);

    QLineEdit *lineEditPassword = new QLineEdit(widgetRight);
    lineEditPassword->setMaxLength(60);
    lineEditPassword->setEchoMode(QLineEdit::Password);
    lineEditPassword->setValidator(new QRegExpValidator
                                   (QRegExp("^[0-9A-Za-z]{0,60}$"), lineEditPassword));
    formLayoutRight->addRow(QStringLiteral("密码："), lineEditPassword);

    QComboBox *comboBoxUserType = new QComboBox(widgetRight);
    formLayoutRight->addRow(QStringLiteral("用户类型："), comboBoxUserType);

    QHBoxLayout *horiLayoutRightBottom = new QHBoxLayout();
    vertLayoutRight->addLayout(horiLayoutRightBottom);

    horiLayoutRightBottom->addStretch();

    QPushButton *buttonApply = new QPushButton(QStringLiteral("应用"), widgetRight);
    buttonApply->setMinimumWidth(80);
    horiLayoutRightBottom->addWidget(buttonApply);

    //
    connect(tableView, &JTableView::currentItemChanged, this,
            [=](QStandardItem *, QStandardItem *previous){
        //
        int currentRow = tableView->currentRow();
        if (previous && previous->row() == currentRow) {
            return;
        }
        //
        lineEditUserName->setProperty("XH", tableView->itemData(currentRow, 0, Qt::UserRole + 2).toString());
        lineEditUserName->setText(tableView->itemData(currentRow, 0).toString());
        lineEditPassword->setText(tableView->itemData(currentRow, 0, Qt::UserRole + 1).toString());
        const QString userType = tableView->itemData(currentRow, 1).toString();
        //
        if (userType == stringFromType(AccountUserAdmin)) {
            comboBoxUserType->setDisabled(true);
            buttonRemove->setDisabled(true);
            formLayoutRight->labelForField(comboBoxUserType)->hide();
            comboBoxUserType->hide();
            comboBoxUserType->setProperty("userType", (int)AccountUserAdmin);
        } else {
            comboBoxUserType->setEnabled(true);
            buttonRemove->setEnabled(true);
            formLayoutRight->labelForField(comboBoxUserType)->show();
            comboBoxUserType->show();
            comboBoxUserType->setCurrentText(userType);
        }
        //
        buttonApply->setEnabled(false);
    });
    connect(buttonAdd, &QPushButton::clicked, this, [=](){
        int row = tableView->rowCount();
        tableView->insertRow(row);
        tableView->setItemData(row, 0, "", Qt::UserRole + 2);   // XH
        tableView->setItemData(row, 0, "");
        tableView->setItemData(row, 0, "", Qt::UserRole + 1);   // password
        tableView->setItemData(row, 1, stringFromType(AccountUserNormal));
        tableView->setCurrentCell(row, 0);
        lineEditUserName->setFocus();
    });
    connect(buttonRemove, &QPushButton::clicked, this, [=](){
        int currentRow = tableView->currentRow();
        if (currentRow == -1) {
            return;
        }
        const QString userName = tableView->itemData(currentRow, 0).toString();
        if (!JSqlTableMgr::instance()->removeUser(userName)) {
            QString msg = QStringLiteral("删除用户[%1]失败！");
            QMessageBox::warning(this, QStringLiteral("警告"), msg.arg(userName));
            return;
        }
        tableView->removeRow(currentRow);
    });
    connect(buttonApply, &QPushButton::clicked, this, [=](){
        // check
        const QString userName = lineEditUserName->text().trimmed();
        if (userName.isEmpty()) {
            QMessageBox::warning(this, QStringLiteral("警告"),
                                 QStringLiteral("用户名不能为空！"));
            lineEditUserName->setFocus();
            return;
        }
        //
        const QString password = lineEditPassword->text().trimmed();
        if (password.isEmpty()) {
            QMessageBox::warning(this, QStringLiteral("警告"),
                                 QStringLiteral("密码不能为空！"));
            lineEditPassword->setFocus();
            return;
        }
        if (password.count() < 4) {
            QMessageBox::warning(this, QStringLiteral("警告"),
                                 QStringLiteral("密码不能少于4个字符！"));
            lineEditPassword->setFocus();
            return;
        }
        //
        JAccountInfo info;
        info.XH = lineEditUserName->property("XH").toString();
        info.userName = userName;
        info.password = password;
        if (comboBoxUserType->isHidden()) {
            info.userType = AccountUserAdmin;

        } else {
            info.userType = (AccountUserType)comboBoxUserType->currentData().toInt();
        }
        //
        if (!JSqlTableMgr::instance()->addUser(info)) {
            QMessageBox::critical(this, QStringLiteral("错误"),
                                 QStringLiteral("添加（修改）用户失败！"));
            return;
        }
        //
        int currentRow = tableView->currentRow();
        if (currentRow == -1) {
            return;
        };
        tableView->setItemData(currentRow, 0, info.XH, Qt::UserRole + 2);   // XH
        tableView->setItemData(currentRow, 0, userName);
        tableView->setItemData(currentRow, 0, password, Qt::UserRole + 1);  // password
        if (!comboBoxUserType->isHidden()) {
            tableView->setItemData(currentRow, 1, comboBoxUserType->currentText());
        }
        //
        buttonApply->setEnabled(false);
    });
    connect(lineEditUserName, &QLineEdit::textChanged, this, [=](){
        buttonApply->setEnabled(true);
    });
    connect(lineEditPassword, &QLineEdit::textChanged, this, [=](){
        buttonApply->setEnabled(true);
    });
    connect(comboBoxUserType, static_cast<void(QComboBox::*)(int)>
            (&QComboBox::currentIndexChanged), this, [=](){
        buttonApply->setEnabled(true);
    });

    //
    comboBoxUserType->addItem(stringFromType(AccountUserSecurity), AccountUserSecurity);
    comboBoxUserType->addItem(stringFromType(AccountUserAudit), AccountUserAudit);
    comboBoxUserType->addItem(stringFromType(AccountUserNormal), AccountUserNormal);

    //
    tableView->setColumnCount(2);
    tableView->setColumnWidth(0, 220);
    tableView->setHorizontalHeaderLabels(
                QStringList() << QStringLiteral("用户名") << QStringLiteral("用户类型（权限）"));

    //
    int currentRow = 0;
    QList<JAccountInfo> accounts;
    if (JSqlTableMgr::instance()->readAllUser(accounts)) {
        QListIterator<JAccountInfo> citer(accounts);
        while (citer.hasNext()) {
            const JAccountInfo &account = citer.next();
            tableView->insertRow(currentRow);
            tableView->setItemData(currentRow, 0, account.XH, Qt::UserRole + 2);   // XH
            tableView->setItemData(currentRow, 0, account.userName);
            tableView->setItemData(currentRow, 0, account.password, Qt::UserRole + 1);  // password
            tableView->setItemData(currentRow, 1, stringFromType(account.userType));
            ++currentRow;
        }
    }
}

void AccountMgr::done(int code)
{
    int result = QMessageBox::warning(this, QStringLiteral("警告"),
                                      QStringLiteral("是否要关闭软件？点击【退出】即可"),
                                      QStringLiteral("退出"), QStringLiteral("取消"));
    if (result == 0) {
        QDialog::done(code);
    }
}

QString AccountMgr::stringFromType(AccountUserType type)
{
    switch (type) {
    default:
    case AccountUserInvalid: return QStringLiteral("<未知用户>");
    case AccountUserAdmin: return QStringLiteral("系统管理员");
    case AccountUserSecurity: return QStringLiteral("安全保密员");
    case AccountUserAudit: return QStringLiteral("安全审计员");
    case AccountUserNormal: return QStringLiteral("软件用户");
    }
}
