#ifndef DATAMGRWIDGET_H
#define DATAMGRWIDGET_H

#include <QWidget>
#include "customtype.h"

class QTabWidget;
class DataQueryAndDisplay;
class DataImportMgr;
class DatabaseSampling;
class DatabaseMgr;
class RuleMgrWidget;

class DataMgrWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DataMgrWidget(QWidget *parent = 0);

    bool init();

    DataQueryAndDisplay *dataQueryAndDisplay();
    DataImportMgr *databaseMgr();
    RuleMgrWidget *ruleMgrWidget();

Q_SIGNALS:
    void switchManagerToQuery(const JTableSelInfo &);

public Q_SLOTS:

private:
    QTabWidget *q_tabWidget;
    DataQueryAndDisplay *q_dataQueryAndDisplay;
    DataImportMgr *q_dataImportMgr;
    DatabaseSampling *q_databaseSampling;
    DatabaseMgr *q_databaseMgr;
    RuleMgrWidget *q_ruleMgrWidget;
};

#endif // DATAMGRWIDGET_H
