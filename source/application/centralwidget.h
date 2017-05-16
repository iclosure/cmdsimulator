#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>

class QStackedWidget;
class DataMgrWidget;
class DataQueryWidget;
class DataAnalyseWidget;

class CentralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CentralWidget(QWidget *parent = 0);

    bool init();

Q_SIGNALS:
    void currentModelChanged(const QString &model);

public Q_SLOTS:
    void setCurrentModel(const QString &model);
    void setCurrentIndex(const QString &model);

private:
    QStackedWidget *q_stackedWidget;
    DataMgrWidget *q_dataMgrWidget;
    DataQueryWidget *q_dataQueryWidget;
    DataAnalyseWidget *q_dataAnalyseWidget;
};

#endif // CENTRALWIDGET_H
