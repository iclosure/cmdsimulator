#ifndef RULEMGRWIDGET_H
#define RULEMGRWIDGET_H

#include <QDialog>

// 树节点类型
enum TreeItemType
{
    TreeItemInvalid = -1,
    TreeItemRoot,
    TreeItemJX,
    TreeItemSymbol,
    TreeItemName,
    TreeItemFilePath
};

// - class NameEditWidget -

class QLabel;
class QLineEdit;

class NameEditWidget : public QDialog
{
    Q_OBJECT
public:
    explicit NameEditWidget(QWidget *parent = 0);

    void setEditLabelText(const QString &text);

    QString editText() const;

private:
    QLabel *q_labelName;
    QLineEdit *q_editName;
};

// - class RuleMgrWidget -

class QLineEdit;
class QPushButton;
class QTreeWidget;
class QTreeWidgetItem;
class JFilterTableView;
class QStandardItem;

class RuleMgrWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RuleMgrWidget(QWidget *parent = 0);

    bool init();

    void updateTreeRule();
    void updateTableRule();

Q_SIGNALS:

public Q_SLOTS:
    void _emit_itemPressed(QTreeWidgetItem *item, int column);
    void _emit_itemSelectionChanged();
    void _emit_tableRuleItemChanged(QStandardItem *item);
    void _emit_tableRuleItemSelectedChanged();

    void _emit_addJX();
    void _emit_removeJX();
    void _emit_addName();
    void _emit_removeName();
    void _emit_addSymbol();
    void _emit_removeTableRuleItems();

private:
    void enableItemChangedNotify(bool enabled = true);

    bool removeJX(QTreeWidgetItem *itemJX);
    bool removeName(QTreeWidgetItem *itemName);

    void removeTableRuleRows(const QList<int> rows);

private:
    QLineEdit *q_editRulePath;
    QPushButton *q_buttonView;
    QTreeWidget *q_treeRule;
    JFilterTableView *q_tableRule;
};

#endif // RULEMGRWIDGET_H
