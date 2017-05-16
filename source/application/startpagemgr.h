#ifndef STARTPAGEMGR_H
#define STARTPAGEMGR_H

#include <QDialog>

// - class StartPageMgr -

class StartPageMgr : public QDialog
{
    Q_OBJECT
public:
    explicit StartPageMgr(QWidget *parent = 0);

    bool start(QWidget *widget);

Q_SIGNALS:

public Q_SLOTS:

private:
    QWidget *q_widget;
};

#endif // STARTPAGEMGR_H
