#ifndef STATUSBARMGR_H
#define STATUSBARMGR_H

#include <QStatusBar>

// - class StatusBarMgr -

class ModelMgrWidget;
class QLabel;
class QProgressBar;

class StatusBarMgr : public QStatusBar
{
    Q_OBJECT
public:
    explicit StatusBarMgr(QWidget *parent = 0);

    bool init();

Q_SIGNALS:
    void currentModelChanged(const QString &model);
    void currentIndexChanged(const QString &model);

public Q_SLOTS:
    void setCurrentModel(const QString &model);
    void setMessage(const QString &text);

private:
    ModelMgrWidget *q_modelMgrWidget;
    QLabel *q_labelInfo;
    QProgressBar *q_progressBar;
};

#endif // STATUSBARMGR_H
