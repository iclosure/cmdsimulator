#ifndef SIFTAREASETTINGS_H
#define SIFTAREASETTINGS_H

#include <QWidget>
#include "jsqlcustomtype.h"

// - class SiftAreaSettings -

class JTableView;
class JXmlTable;
class QComboBox;
class QPushButton;
class QStandardItem;

class SiftAreaSettings : public QWidget
{
    Q_OBJECT
public:
    explicit SiftAreaSettings(QWidget *parent = 0);

    bool init();

    void setFileNameInfo(const JFileNameInfo &fileNameInfo);
    void setBaseRange(const QRectF &range, bool notify = false);

    bool setEnabledContentChange(bool enabled);

    const JTableView *tableView() const;

Q_SIGNALS:
    void siftAreaChanged(const QRectF &range);

public Q_SLOTS:
    void updateSiftArea();
    void clear();

    void _emit_currentSuffixChanged(int index);
    void _emit_symbolChanged(int index);

    void _emit_xmlTableItemSelectedChanged();
    void _emit_removeXmlTableItems();
    void _emit_buttonAddClicked();

private:
    bool q_enabledContentChange;
    JXmlTable *q_xmlTable;
    QComboBox *q_comboBoxSuffix;
    QComboBox *q_comboBoxSymbol;
    QPushButton *q_buttonAdd;
    JFileNameInfo q_fileNameInfo;
    QRectF q_baseRange;
};

#endif // SIFTAREASETTINGS_H
