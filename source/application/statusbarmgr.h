#ifndef STATUSBARMGR_H
#define STATUSBARMGR_H

#include <QStatusBar>

// - class StatusBarMgr -

class QLabel;
class QProgressBar;

class StatusBarMgr : public QStatusBar
{
    Q_OBJECT
public:
    explicit StatusBarMgr(QWidget *parent = 0);

    bool init();

Q_SIGNALS:

public Q_SLOTS:
    void setMessage(const QString &text);

private:
    QLabel *q_labelInfo;
    QProgressBar *q_progressBar;
};

#endif // STATUSBARMGR_H
