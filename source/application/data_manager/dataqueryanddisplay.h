#ifndef DATAQUERYANDDISPLAY_H
#define DATAQUERYANDDISPLAY_H

#include <QWidget>
#include "customtype.h"
#include "jtableview.h"

class JTableView;
class QPushButton;
class QSpinBox;
class QLabel;

class JLiteTableView : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int rowOffset READ rowOffset)
    Q_PROPERTY(int pageCount READ pageCount)
    Q_PROPERTY(int pageRowCount READ pageRowCount)
    Q_PROPERTY(int totalRowCount READ totalRowCount)
public:
    explicit JLiteTableView(QWidget *parent = 0);

    void setTotalRowCount(int count);
    void setCurrentPage(int page);

    JTableView *tableView();

    int rowOffset() const;
    int pageCount() const;
    int pageRowCount() const;
    int totalRowCount() const;

Q_SIGNALS:
    void pageIndexChanged(int value);

private:
    JTableView *q_tableView;
    int q_pageRowCount;
    int q_totalRowCount;

    QPushButton *q_buttonPrev;
    QPushButton *q_buttonNext;
    QSpinBox *q_spinBoxPage;
    QLabel *q_labelTotalRowCount;
};

class QToolBox;
class QListWidget;
class QComboBox;
class QPushButton;
class QTabWidget;
class QStandardItem;

class DataQueryAndDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit DataQueryAndDisplay(QWidget *parent = 0);

    bool init();

    bool setUpdateTableEnabled(bool enabled);

    void addOnePageTable(const QString &suffix);

    QString currentJX() const;
    QString currentJH() const;
    QString currentRQ() const;
    QString currentJC() const;

    static QString parseSiftJX(const QString &text);
    static QString parseSiftJH(const QString &text);
    static QString parseSiftRQ(const QString &text);
    static int parseSiftJC(const QString &text);

Q_SIGNALS:
    void tableItemDoubleClicked(const JTableSelInfo &);

public Q_SLOTS:
    bool updateSift();
    bool updateTable();
    void _emit_tableItemDoubleClicked(QStandardItem *item);

    void _emit_tablePageIndexChanged(int);

private:
    void updateToolBoxItemText(int index, const QString &prefix, const QString &value);

private:
    QToolBox *q_toolBoxSift;
    QListWidget *q_listWidgetJX;
    QListWidget *q_listWidgetJH;
    QListWidget *q_listWidgetRQ;
    QListWidget *q_listWidgetJC;
    QPushButton *q_buttonReload;
    QPushButton *q_buttonUpdate;
    QTabWidget *q_tabWidgetTable;
    bool q_updateTableEnabled;
};

#endif // DATAQUERYANDDISPLAY_H
