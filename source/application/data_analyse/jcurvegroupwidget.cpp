#include "precomp.h"
#include "jcurvegroupwidget.h"
#include "../globalconfig.h"

// - class JCurveGroupWidget -

JCurveGroupWidget::JCurveGroupWidget(QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout *horiLayoutMain = new QHBoxLayout(this);
    horiLayoutMain->setContentsMargins(2, 2, 2, 2);

    q_scrollAreaCurve = new QScrollArea(this);
    q_scrollAreaCurve->setFrameShape(QFrame::NoFrame);
    q_scrollAreaCurve->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    q_scrollAreaCurve->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    q_scrollAreaCurve->setWidgetResizable(true);
    horiLayoutMain->addWidget(q_scrollAreaCurve);

    QWidget* curveArea = new QWidget(q_scrollAreaCurve);
    curveArea->setObjectName("curveArea");
    q_scrollAreaCurve->setWidget(curveArea);

    q_vertLayoutCurveArea = new QVBoxLayout(curveArea);
    q_vertLayoutCurveArea->setContentsMargins(0, 0, 0, 0);
    q_vertLayoutCurveArea->setSpacing(6);
}

bool JCurveGroupWidget::addCurveView(const QString &suffix, const QString &nameX,
                                     const QString &nameY, int axisXYType)
{
    if (axisXYType == JCurveWidget::AxisXY_Invalid) {
        return false;
    }

    JCurveWidget *curveView = new JCurveWidget(this, true);
    curveView->setMinimumHeight(300);
    curveView->setAxisXYType((JCurveWidget::AxisXYType)axisXYType);
    curveView->setTitle(QString("%2 - %3 (<font color='#aaf'>%1</font>)")
                        .arg(QString(suffix).replace('_', '-')).arg(nameX).arg(nameY));
    curveView->setProperty("suffix", suffix);
    curveView->setProperty("nameX", nameX);
    curveView->setProperty("nameY", nameY);
    //
    curveView->setLegendVisible(false);
    curveView->setEnableDbClick(true);
    curveView->setBaseLineVisible(false);
    curveView->setBaseLineEnabled(false);
    //
    if (!GlobalConfig::instance()->canExportData()) {
        curveView->setExportVisible(false);
    }
    //
    if (axisXYType == JCurveWidget::AxisXY_Time_Time
            || axisXYType == JCurveWidget::AxisXY_Time_Value) {
        if (GlobalConfig::instance()->canWriteDatabase()) {
            curveView->setEditEnabled(true);
        }
        curveView->setZoomModeVisible(true);
        curveView->setSiftModeVisible(true);
    } else {
        curveView->setEditEnabled(false);
        curveView->setZoomModeVisible(false);
        curveView->setSiftModeVisible(false);
    }
    curveView->setZoomMode(true);
    //
    q_curveViews.append(curveView);
    q_vertLayoutCurveArea->addWidget(curveView);

    //
    updateCurve(curveView);

    //
    //updateBaseLine(curveView);

    //
    int currentIndex = q_curveViews.count() - 1;

    //
    connect(curveView, &JCurveWidget::doubleClicked, [=](bool checked){
        //
        bool zoomEnabled = (q_curveViews.count() < 3) ? true : checked;
        QListIterator<JCurveWidget *> citer(q_curveViews);
        while (citer.hasNext()) {
            JCurveWidget *item = citer.next();
            if (item == 0) {
                continue;
            }
            //
            if (item != curveView) {
                item->setVisible(!checked);
            }
            //
            if (!item->editEnabled()) {
                item->setZoomMode(zoomEnabled);
            }
        }
        //
        Q_EMIT curveViewDoubleClicked(currentIndex, checked);
    });
    connect(curveView, &JCurveWidget::editClicked, [=](){
        //
        Q_EMIT editClicked(currentIndex);
    });
    connect(curveView, &JCurveWidget::timeBandApply,
            [=](const QTime &minimum, const QTime &maximum, bool retain){
        //
        Q_EMIT timeBandApply(currentIndex, minimum, maximum, retain);
    });

    return true;
}

void JCurveGroupWidget::removeCurveView(int index)
{
    //
    if (index < 0 || index >= q_curveViews.count()) {
        return;
    }

    //
    JCurveWidget *curveView = q_curveViews.takeAt(index);
    q_vertLayoutCurveArea->removeWidget(curveView);
    curveView->deleteLater();
}

void JCurveGroupWidget::clearCurves()
{
    QListIterator<JCurveWidget *> citer(q_curveViews);
    while (citer.hasNext()) {
        JCurveWidget *curveView = citer.next();
        q_vertLayoutCurveArea->removeWidget(curveView);
        curveView->deleteLater();
    }
    q_curveViews.clear();
}

