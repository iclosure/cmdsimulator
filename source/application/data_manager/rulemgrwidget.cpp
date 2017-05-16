#include "precomp.h"
#include "rulemgrwidget.h"
#include "jsplitter.h"
#include "jfiltertableview.h"
#include "jruledict.h"
#include "../globalconfig.h"

// - class NameEditWidget -

NameEditWidget::NameEditWidget(QWidget *parent) :
    QDialog(parent)
{
    QVBoxLayout *vertLayoutMain = new QVBoxLayout(this);

    QFormLayout *formLayout = new QFormLayout();
    vertLayoutMain->addLayout(formLayout);

    q_labelName = new QLabel(this);
    q_editName = new QLineEdit(this);
    q_editName->setMinimumWidth(300);
    formLayout->addRow(q_labelName, q_editName);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok |
                                                     QDialogButtonBox::Cancel, this);
    vertLayoutMain->addWidget(buttons, 0, Qt::AlignRight);

    connect(q_editName, &QLineEdit::textChanged, [=](const QString &text){
        buttons->button(QDialogButtonBox::Ok)->setDisabled(text.isEmpty());
    });
    connect(buttons, &QDialogButtonBox::accepted, [=](){
        if (q_editName->text().isEmpty()) {
            q_editName->setFocus();
            return;
        }
        this->accept();
    });
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    //adjustSize();
    //resize(sizeHint().width() * 3 / 2, sizeHint().height());

    buttons->button(QDialogButtonBox::Ok)->setEnabled(false);
}

void NameEditWidget::setEditLabelText(const QString &text)
{
    q_labelName->setText(text);
}

QString NameEditWidget::editText() const
{
    return q_editName->text();
}

// - class RuleMgrWidget -

RuleMgrWidget::RuleMgrWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *vertLayoutMain = new QVBoxLayout(this);
    vertLayoutMain->setContentsMargins(2, 2, 2, 2);

    // top

    QGroupBox *groupRulePath = new QGroupBox(this);
    vertLayoutMain->addWidget(groupRulePath);

    QHBoxLayout *horiLayoutRulePath = new QHBoxLayout(groupRulePath);

    QFormLayout *formLayoutRuleFilePath = new QFormLayout();
    formLayoutRuleFilePath->setFormAlignment(Qt::AlignCenter);
    horiLayoutRulePath->addLayout(formLayoutRuleFilePath);

    q_editRulePath = new QLineEdit(this);
    q_editRulePath->setReadOnly(true);
    formLayoutRuleFilePath->addRow(QStringLiteral("规则文件："), q_editRulePath);

    q_buttonView = new QPushButton(QStringLiteral(" 打开"), this);
    q_buttonView->setMinimumWidth(120);
    horiLayoutRulePath->addWidget(q_buttonView);

    // adjustSize
    groupRulePath->adjustSize();
    groupRulePath->setFixedHeight(groupRulePath->sizeHint().height());

    // bottom

    QGroupBox *groupBoxRule = new QGroupBox(QStringLiteral("规则编辑"), this);
    vertLayoutMain->addWidget(groupBoxRule);

    QHBoxLayout *horiLayoutRule = new QHBoxLayout(groupBoxRule);
    horiLayoutRule->setContentsMargins(0, horiLayoutRule->contentsMargins().top(), 0, 0);

    JSplitter *splitter = new JSplitter(this);
    splitter->setHandleWidth(6);
    splitter->setHandleColor(QColor(100, 100, 100, 30));
    splitter->setScales(QList<double>() << 1 << 3);
    horiLayoutRule->addWidget(splitter);

    // bottom - left

    q_treeRule = new QTreeWidget(groupBoxRule);
    splitter->addWidget(q_treeRule);

    // bottom - right

    q_tableRule = new JFilterTableView(groupBoxRule);
    q_tableRule->view()->setSelectionMode(QTableView::ExtendedSelection);
    if (!GlobalConfig::instance()->canWriteRuleFile()) {
        q_tableRule->view()->setEditTriggers(QTableView::NoEditTriggers);
    }
    splitter->addWidget(q_tableRule);

    // table-rule-context-menu
    q_tableRule->view()->addAction(new QAction(q_tableRule->view()));

    //
    connect(q_buttonView, &QPushButton::clicked, [=](){
        QString filePath = QFileDialog::getOpenFileName(this, QStringLiteral("打开规则文件"),
                                                        QApplication::applicationDirPath().append("/../config"),
                                                        "XML File (*.xml)",
                                                        0);
        if (filePath.isEmpty()) {
            return;
        }
        q_editRulePath->setText(filePath);

        // 重新加载规则字典模块
        JRuleDict::instance()->loadRuleFile(filePath);

        // update rule of tree
        updateTreeRule();

        // update rule of table
        updateTableRule();
    });
    connect(q_treeRule, &QTreeWidget::itemPressed,
            this, &RuleMgrWidget::_emit_itemPressed);
    connect(q_treeRule, &QTreeWidget::itemSelectionChanged,
            this, &RuleMgrWidget::_emit_itemSelectionChanged);
    connect(q_tableRule->view(), SIGNAL(itemSelectionChanged()),
            SLOT(_emit_tableRuleItemSelectedChanged()));
    connect(q_tableRule->view()->actions().at(0), SIGNAL(triggered(bool)),
            SLOT(_emit_removeTableRuleItems()));

    //
    if (!GlobalConfig::instance()->canReadRuleFile()) {
        groupRulePath->hide();
    }
}

