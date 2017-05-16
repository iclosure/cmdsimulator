#ifndef SECURITMGR_H
#define SECURITMGR_H

#include <QDialog>

class SecuritMgr : public QDialog
{
    Q_OBJECT
public:
    explicit SecuritMgr(QWidget *parent = 0);

protected:
    void done(int code);

private:
};

#endif // SECURITMGR_H