void JCurveGroupWidget::focusCurveView(int index)
{
    //
    if (index < 0 || index >= q_curveViews.count()) {
        return;
    }

    //
    JCurveWidget *curveView = q_curveViews.at(index);
    if (curveView == 0) {
        return;
    }

    //
    q_scrollAreaCurve->ensureWidgetVisible(curveView);
}

void JCurveGroupWidget::setZoomEnabled(bool enabled)
{
    //
    QListIterator<JCurveWidget *> citer(q_curveViews);
    if (q_curveViews.count() < 3) {
        while (citer.hasNext()) {
            citer.next()->setZoomMode(true);
        }
    } else {
        while (citer.hasNext()) {
            citer.next()->setZoomMode(enabled);
        }
    }
}

void JCurveGroupWidget::setSiftAreas(const QList<QRectF> &siftAreas)
{
    q_siftAreas = siftAreas;
}

void JCurveGroupWidget::updateCurves(int index)
{
    //
    JAutoCursor autoCursor(Qt::BusyCursor);

    if (index < 0) {
        QListIterator<JCurveWidget *> citer(q_curveViews);
        while (citer.hasNext()) {
            updateCurve(citer.next());
        }
    } else if (index < q_curveViews.count()) {
        updateCurve(q_curveViews.at(index));
    } else {
        return;
    }
}

void JCurveGroupWidget::hidePopupWidget()
{
    //
    QListIterator<JCurveWidget *> citerCurveViews(q_curveViews);
    while (citerCurveViews.hasNext()) {
        JCurveWidget *curveWidget = citerCurveViews.next();
        curveWidget->setEditWidgetVisible(false);
    }
}

void JCurveGroupWidget::updateCurve(JCurveWidget *curveView)
{
    if (!curveView) {
        return;
    }

    // 清空旧数据
    curveView->clearCurve();

    // 获取横、纵坐标信号量
    const QString suffix = curveView->property("suffix").toString().replace('-', '_');
    const QString nameX = curveView->property("nameX").toString();
    const QString nameY = curveView->property("nameY").toString();
    QString sectionX = nameX.section(" (", 0, 0);
    QString sectionY = nameY.section(" (", 0, 0);

    //
    QString legendTitle = nameY % " (" % suffix % ")";
    JCurveWidget::AxisXYType axisXYType = curveView->axisXYType();

    // 获取M-H信号名称
    QString machSection, heightSection;
    if (!JFlyRecord::instance()->readSignalMHSection(
                JSqlDataMgr::instance()->signalTableNamePrefix().append(suffix),
                machSection, heightSection)) {
        return;
    }

    // 生成数据表名称
    const QString recordTableName =
            JSqlDataMgr::instance()->recordTableNamePrefix().append(suffix);

    //
    if (axisXYType == JCurveWidget::AxisXY_Value_Value) {
        // 获取记录数据
        QVector<QPointF> data;
        QListIterator<QRectF> citerSiftAreas(q_siftAreas);
        while (citerSiftAreas.hasNext()) {
            const QRectF &range = citerSiftAreas.next();
            //
            QVector<QPointF> tempData;
            if (JSqlDataMgr::instance()->readRecordDataV(recordTableName, sectionX, sectionY,
                                                         machSection,
                                                         heightSection,
                                                         range, tempData)) {
                data << tempData;
            }
        }

        // 检测是否有数据需要处理
        if (data.isEmpty()) {
            return;   // 无数据需要绘制
        }
        //
        curveView->updateData(curveView->addCurve(legendTitle), data);
    } else {
        // 获取记录数据
        if (axisXYType == JCurveWidget::AxisXY_Value_Time) {
            qSwap(sectionX, sectionY);
        }

        //
        QList<QPair<QTime, qreal> > data;
        QListIterator<QRectF> citerSiftAreas(q_siftAreas);
        while (citerSiftAreas.hasNext()) {
            const QRectF &range = citerSiftAreas.next();
            //
            QList<QPair<QTime, qreal> > tempData;
            if (JSqlDataMgr::instance()->readRecordDataV(recordTableName, sectionX, sectionY,
                                                         machSection,
                                                         heightSection,
                                                         range, tempData)) {
                data.append(tempData);
            }
        }

        // 检测是否有数据需要处理
        if (data.isEmpty()) {
            return;   // 无数据需要绘制
        }
        //
        curveView->updateData(curveView->addCurve(legendTitle), data);
    }
}

void JCurveGroupWidget::updateBaseLine(JCurveWidget *curveView)
{
    // 获取母线图数据点
    QList<QVector<QPointF> > groupsData;
    if (!JMyExcel::instance()->readBaseLine(groupsData)) {
        return;
    }

    //
    curveView->setBaseLineSamples(groupsData);
}