bool RuleMgrWidget::init()
{
    // rulePath
    q_editRulePath->setText(QFileInfo(JRuleDict::instance()->ruleFilePath()).absoluteFilePath());
    //q_buttonView->setVisible(false);

    // treeWidget
    q_treeRule->setHeaderLabel(QStringLiteral("规则树信息"));

    // tableView
    q_tableRule->view()->setConfig(QApplication::applicationDirPath()
                                   .append("/../config/xmltable.xml"));

    // update rule of tree
    updateTreeRule();

    // update rule of table
    updateTableRule();

    return true;
}

void RuleMgrWidget::updateTreeRule()
{
    if (!GlobalConfig::instance()->canReadRuleFile()) {
        return;
    }

    // 清空树
    q_treeRule->clear();

    // 获取隐藏的根节点
    QTreeWidgetItem *invRootItem = q_treeRule->invisibleRootItem();
    if (!invRootItem) {
        return;     // 不存在
    }

    // 添加可见根节点
    QTreeWidgetItem *rootItem = new QTreeWidgetItem(QStringList() << QStringLiteral("规则表"));
    rootItem->setData(0, Qt::UserRole, TreeItemRoot);
    invRootItem->addChild(rootItem);

    // 更新树
    QString text;
    QMapIterator<QString, QMap<QString, QStringList> > citerMapRule(JRuleDict::instance()->data());
    while (citerMapRule.hasNext()) {
        citerMapRule.next();
        const QString &JX = citerMapRule.key();
        if (JX.isEmpty()) {     // global rule
            text = QStringLiteral("<缺省规则>");
        } else {
            text = QString(QStringLiteral("机型：%1")).arg(JX);
        }
        QTreeWidgetItem *itemJX = new QTreeWidgetItem(QStringList() << text);
        //itemJX->setFlags(itemJX->flags() | Qt::ItemIsEditable);
        itemJX->setData(0, Qt::UserRole, TreeItemJX);
        itemJX->setData(0, Qt::UserRole + 1, JX);
        rootItem->addChild(itemJX);
        QStringListIterator citerKeys(citerMapRule.value().keys());
        while (citerKeys.hasNext()) {
            const QString &name = citerKeys.next();
            QTreeWidgetItem *itemName = new QTreeWidgetItem(QStringList() << name);
            //itemName->setFlags(itemJX->flags() | Qt::ItemIsEditable);
            itemName->setData(0, Qt::UserRole, TreeItemName);
            itemName->setData(0, Qt::UserRole + 1, name);
            itemJX->addChild(itemName);
        }
    }

    // 展开所有节点
    q_treeRule->expandAll();
}

