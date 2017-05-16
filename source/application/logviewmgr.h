#ifndef LOGVIEWMGR_H
#define LOGVIEWMGR_H

#include <QDialog>

class QListWidget;
class QListWidgetItem;

class LogViewMgr : public QDialog
{
    Q_OBJECT
public:
    explicit LogViewMgr(QWidget *parent = 0);

protected:
    void done(int code);

public slots:
    void onItemPressed(QListWidgetItem *item);
    void onRemoveItem();

private:
    QListWidget *d_listWidgetLog;
};

#endif // LOGVIEWMGR_H
