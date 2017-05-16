#include "precomp.h"
#include "dataanalysewidget.h"
#include "jcurvewidget.h"
#include "jsqldatamgr.h"
#include "jsqltablemgr.h"
#include "../data_manager/dataqueryanddisplay.h"
#include "../data_query/dataquerywidget.h"
#include "globalconfig.h"
#include "jlistcoorditem.h"
#include "jcurvegroupwidget.h"

// - class DataAnalyseWidget -

DataAnalyseWidget::DataAnalyseWidget(QWidget *parent) :
    QWidget(parent),
    q_notifyEnabled(false)
{
    QHBoxLayout *horiLayoutMain = new QHBoxLayout(this);
    horiLayoutMain->setContentsMargins(2, 2, 2, 2);

    JSplitter *splitter = new JSplitter(this);
    splitter->setHandleWidth(6);
    splitter->setHandleColor(QColor(100, 100, 100, 30));
    splitter->setScales(QList<double>() << 1 << 2.5);
    horiLayoutMain->addWidget(splitter);

    // left

    q_widgetAreaLeft = new QWidget(this);
    splitter->addWidget(q_widgetAreaLeft);

    QVBoxLayout *vertLayoutLeft = new QVBoxLayout(q_widgetAreaLeft);
    vertLayoutLeft->setContentsMargins(0, 0, 0, 0);

    QGroupBox *groupBoxInfo = new QGroupBox(QStringLiteral("查询信息"), q_widgetAreaLeft);
    vertLayoutLeft->addWidget(groupBoxInfo);

    QFormLayout *formLayoutInfo = new QFormLayout(groupBoxInfo);

    q_lineEditJX = new QLineEdit(groupBoxInfo);
    formLayoutInfo->addRow(QStringLiteral("机型："), q_lineEditJX);

    q_lineEditJH = new QLineEdit(this);
    formLayoutInfo->addRow(QStringLiteral("机号："), q_lineEditJH);

    QWidget *widgetRQ = new QWidget(groupBoxInfo);
    formLayoutInfo->addRow(QStringLiteral("日期："), widgetRQ);

    QHBoxLayout *horiLayoutRQ = new QHBoxLayout(widgetRQ);
    horiLayoutRQ->setContentsMargins(0, 0, 0, 0);
    horiLayoutRQ->setSpacing(3);

    q_lineEditMinRQ = new QLineEdit(groupBoxInfo);
    q_lineEditMaxRQ = new QLineEdit(groupBoxInfo);

    horiLayoutRQ->addWidget(q_lineEditMinRQ);
    QLabel *labelRQ = new QLabel(QStringLiteral("~"), widgetRQ);
    labelRQ->setFixedWidth(QFontMetrics(labelRQ->font()).width(labelRQ->text()));
    horiLayoutRQ->addWidget(labelRQ);
    horiLayoutRQ->addWidget(q_lineEditMaxRQ);

    q_lineEditJC = new QLineEdit(this);
    formLayoutInfo->addRow(QStringLiteral("架次："), q_lineEditJC);

    //
    groupBoxInfo->adjustSize();
    groupBoxInfo->setFixedHeight(groupBoxInfo->sizeHint().height());

    QGroupBox *groupBoxSettings = new QGroupBox(QStringLiteral("设置"), q_widgetAreaLeft);
    vertLayoutLeft->addWidget(groupBoxSettings);

    QVBoxLayout *vertLayoutSettings = new QVBoxLayout(groupBoxSettings);
    vertLayoutSettings->setContentsMargins(2, 2, 2, 2);
    vertLayoutSettings->setSpacing(2);

    q_tableWidgetCoord = new QTableWidget(groupBoxSettings);
    q_tableWidgetCoord->setObjectName("tableWidgetCoord");
    q_tableWidgetCoord->horizontalHeader()->hide();
    q_tableWidgetCoord->verticalHeader()->hide();
    q_tableWidgetCoord->horizontalHeader()->setStretchLastSection(true);
    q_tableWidgetCoord->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    q_tableWidgetCoord->setColumnCount(1);
    vertLayoutSettings->addWidget(q_tableWidgetCoord);

    QHBoxLayout *horiLayoutCoord = new QHBoxLayout();
    horiLayoutCoord->setContentsMargins(0, 0, 0, 0);
    horiLayoutCoord->setSpacing(2);
    vertLayoutSettings->addLayout(horiLayoutCoord);

    QFormLayout *formLayoutSettings = new QFormLayout();
    horiLayoutCoord->addLayout(formLayoutSettings);

    q_comboBoxSuffix = new QComboBox(groupBoxSettings);
    formLayoutSettings->addRow(QStringLiteral("表名："), q_comboBoxSuffix);

    q_comboBoxAxisX = new QComboBox(groupBoxSettings);
    formLayoutSettings->addRow(QStringLiteral("横轴："), q_comboBoxAxisX);

    q_comboBoxAxisY = new QComboBox(groupBoxSettings);
    formLayoutSettings->addRow(QStringLiteral("纵轴："), q_comboBoxAxisY);

    QVBoxLayout *vertLayoutButton = new QVBoxLayout();
    vertLayoutButton->setContentsMargins(2, 2, 2, 2);
    vertLayoutButton->setSpacing(2);
    horiLayoutCoord->addLayout(vertLayoutButton);

    int buttonHeight = formLayoutSettings->sizeHint().height() - 2;

    q_buttonAddCoord = new QPushButton(QStringLiteral("增加"), groupBoxSettings);
    q_buttonAddCoord->setAutoDefault(true);
    q_buttonAddCoord->setDefault(true);
    q_buttonAddCoord->setFixedSize(buttonHeight * 0.7, buttonHeight / 2);
    vertLayoutButton->addWidget(q_buttonAddCoord);

    q_buttonClearCoord = new QPushButton(QStringLiteral("清空"), groupBoxSettings);
    q_buttonClearCoord->setFixedSize(buttonHeight * 0.7, buttonHeight / 2);
    vertLayoutButton->addWidget(q_buttonClearCoord);

    // right

    q_curveGroupWidget = new JCurveGroupWidget(this);
    splitter->addWidget(q_curveGroupWidget);

    //
    connect(q_comboBoxSuffix, SIGNAL(currentIndexChanged(int)),
            SLOT(_emit_suffixChanged(int)));
    connect(q_comboBoxAxisX, SIGNAL(currentIndexChanged(int)),
            SLOT(_emit_axisXChanged(int)));
    connect(q_curveGroupWidget, SIGNAL(curveViewDoubleClicked(int,bool)),
            this, SLOT(_emit_curveDoubleClicked(int,bool)));
    connect(q_curveGroupWidget, SIGNAL(editClicked(int)),
            this, SLOT(_emit_curveEditClicked(int)));
    connect(q_curveGroupWidget, SIGNAL(timeBandApply(int,QTime,QTime,bool)),
            this, SLOT(_emit_curveTimeBandApply(int,QTime,QTime,bool)));
    connect(q_buttonAddCoord, &QPushButton::clicked,
            this, &DataAnalyseWidget::_emit_buttonAddCoordClicked);
    connect(q_buttonClearCoord, &QPushButton::clicked,
            this, &DataAnalyseWidget::_emit_buttonClearCoordClicked);
}

