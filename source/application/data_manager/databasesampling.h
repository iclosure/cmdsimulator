#ifndef DATABASESAMPLING_H
#define DATABASESAMPLING_H

#include <QWidget>
#include <QFutureWatcher>

// - class DatabaseMgr -

class QToolBox;
class QListWidget;
class QCheckBox;
class QPushButton;
class QTimeEdit;
class QSpinBox;
class QProgressBar;
class JBusyIndicator;
class QListWidget;

class DatabaseSampling : public QWidget
{
    Q_OBJECT
public:
    explicit DatabaseSampling(QWidget *parent = 0);

    bool init();

Q_SIGNALS:

public Q_SLOTS:
    void onSignalTableChanged(int);
    void onActionRemoveTable();
    void onIntervalChanged(int);
    void onButtonReloadClicked();
    void onButtonConvertClicked();
    void onButtonRestoreClicked();
    void onButtonRemoveJCClicked();

    void updateSignalTable();
    void updateJCList();
    void updateTolBoxItemText();

    void onSamplingFinished();
    void onRemoveJCFnished();
    void onRemoveTableFnished();

private:
    static bool taskSamplingRecordData(const QString &suffix, int interval);
    static bool taskRemoveJCData(const QList<int> &jcs);
    static bool taskRemoveTableData(const QString &suffix);

private:
    QToolBox *q_toolBoxSignalTables;
    QListWidget *q_listWidgetSignalTable;
    QSpinBox *q_spinBoxInterval;
    QPushButton *q_buttonReload;
    QPushButton *q_buttonConvert;
    QToolBox *q_toolBoxJC;
    QListWidget *q_listWidgetJC;
    QPushButton *q_buttonRestore;
    QPushButton *q_buttonRemoveJC;
    QProgressBar *q_progressBar;
    QFutureWatcher<bool> *q_futureWatcherSampling;
    QFutureWatcher<bool> *q_futureWatcherRemoveJC;
    QFutureWatcher<bool> *q_futureWatcherRemoveTable;
};

#endif // DATABASESAMPLING_H
