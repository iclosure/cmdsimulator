#ifndef DATAIMPORTMGR_H
#define DATAIMPORTMGR_H

#include <QWidget>
#include <QFutureWatcher>

// - class DataImportMgr -

class QRadioButton;
class QPushButton;
class QTreeWidget;
class JTableView;
class QTreeWidgetItem;
class QComboBox;
class QCheckBox;
struct stSignalSiftInfo;

class DataImportMgr : public QWidget
{
    Q_OBJECT
public:
    explicit DataImportMgr(QWidget *parent = 0);

    bool init();

Q_SIGNALS:

public Q_SLOTS:
    void _emit_ruleModeChanged();
    void _emit_buttonImportClicked();
    void _emit_treeFilePathsItemSelectChanged();
    void _emit_buttonSaveClicked();

    void updateTreeWidgetState(QTreeWidgetItem *item, int column);

    void onImportFinished();

private:
    void updateTreeWidgetFilePaths(const QStringList &filePaths);
    void updateTreeWidgetSiftBefore();
    void updateTreeWidgetSiftAfter();

    //
    QStringList filePathsOfJX(const QString &JX) const;

private:
    static bool taskImportData(DataImportMgr *_this,
                               bool isGlobalMode,
                               int importMode,
                               bool isIgnoreSift,
                               const QMap<QString, QList<stSignalSiftInfo> > &mapJXSift);

private:
    QRadioButton *q_radioButtonGlobalRule;
    QRadioButton *q_radioButtonJXRule;
    QPushButton *q_buttonImport;
    QTreeWidget *q_treeWidgetFilePaths;
    JTableView *q_tableSymbols;
    QTreeWidget *q_treeWidgetSiftBefore;
    QTreeWidget *q_treeWidgetSiftAfter;
    QComboBox *q_comboBoxImportMode;
    QCheckBox *q_checkBoxIgnoreSift;
    QPushButton *q_buttonSave;
    QFutureWatcher<bool> *q_futureWatcher;
};

#endif // DATAIMPORTMGR_H