bool DataAnalyseWidget::init()
{
    //
    q_lineEditJX->setReadOnly(true);
    q_lineEditJH->setReadOnly(true);
    q_lineEditMinRQ->setReadOnly(true);
    q_lineEditMaxRQ->setReadOnly(true);
    q_lineEditJC->setReadOnly(true);

    clearCoords();
    clearCurves();

    return true;
}

QList<QRectF> DataAnalyseWidget::siftAreas() const
{
    return q_siftAreas;
}

void DataAnalyseWidget::setSiftAreas(const QList<QRectF> &ranges)
{
    q_siftAreas = ranges;

    //
    q_curveGroupWidget->setSiftAreas(ranges);
}

JFileNameInfo DataAnalyseWidget::fileNameInfo() const
{
    // 获取筛选基本信息
    JFileNameInfo fileNameInfo;
    fileNameInfo.JX = DataQueryAndDisplay::parseSiftJX(q_lineEditJX->text());
    fileNameInfo.JH = DataQueryAndDisplay::parseSiftJH(q_lineEditJH->text());
    fileNameInfo.minRQ = QDate::fromString(q_lineEditMinRQ->text(), QStringLiteral("yyyy 年 MM 月 dd 日"));
    fileNameInfo.maxRQ = QDate::fromString(q_lineEditMaxRQ->text(), QStringLiteral("yyyy 年 MM 月 dd 日"));
    fileNameInfo.JC = DataQueryAndDisplay::parseSiftJC(q_lineEditJC->text());
    return fileNameInfo;
}

void DataAnalyseWidget::hidePopupWidget()
{
    //
    q_curveGroupWidget->hidePopupWidget();
}

