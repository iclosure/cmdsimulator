#ifndef DATABASEMGR_H
#define DATABASEMGR_H

#include <QWidget>

// - class DatabaseMgr -

class QToolBox;
class QListWidget;
class JFilterTableView;
class QCheckBox;
class QPushButton;

class DatabaseMgr : public QWidget
{
    Q_OBJECT
public:
    explicit DatabaseMgr(QWidget *parent = 0);

    bool init();

    JFilterTableView *getTableSignalSift() const;
    void setTableSignalSift(JFilterTableView *value);

Q_SIGNALS:

public Q_SLOTS:
    void updateSignalTable();
    void updateTableSignalSift();
    void signalSiftSelectAll(bool bSelectAll);
    void _emit_buttonSaveClicked();
    void _emit_tableSignalSiftSelectChanged();

private:
    void updateToolBoxItemText(int index, const QString &prefix, const QString &value);

private:
    QToolBox *q_toolBoxSignalTables;
    QListWidget *q_listWidgetSignalTable;
    QPushButton *q_buttonReload;
    JFilterTableView *q_tableSignalSiftBefore;
    QCheckBox *q_checkSelectAll;
    QPushButton *q_buttonRestore;
    QPushButton *q_buttonSave;
};

#endif // DATABASEMGR_H