void RuleMgrWidget::updateTableRule()
{
    if (!GlobalConfig::instance()->canReadRuleFile()) {
        return;
    }

    // 清空规则编辑表格数据
    q_tableRule->setTitle("<>");
    q_tableRule->view()->clearContents();

    // 获取当前选中节点
    QTreeWidgetItem *currentItem = q_treeRule->currentItem();
    if (!currentItem) {
        return;     // invalid selected item
    }

    // 获取节点类型
    QVariant type = currentItem->data(0, Qt::UserRole);
    if (type.isNull()) {
        return;     // 节点类型无效
    }

    // 禁止表格内容改变通知
    enableItemChangedNotify(false);

    // 根据节点类型进行相应处理
    switch (type.toInt()) {
    case TreeItemRoot:
    {
        // 设置表配置信息
        q_tableRule->view()->setTableName(QStringLiteral("规则编辑表——机型级"));

        // 设置表单右键菜单项文本
        q_tableRule->view()->actions().at(0)->setText(QStringLiteral("删除选中机型"));

        // 设置表单标题
        q_tableRule->setTitle(QString(QStringLiteral("机型编辑")));

        // 获取所有机型（除缺省机型）
        QStringList JXs = JRuleDict::instance()->allJX();

        // 检测参数有效性
        if (JXs.isEmpty()) {
            break;     // 无数据可更新
        }

        // 更新规则编辑表
        int rowIndex = 0;
        QStringListIterator citerJX(JXs);
        while (citerJX.hasNext()) {
            const QString &JX = citerJX.next();
            if (JX.isEmpty()) {
                continue;       // 排除缺省规则项
            }
            q_tableRule->view()->insertRow(rowIndex);
            q_tableRule->view()->setItemValue(rowIndex, 0, JX);
            q_tableRule->view()->setItemData(rowIndex, 0, JX, Qt::UserRole);
            ++rowIndex;
        }
        break;
    }
    case TreeItemJX:
    {
        // 设置表配置信息
        q_tableRule->view()->setTableName(QStringLiteral("规则编辑表——别名级"));

        // 设置表单右键菜单项文本
        q_tableRule->view()->actions().at(0)->setText(QStringLiteral("删除选中别名"));

        // 获取机型
        QString JX = currentItem->data(0, Qt::UserRole + 1).toString();

        // 获取JX的所有别名
        QStringList names = JRuleDict::instance()->namesOfJX(JX);

        //
        if (JX.isEmpty()) {
            JX = QStringLiteral("<缺省规则>");
        }

        // 设置表单标题
        q_tableRule->setTitle(QStringLiteral("%1 —— 别名编辑").arg(JX));

        // 检测参数有效性
        if (names.isEmpty()) {
            break;     // 无数据可更新
        }

        // 更新规则编辑表
        int rowIndex = 0;
        QStringListIterator citerName(names);
        while (citerName.hasNext()) {
            const QString &name = citerName.next();
            q_tableRule->view()->insertRow(rowIndex);
            q_tableRule->view()->setItemValue(rowIndex, 0, name);
            q_tableRule->view()->setItemData(rowIndex, 0, name, Qt::UserRole);
            ++rowIndex;
        }
        break;
    }
    case TreeItemName:
    {
        // 获取机型节点
        QTreeWidgetItem *itemJX = currentItem->parent();
        if (!itemJX || itemJX->data(0, Qt::UserRole).toInt() != TreeItemJX) {
            break;     // 节点无效或为非机型节点
        }

        // 设置表配置信息
        q_tableRule->view()->setTableName(QStringLiteral("规则编辑表——变量级"));

        // 设置表单右键菜单项文本
        q_tableRule->view()->actions().at(0)->setText(QStringLiteral("删除选中变量"));

        // 获取机型
        QString JX = itemJX->data(0, Qt::UserRole + 1).toString();

        // 获取别名
        QString name = currentItem->data(0, Qt::UserRole + 1).toString();

        // 查询别名对应的所有变量
        QStringList symbols;
        if (itemJX->text(0) == QStringLiteral("<缺省规则>")) {
            JX = itemJX->text(0);
            symbols = JRuleDict::instance()->symbolsOfName(name);
        } else {
            symbols = JRuleDict::instance()->symbolsOfName(JX, name);
        }

        // 设置表单标题
        q_tableRule->setTitle(QStringLiteral("%1 —— %2").arg(JX).arg(name));

        // 检测参数有效性
        if (symbols.isEmpty()) {
            break;     // 无数据可更新
        }

        // 更新规则编辑表
        int rowIndex = 0;
        QStringListIterator citerSymbols(symbols);
        while (citerSymbols.hasNext()) {
            const QString &symbol = citerSymbols.next();
            q_tableRule->view()->insertRow(rowIndex);
            q_tableRule->view()->setItemValue(rowIndex, 0, name);
            q_tableRule->view()->setItemValue(rowIndex, 1, symbol);
            q_tableRule->view()->setItemData(rowIndex, 1, symbol, Qt::UserRole);
            ++rowIndex;
        }
        break;
    }
    default:
        break;     // 无效节点
    }

    // 允许表格内容改变通知
    enableItemChangedNotify(true);
}