bool DataAnalyseWidget::updateSift(const JTableSelInfo &info)
{
    if (!GlobalConfig::instance()->canReadDatabase()) {
        return true;
    }

    // 清空旧数据
    q_comboBoxSuffix->clear();
    q_comboBoxAxisX->clear();
    q_comboBoxAxisY->clear();
    clearCoords();
    clearCurves();

    //
    q_lineEditJX->setText(info.baseInfo.JX.isEmpty() ? QStringLiteral("全部") : info.baseInfo.JX);
    q_lineEditJH->setText(info.baseInfo.JH.isEmpty() ? QStringLiteral("全部") : info.baseInfo.JH);
    q_lineEditMinRQ->setText(info.baseInfo.minRQ.toString(QStringLiteral("yyyy 年 MM 月 dd 日")));
    q_lineEditMaxRQ->setText(info.baseInfo.maxRQ.toString(QStringLiteral("yyyy 年 MM 月 dd 日")));
    q_lineEditJC->setText(info.baseInfo.JC == -1 ? QStringLiteral("全部")
                                                 : QString::number(info.baseInfo.JC));

    // 获取所有表后缀名
    QStringList suffixes;
    if (JSqlDataMgr::instance()->readSubTableSuffix(info.baseInfo, suffixes)) {
        QStringListIterator citerSuffixes(suffixes);
        while (citerSuffixes.hasNext()) {
            const QString &suffix = citerSuffixes.next();
            q_comboBoxSuffix->addItem(suffix);
        }
        q_comboBoxSuffix->setCurrentIndex(0);
    }

    return true;
}

void DataAnalyseWidget::_emit_suffixChanged(int index)
{
    // 清空旧数据
    q_comboBoxAxisX->clear();
    q_comboBoxAxisY->clear();

    //
    if (index == -1) {
        return;
    }

    // 获取当前表后缀名
    const QString currentSuffix = q_comboBoxSuffix->currentText();

    // 获取信号表所有信号量
    QList<stSignalInfo> signalInfos;
    if (!JSqlDataMgr::instance()->readSignalData(
                JSqlDataMgr::instance()->signalTableNamePrefix()
                .append(currentSuffix), signalInfos)) {
        return;
    }

    //
    QListIterator<stSignalInfo> citerSignalInfos(signalInfos);
    while (citerSignalInfos.hasNext()) {
        const stSignalInfo &signalInfo = citerSignalInfos.next();
        //
        if (signalInfo.BM.isEmpty()) {
            q_comboBoxAxisX->addItem(signalInfo.YXHM % " (?)");
        } else {
            q_comboBoxAxisX->addItem(signalInfo.YXHM % " (" % signalInfo.BM % ")");
        }
    }

    //
    q_comboBoxAxisX->setCurrentIndex(0);

    //
    _emit_axisXChanged(0);

    //
    q_buttonAddCoord->setEnabled(q_comboBoxAxisX->currentIndex() >= 0
                                 && q_comboBoxAxisY->currentIndex() >= 0);
}

void DataAnalyseWidget::_emit_axisXChanged(int index)
{
    // 清空旧数据
    q_comboBoxAxisY->clear();

    //
    if (index == -1) {
        return;
    }

    //
    int currentAxisX = q_comboBoxAxisX->currentIndex();

    //
    int count = q_comboBoxAxisX->count();
    for (int i = 0; i < count; ++i) {
        if (i == currentAxisX) {
            continue;
        }

        //
        q_comboBoxAxisY->addItem(q_comboBoxAxisX->itemText(i));
    }

    //
    q_buttonAddCoord->setEnabled(q_comboBoxAxisX->currentIndex() >= 0
                                 && q_comboBoxAxisY->currentIndex() >= 0);
}

void DataAnalyseWidget::_emit_curveDoubleClicked(int index, bool checked)
{
    Q_UNUSED(index);
    Q_UNUSED(checked);

    //
    q_widgetAreaLeft->setVisible(!checked);
}

void DataAnalyseWidget::_emit_curveEditClicked(int index)
{
    Q_UNUSED(index);
}

void DataAnalyseWidget::_emit_curveTimeBandApply(int index,
                                                 const QTime &minimum,
                                                 const QTime &maximum,
                                                 bool retain)
{
    if (!GlobalConfig::instance()->canWriteDatabase()) {
        return;
    }

    //
    JAutoCursor autoCursor(Qt::BusyCursor);

    //
    JListCoordItem *itemWidget =
            qobject_cast<JListCoordItem *>(q_tableWidgetCoord->cellWidget(index, 0));
    if (!itemWidget) {
        return;
    }
    //
    int result = QMessageBox::warning(this, QStringLiteral("数据截取"),
                                      QStringLiteral("数据库将被裁剪！是否继续？"),
                                      QStringLiteral("继续"), QStringLiteral("取消"));
    if (result == 1) {
        return; // 取消修改
    }
    //
    const QString suffix = itemWidget->suffix().replace('-', '_');

    //
    if (!JSqlDataMgr::instance()->deleteRecordData(
                JSqlDataMgr::instance()->recordTableNamePrefix().append(suffix),
                itemWidget->symbolX(),
                minimum, maximum, retain)) {
        QMessageBox::warning(this, QStringLiteral("截取结果提示"),
                             QStringLiteral("数据截取失败！失败原因请查看最新日志文件"));
        return;
    }

    //
    q_curveGroupWidget->updateCurves(index);

    //
    QMessageBox::information(this, QStringLiteral("截取结果提示"),
                             QStringLiteral("截取成功！"));
}

