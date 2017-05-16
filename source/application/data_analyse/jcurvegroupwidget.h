#ifndef JCURVEGROUPWIDGET_H
#define JCURVEGROUPWIDGET_H

#include <QWidget>
#include "customtype.h"

// - class JCurveGroupWidget -

class QScrollArea;
class QVBoxLayout;
class JCurveWidget;

class JCurveGroupWidget : public QWidget
{
    Q_OBJECT
public:
    explicit JCurveGroupWidget(QWidget *parent = 0);

    bool addCurveView(const QString &suffix, const QString &nameX,
                      const QString &nameY, int axisXYType);
    void removeCurveView(int index);
    void clearCurves();

    void focusCurveView(int index);

    void setZoomEnabled(bool enabled);

    void setSiftAreas(const QList<QRectF> &siftAreas);

    void updateCurves(int index = -1);

    void hidePopupWidget();

Q_SIGNALS:
    void curveViewDoubleClicked(int index, bool checked);
    void editClicked(int index);
    void timeBandApply(int index, const QTime &minimum, const QTime &maximum, bool retain);

private:
    void updateCurve(JCurveWidget *curveView);
    void updateBaseLine(JCurveWidget *curveView);

private:
    QScrollArea *q_scrollAreaCurve;
    QVBoxLayout *q_vertLayoutCurveArea;
    QList<QRectF> q_siftAreas;
    QList<JCurveWidget *> q_curveViews;
};

#endif // JCURVEGROUPWIDGET_H