void RuleMgrWidget::_emit_itemPressed(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);

    // 检测参数有效性
    if (!item) {
        return;
    }

    // 判断鼠标点击按键
    Qt::MouseButtons buttons = QApplication::mouseButtons();
    if (buttons != Qt::RightButton) {
        return;     // 只响应鼠标右键
    }

    if (!GlobalConfig::instance()->canWriteRuleFile()) {
        return;
    }

    // 获取节点类型
    QVariant type = item->data(0, Qt::UserRole);
    if (type.isNull()) {
        return;     // 节点类型无效
    }

    // 创建菜单
    QMenu menu(this);
    int itemType = type.toInt();
    switch (itemType) {
    case TreeItemRoot:
    {
        menu.addAction(QIcon(""), QStringLiteral("增加机型"), this, SLOT(_emit_addJX()));
        break;
    }
    case TreeItemJX:
    {
        if (item->text(0) != QStringLiteral("<缺省规则>")) {
            menu.addAction(QIcon(""), QStringLiteral("删除机型"), this, SLOT(_emit_removeJX()));
        }
        menu.addAction(QIcon(""), QStringLiteral("增加别名"), this, SLOT(_emit_addName()));
        break;
    }
    case TreeItemName:
    {
        menu.addAction(QIcon(""), QStringLiteral("删除别名"), this, SLOT(_emit_removeName()));
        menu.addAction(QIcon(""), QStringLiteral("增加变量"), this, SLOT(_emit_addSymbol()));
        break;
    }
    default:
        break;
    }

    if (menu.isEmpty()) {
        return;     // 没有菜单项
    }

    menu.exec(QCursor::pos());
}

void RuleMgrWidget::_emit_itemSelectionChanged()
{
    updateTableRule();
}

