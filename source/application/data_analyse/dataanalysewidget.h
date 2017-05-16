#ifndef DATAANALYSEWIDGET_H
#define DATAANALYSEWIDGET_H

#include <QWidget>
#include "customtype.h"

// - class DataAnalyseWidget -

class QLineEdit;
class QComboBox;
class DataQueryWidget;
class QTableWidget;
class QPushButton;
class JCurveGroupWidget;
class QTableWidgetItem;

class DataAnalyseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DataAnalyseWidget(QWidget *parent = 0);

    bool init();

    QList<QRectF> siftAreas() const;
    void setSiftAreas(const QList<QRectF> &ranges);

    JFileNameInfo fileNameInfo() const;

    void hidePopupWidget();

    void clearCoords();
    void clearCurves();

Q_SIGNALS:

public Q_SLOTS:
    bool updateSift(const JTableSelInfo &info);
    void _emit_axisXChanged(int index);
    void _emit_suffixChanged(int index);
    void _emit_curveDoubleClicked(int index, bool checked);
    void _emit_curveEditClicked(int index);
    void _emit_curveTimeBandApply(int index, const QTime &minimum, const QTime &maximum, bool retain);
    void _emit_buttonAddCoordClicked();
    void _emit_buttonClearCoordClicked();
    void _emit_focusCoord(int index);
    void _emit_removeCoord(int index);

private:
    void addOneCoord(const QString &nameX, const QString &nameY);
    void removeOneCoord(int index);

private:
    QWidget *q_widgetAreaLeft;
    QTableWidget *q_tableWidgetCoord;
    QPushButton *q_buttonAddCoord;
    QPushButton *q_buttonClearCoord;
    JCurveGroupWidget *q_curveGroupWidget;
    QLineEdit *q_lineEditJX;
    QLineEdit *q_lineEditJH;
    QLineEdit *q_lineEditMinRQ;
    QLineEdit *q_lineEditMaxRQ;
    QLineEdit *q_lineEditJC;
    QComboBox *q_comboBoxSuffix;
    QComboBox *q_comboBoxAxisX;
    QComboBox *q_comboBoxAxisY;

    QList<QRectF> q_siftAreas;
    QList<stSignalInfo> q_signalInfos;

    bool q_notifyEnabled;
};

#endif // DATAANALYSEWIDGET_H