void DataAnalyseWidget::_emit_buttonAddCoordClicked()
{
    if (!GlobalConfig::instance()->canReadDatabase()) {
        return;
    }

    //
    const QString nameX = q_comboBoxAxisX->currentText();
    const QString nameY = q_comboBoxAxisY->currentText();

    //
    addOneCoord(nameX, nameY);

    //
    int axisXYType = JCurveWidget::AxisXY_Invalid;
    if (q_comboBoxAxisX->currentIndex() == 0) {
        axisXYType = JCurveWidget::AxisXY_Time_Value;
    } else if (q_comboBoxAxisY->currentIndex() == 0) {
        axisXYType = JCurveWidget::AxisXY_Value_Time;
    } else {
        axisXYType = JCurveWidget::AxisXY_Value_Value;
    }
    q_curveGroupWidget->addCurveView(q_comboBoxSuffix->currentText(),
                                     nameX, nameY, axisXYType);

    //
    q_curveGroupWidget->setZoomEnabled(false);
}

void DataAnalyseWidget::_emit_buttonClearCoordClicked()
{
    //
    clearCoords();

    //
    clearCurves();
}

void DataAnalyseWidget::_emit_focusCoord(int index)
{
    //
    q_curveGroupWidget->focusCurveView(index);
}

void DataAnalyseWidget::_emit_removeCoord(int index)
{
    //
    removeOneCoord(index);

    //
    q_curveGroupWidget->setZoomEnabled(false);
}

void DataAnalyseWidget::addOneCoord(const QString &nameX, const QString &nameY)
{
    // 获取当前表后缀名
    const QString currentSuffix = q_comboBoxSuffix->currentText();
    QString suffix = currentSuffix;
    //
    int rowIndex = q_tableWidgetCoord->rowCount();
    q_tableWidgetCoord->insertRow(rowIndex);
    JListCoordItem *coordItem = new JListCoordItem(rowIndex, nameX, nameY);
    coordItem->setSuffix(suffix.replace('_', '-'));
    q_tableWidgetCoord->setCellWidget(rowIndex, 0, coordItem);
    connect(coordItem, &JListCoordItem::focus,
            this, &DataAnalyseWidget::_emit_focusCoord);
    connect(coordItem, &JListCoordItem::remove,
            this, &DataAnalyseWidget::_emit_removeCoord);
}

void DataAnalyseWidget::removeOneCoord(int index)
{
    //
    q_curveGroupWidget->removeCurveView(index);

    //
    JListCoordItem *cellWidget =
            qobject_cast<JListCoordItem *>(q_tableWidgetCoord->cellWidget(index, 0));
    if (cellWidget) {
        disconnect(cellWidget, &JListCoordItem::focus,
                   this, &DataAnalyseWidget::_emit_focusCoord);
        disconnect(cellWidget, &JListCoordItem::remove,
                   this, &DataAnalyseWidget::_emit_removeCoord);
    }
    q_tableWidgetCoord->removeCellWidget(index, 0);
    q_tableWidgetCoord->removeRow(index);
}

void DataAnalyseWidget::clearCoords()
{
    //
    while (q_tableWidgetCoord->rowCount() > 0) {
        JListCoordItem *cellWidget =
                qobject_cast<JListCoordItem *>(q_tableWidgetCoord->cellWidget(0, 0));
        if (cellWidget) {
            disconnect(cellWidget, &JListCoordItem::focus,
                       this, &DataAnalyseWidget::_emit_focusCoord);
            disconnect(cellWidget, &JListCoordItem::remove,
                       this, &DataAnalyseWidget::_emit_removeCoord);
        }
        q_tableWidgetCoord->removeCellWidget(0, 0);
        q_tableWidgetCoord->removeRow(0);
    }
}

void DataAnalyseWidget::clearCurves()
{
    //
    q_curveGroupWidget->clearCurves();
}