void RuleMgrWidget::_emit_tableRuleItemChanged(QStandardItem *item)
{
    // 获取规则树当前选中节点
    QTreeWidgetItem *currentTreeItem = q_treeRule->currentItem();
    if (!currentTreeItem) {
        return;     // 选中节点无效
    }

    // 获取节点类型
    QVariant type = currentTreeItem->data(0, Qt::UserRole);
    if (type.isNull()) {
        return;     // 节点类型无效
    }

    // 禁止表格内容改变通知
    enableItemChangedNotify(false);

    // 获取表格当前
    int itemType = type.toInt();
    switch (itemType) {
    case TreeItemRoot:
    {
        // 获取新的机型
        QString newJX = item->text().trimmed();
        if (newJX.isEmpty()) {
            QMessageBox::warning(this, QStringLiteral("机型修改"),
                                 QStringLiteral("机型名称不能为空，请重填！\n(第%1行第%2列)")
                                 .arg(item->row() + 1).arg(item->column() + 1));
            q_tableRule->view()->setItemSelected(item, true);
            break;      // 机型不能为空
        }

        // 获取原有机型
        QString oldJX = item->data(Qt::UserRole).toString();

        // 检测是否输入了空格在文本两侧
        if (newJX == oldJX) {
            item->setText(newJX);
            break;
        }

        // 修改机型
        if (!JRuleDict::instance()->replaceJX(oldJX, newJX)) {
            break;      // 修改失败
        }

        // 更新原有机型
        item->setData(newJX, Qt::UserRole);

        // 更新规则树对应的机型节点名称
        int childCount = currentTreeItem->childCount();
        for (int i = 0; i < childCount; ++i) {
            QTreeWidgetItem *treeItem = currentTreeItem->child(i);
            if (!treeItem) {
                continue;
            }

            // 获取机型名称
            QString itemJX = treeItem->data(0, Qt::UserRole + 1).toString();
            if (itemJX == oldJX) {
                // 修改机型名称
                treeItem->setText(0, QStringLiteral("机型：") % newJX);
                treeItem->setData(0, Qt::UserRole + 1, newJX);
                break;
            }
        }
        break;
    }
    case TreeItemJX:
    {
        // 获取新的别名
        QString newName = item->text().trimmed();
        if (newName.isEmpty()) {
            QMessageBox::warning(this, QStringLiteral("别名修改"),
                                 QStringLiteral("别名不能为空，请重填！\n(第%1行第%2列)")
                                 .arg(item->row() + 1).arg(item->column() + 1));
            q_tableRule->view()->setItemSelected(item, true);
            break;      // 别名不能为空
        }

        // 获取原有别名
        QString oldName = item->data(Qt::UserRole).toString();

        // 检测是否还是输入了空格在文本两侧
        if (newName == oldName) {
            item->setText(newName);
            break;
        }

        // 获取当前机型
        QString JX = currentTreeItem->data(0, Qt::UserRole + 1).toString();
        if (JX.isEmpty()) {
            // 修改缺省规则别名
            if (!JRuleDict::instance()->replaceName(oldName, newName)) {
                break;      // 修改失败
            }
        } else {
            // 修改指定机型别名
            if (!JRuleDict::instance()->replaceName(JX, oldName, newName)) {
                break;      // 修改失败
            }
        }

        // 更新原有别名
        item->setData(newName, Qt::UserRole);

        // 更新规则树对应的别名节点名称
        int childCount = currentTreeItem->childCount();
        for (int i = 0; i < childCount; ++i) {
            QTreeWidgetItem *treeItem = currentTreeItem->child(i);
            if (!treeItem) {
                continue;
            }

            // 获取别名
            QString itemName = treeItem->data(0, Qt::UserRole + 1).toString();
            if (itemName == oldName) {
                // 修改别名
                treeItem->setText(0, newName);
                treeItem->setData(0, Qt::UserRole + 1, newName);
                break;
            }
        }
        break;
    }
    case TreeItemName:
    {
        // 获取新的变量名
        QString newSymbol = item->text().trimmed();
        if (newSymbol.isEmpty()) {
            QMessageBox::warning(this, QStringLiteral("变量名修改"),
                                 QStringLiteral("变量名不能为空，请重填！\n(第%1行第%2列)")
                                 .arg(item->row() + 1).arg(item->column() + 1));
            q_tableRule->view()->setItemSelected(item, true);
            break;      // 变量名不能为空
        }

        // 获取机型节点
        QTreeWidgetItem *itemJX = currentTreeItem->parent();
        if (!itemJX || itemJX->data(0, Qt::UserRole).toInt() != TreeItemJX) {
            break;     // 节点无效或为非机型节点
        }

        // 获取机型
        QString JX = itemJX->data(0, Qt::UserRole + 1).toString();

        // 获取别名
        QString name = currentTreeItem->data(0, Qt::UserRole + 1).toString();

        // 获取原有变量名
        QString oldSymbol = item->data(Qt::UserRole).toString();

        // 检测是否输入了空格在文本两侧
        if (newSymbol == oldSymbol) {
            item->setText(newSymbol);
            break;
        }

        // 修改变量名
        if (JX.isEmpty()) {
            // 修改缺省规则，指定别名下的变量名
            if (!JRuleDict::instance()->replaceSymbol(name, oldSymbol, newSymbol)) {
                break;      // 修改失败
            }
        } else {
            // 修改指定机型、别名下的变量名
            if (!JRuleDict::instance()->replaceSymbol(JX, name, oldSymbol, newSymbol)) {
                break;      // 修改失败
            }
        }

        // 更新原有变量名
        item->setData(newSymbol, Qt::UserRole);
        break;
    }
    default:
        break;
    }

    // 允许表格内容改变通知
    enableItemChangedNotify(true);
}

void RuleMgrWidget::_emit_tableRuleItemSelectedChanged()
{
    QList<JTableViewSelectionRange> selectionRanges = q_tableRule->view()->selectRanges();
    if (selectionRanges.isEmpty()) {
        q_tableRule->view()->setContextMenuPolicy(Qt::NoContextMenu);
    } else {
        if (GlobalConfig::instance()->canWriteRuleFile()) {
            q_tableRule->view()->setContextMenuPolicy(Qt::ActionsContextMenu);
        }
    }
}

