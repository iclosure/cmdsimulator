#ifndef DATAQUERYWIDGET_H
#define DATAQUERYWIDGET_H

#include <QWidget>
#include "customtype.h"

class JSplitter;
class QLineEdit;
class QDateEdit;
class QComboBox;
class QTabWidget;
class JCurveView;
class JCurveWidget;
class QPushButton;
class JCurve;
class SiftAreaSettings;
class QGroupBox;
class JXmlTable;

class DataQueryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DataQueryWidget(QWidget *parent = 0);

    bool init();

    JFileNameInfo fileNameInfo() const;

    void addOnePageTable(const QString &suffix);

    QList<QRectF> siftAreas() const;

    bool setEnabledContentChange(bool enabled);

Q_SIGNALS:
    void switchQueryToAnalyze(const JTableSelInfo &);

public Q_SLOTS:
    bool updateSift(const JTableSelInfo &info);
    bool updateCoord();
    bool updateBaseLine();
    bool updateCurve();
    void updateSiftAreaSettings();
    bool updateSiftResult(bool updateHotSpot = true);
    void updateTableArea(const QRectF &range);

    void _emit_buttonUpdateClicked();

    void _emit_tableItemDoubleClicked(QStandardItem *item);

    void _emit_curveDoubleClicked();
    void _emit_selectedChanged(int index, JCurve *curve);

    void _emit_siftAreaChanged(int index, const QRectF &range);

    void _emit_buttonExportClicked();

    void _emit_tableAreaContentChanged();

    void _emit_tablePageIndexChanged(int);

private:
    QWidget *q_widgetAreaLeft;
    QComboBox *q_comboBoxJX;
    QComboBox *q_comboBoxJH;
    QDateEdit *q_dateEditMinRQ;
    QDateEdit *q_dateEditMaxRQ;
    QComboBox *q_comboBoxJC;
    QPushButton *q_buttonUpdate;
    JXmlTable *q_tableArea;
    SiftAreaSettings *q_siftAreaSettings;
    QWidget *q_widgetAreaRight;
    JSplitter *q_splitterTop;
    JCurveWidget *q_curveView;
    QTabWidget *q_tabWidgetTable;
    QPushButton *q_buttonExport;

    bool q_enabledContentChange;
};

#endif // DATAQUERYWIDGET_H