void RuleMgrWidget::_emit_addJX()
{
    NameEditWidget nameEditWidget(this);
    nameEditWidget.setWindowTitle(QStringLiteral("增加机型操作"));
    nameEditWidget.setEditLabelText(QStringLiteral("机型名称："));
    if (nameEditWidget.exec() == QDialog::Rejected) {
        return;
    }

    // 获取当前规则树节点
    QTreeWidgetItem *currentTreeItem = q_treeRule->currentItem();
    if (!currentTreeItem) {
        return;     // 没有选中任何节点
    }

    // 获取机型名称
    QString JX = nameEditWidget.editText();

    // 同步规则字典模块
    if (!JRuleDict::instance()->addJX(JX)) {
        return;     // 增加失败
    }

    //
    QTreeWidgetItem *itemJX = new QTreeWidgetItem(QStringList() << QStringLiteral("机型：%1")
                                                  .arg(JX));
    //itemJX->setFlags(itemJX->flags() | Qt::ItemIsEditable);
    itemJX->setData(0, Qt::UserRole, TreeItemJX);
    itemJX->setData(0, Qt::UserRole + 1, JX);
    currentTreeItem->addChild(itemJX);

    // 更新规则编辑表
    updateTableRule();
}

void RuleMgrWidget::_emit_removeJX()
{
    // 获取当前规则树节点
    QTreeWidgetItem *currentTreeItem = q_treeRule->currentItem();
    if (!currentTreeItem) {
        return;     // 没有选中任何节点
    }

    // 禁止表格内容改变通知
    enableItemChangedNotify(false);

    // 执行删除操作
    removeJX(currentTreeItem);

    // 允许表格内容改变通知
    enableItemChangedNotify(true);
}

void RuleMgrWidget::_emit_addName()
{
    NameEditWidget nameEditWidget(this);
    nameEditWidget.setWindowTitle(QStringLiteral("增加别名操作"));
    nameEditWidget.setEditLabelText(QStringLiteral("别名名称："));
    if (nameEditWidget.exec() == QDialog::Rejected) {
        return;
    }

    // 获取别名名称
    QString name = nameEditWidget.editText();

    // 获取当前规则树节点
    QTreeWidgetItem *currentTreeItem = q_treeRule->currentItem();
    if (!currentTreeItem) {
        return;     // 没有选中任何节点
    }

    // 获取机型
    QString JX = currentTreeItem->data(0, Qt::UserRole + 1).toString();

    // 同步规则字典模块
    bool result = false;
    if (JX.isEmpty()) {
        result = JRuleDict::instance()->addName(name);      // 添加缺省规则别名
    } else {
        result = JRuleDict::instance()->addName(JX, name);  // 添加指定机型的别名
    }
    if (!result) {
        return;     // 增加失败
    }

    //
    QTreeWidgetItem *itemName = new QTreeWidgetItem(QStringList() << name);
    //itemName->setFlags(itemName->flags() | Qt::ItemIsEditable);
    itemName->setData(0, Qt::UserRole, TreeItemName);
    itemName->setData(0, Qt::UserRole + 1, name);
    currentTreeItem->addChild(itemName);

    // 更新规则编辑表
    updateTableRule();
}

void RuleMgrWidget::_emit_removeName()
{
    // 获取当前规则树节点
    QTreeWidgetItem *currentTreeItem = q_treeRule->currentItem();
    if (!currentTreeItem) {
        return;     // 没有选中任何节点
    }

    // 禁止表格内容改变通知
    enableItemChangedNotify(false);

    // 执行删除操作
    removeName(currentTreeItem);

    // 允许表格内容改变通知
    enableItemChangedNotify(true);
}

void RuleMgrWidget::_emit_addSymbol()
{
    NameEditWidget nameEditWidget(this);
    nameEditWidget.setWindowTitle(QStringLiteral("增加变量操作"));
    nameEditWidget.setEditLabelText(QStringLiteral("变量名称："));
    if (nameEditWidget.exec() == QDialog::Rejected) {
        return;
    }

    // 获取变量名称
    QString symbol = nameEditWidget.editText();

    // 获取当前规则树节点
    QTreeWidgetItem *currentTreeItem = q_treeRule->currentItem();
    if (!currentTreeItem) {
        return;     // 没有选中任何节点
    }

    // 获取机型节点
    QTreeWidgetItem *itemJX = currentTreeItem->parent();
    if (!itemJX || itemJX->data(0, Qt::UserRole).toInt() != TreeItemJX) {
        return;     // 节点无效或为非机型节点
    }

    // 获取机型
    QString JX = itemJX->data(0, Qt::UserRole + 1).toString();

    // 获取别名
    QString name = currentTreeItem->data(0, Qt::UserRole + 1).toString();

    // 同步规则字典模块
    bool result = false;
    if (JX.isEmpty()) {
        result = JRuleDict::instance()->addSymbol(name, QStringList() << symbol);       // 添加缺省规则变量
    } else {
        result = JRuleDict::instance()->addSymbol(JX, name, QStringList() << symbol);   // 添加指定机型的变量
    }
    if (!result) {
        return;     // 增加失败
    }

    // 更新规则编辑表
    updateTableRule();
}

void RuleMgrWidget::_emit_removeTableRuleItems()
{
    // 获取当前规则树节点
    QTreeWidgetItem *currentTreeItem = q_treeRule->currentItem();
    if (!currentTreeItem) {
        return;     // 没有选中任何节点
    }

    // 获取节点类型
    QVariant type = currentTreeItem->data(0, Qt::UserRole);
    if (type.isNull()) {
        return;     // 节点类型无效
    }

    // 获取选中的所有行
    QList<JTableViewSelectionRange> selectionRanges = q_tableRule->view()->selectRanges();
    if (selectionRanges.isEmpty()) {
        return;     // 没有选中任何行
    }

    // 筛选出行信息
    QMap<int/*row*/, QString/*name*/> mapRowName;
    QListIterator<JTableViewSelectionRange> citerRange(selectionRanges);
    while (citerRange.hasNext()) {
        const JTableViewSelectionRange &range = citerRange.next();
        for (int row = range.topRow(); row <= range.bottomRow(); ++row) {
            if (type == TreeItemName) {
                mapRowName[row] = q_tableRule->view()->itemValue(row, 1).toString();
            } else {
                mapRowName[row] = q_tableRule->view()->itemValue(row, 0).toString();
            }
        }
    }

    // 检测参数有效性
    if (mapRowName.isEmpty()) {
        return;     // 无效
    }

    // 弹窗提示
    QMessageBox::StandardButtons buttons = (QMessageBox::StandardButtons)
            QMessageBox::warning(this, QStringLiteral("删除选项操作"), QStringLiteral("是否要删除选中数据？"),
                                 QMessageBox::Yes | QMessageBox::No);
    if (buttons == QMessageBox::No) {
        return;     // 取消删除
    }

    // 禁止表格内容改变通知
    enableItemChangedNotify(false);

    // 获取表格当前
    int itemType = type.toInt();

    // 根据类型进行处理
    switch (itemType) {
    case TreeItemRoot:      // 删除机型
    {
        // 删除选中项（倒叙迭代，方便删除表格行）
        QMapIterator<int, QString> citerRowName(mapRowName);
        citerRowName.toBack();
        while (citerRowName.hasPrevious()) {
            citerRowName.previous();

            // 删除规则文件中的指定机型
            if (!JRuleDict::instance()->removeJX(citerRowName.value())) {
                continue;   // 删除失败
            }

            // 删除规则树中对应的机型节点
            int childCount = currentTreeItem->childCount();
            for (int i = 0; i < childCount; ++i) {
                QTreeWidgetItem *treeItem = currentTreeItem->child(i);
                if (!treeItem) {
                    continue;
                }
                if (treeItem->text(0) == citerRowName.value()) {
                    currentTreeItem->removeChild(treeItem);
                    break;
                }
            }

            // 删除规则编辑表中对应的行
            q_tableRule->view()->removeRow(citerRowName.key());
        }
        break;
    }
    case TreeItemJX:        // 删除别名
    {
        // 获取机型
        QString JX = currentTreeItem->data(0, Qt::UserRole + 1).toString();

        // 删除选中项（倒叙迭代，方便删除表格行）
        QMapIterator<int, QString> citerRowName(mapRowName);
        citerRowName.toBack();
        while (citerRowName.hasPrevious()) {
            citerRowName.previous();

            // 删除规则文件中的指定机型
            bool result = false;
            if (JX.isEmpty()) {
                result = JRuleDict::instance()->removeName(citerRowName.value());
            } else {
                result = JRuleDict::instance()->removeName(JX, citerRowName.value());
            }
            if (!result) {
                continue;   // 删除失败
            }

            // 删除树种对应的机型节点
            int childCount = currentTreeItem->childCount();
            for (int i = 0; i < childCount; ++i) {
                QTreeWidgetItem *treeItem = currentTreeItem->child(i);
                if (!treeItem) {
                    continue;
                }
                if (treeItem->text(0) == citerRowName.value()) {
                    currentTreeItem->removeChild(treeItem);
                    break;
                }
            }

            // 删除规则编辑表中对应的行
            q_tableRule->view()->removeRow(citerRowName.key());
        }
        break;
    }
    case TreeItemName:      // 删除变量
    {
        // 获取机型节点
        QTreeWidgetItem *itemJX = currentTreeItem->parent();
        if (!itemJX || itemJX->data(0, Qt::UserRole).toInt() != TreeItemJX) {
            return;     // 节点无效或为非机型节点
        }

        // 获取机型
        QString JX = itemJX->data(0, Qt::UserRole + 1).toString();

        // 获取别名
        QString name = currentTreeItem->data(0, Qt::UserRole + 1).toString();

        // 删除变量
        bool result = false;
        if (JX.isEmpty()) {
            result = JRuleDict::instance()->removeSymbol(name, mapRowName.values());
        } else {
            result = JRuleDict::instance()->removeSymbol(JX, name, mapRowName.values());
        }
        if (!result) {
            break;   // 删除失败
        }

        // 删除规则编辑表中对应的行
        removeTableRuleRows(mapRowName.keys());

        break;
    }
    default:
        break;
    }

    // 允许表格内容改变通知
    enableItemChangedNotify(true);
}

void RuleMgrWidget::enableItemChangedNotify(bool enabled)
{
    disconnect(q_tableRule->view(), SIGNAL(itemChanged(QStandardItem*)), this,
               SLOT(_emit_tableRuleItemChanged(QStandardItem*)));

    if (enabled) {
        connect(q_tableRule->view(), SIGNAL(itemChanged(QStandardItem*)), this,
                SLOT(_emit_tableRuleItemChanged(QStandardItem*)));
    }
}

bool RuleMgrWidget::removeJX(QTreeWidgetItem *itemJX)
{
    // 检测参数有效性
    if (!itemJX) {
        return false;   // 无效
    }

    // 获取机型
    QString JX = itemJX->data(0, Qt::UserRole + 1).toString();
    if (JX.isEmpty()) {
        return false;   // 缺省规则不允许删除
    }

    // 弹窗提示
    QMessageBox::StandardButtons buttons = (QMessageBox::StandardButtons)
            QMessageBox::warning(this, QStringLiteral("删除机型操作"), QStringLiteral("是否要删除机型[%1]？").arg(JX),
                                 QMessageBox::Yes | QMessageBox::No);
    if (buttons == QMessageBox::No) {
        return false;   // 取消删除
    }

    // 删除机型JX
    JRuleDict::instance()->removeJX(JX);

    // 删除机型节点
    QTreeWidgetItem *rootItem = itemJX->parent();
    if (!rootItem) {
        return false;   // 未找到根节点
    }
    rootItem->removeChild(itemJX);

    // 更新规则编辑表
    updateTableRule();

    return true;
}

bool RuleMgrWidget::removeName(QTreeWidgetItem *itemName)
{
    // 检测参数有效性
    if (!itemName) {
        return false;   // 无效
    }

    // 获取机型节点
    QTreeWidgetItem *itemJX = itemName->parent();
    if (!itemJX || itemJX->data(0, Qt::UserRole).toInt() != TreeItemJX) {
        return false;   // 节点无效或为非机型节点
    }

    // 获取机型
    QString JX = itemJX->data(0, Qt::UserRole + 1).toString();

    // 获取别名
    QString name = itemName->data(0, Qt::UserRole + 1).toString();
    if (name.isEmpty()) {
        return false;   // 别名无效
    }

    // 弹窗提示
    QMessageBox::StandardButtons buttons = (QMessageBox::StandardButtons)
            QMessageBox::warning(this, QStringLiteral("删除别名操作"), QStringLiteral("是否要删除机型[%1]下的别名[%2]？")
                                 .arg(JX.isEmpty() ? itemJX->text(0) : JX)
                                 .arg(name),
                                 QMessageBox::Yes | QMessageBox::No);
    if (buttons == QMessageBox::No) {
        return false;   // 取消删除
    }

    // 删除别名
    if (JX.isEmpty()) {
        JRuleDict::instance()->removeName(name);      // 删除缺省规则下的别名
    } else {
        JRuleDict::instance()->removeName(JX, name);  // 删除指定机型下的别名
    }

    // 删除别名节点
    itemJX->removeChild(itemName);

    // 更新规则编辑表
    updateTableRule();

    return true;
}

void RuleMgrWidget::removeTableRuleRows(const QList<int> rows)
{
    for (int i = rows.count() - 1; i >= 0; --i) {
        q_tableRule->view()->removeRow(rows.at(i));
    }
}
