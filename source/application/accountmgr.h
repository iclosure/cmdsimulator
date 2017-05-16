#ifndef ACCOUNTMGR_H
#define ACCOUNTMGR_H

#include <QDialog>
#include "jsqlcustomtype.h"

class AccountMgr : public QDialog
{
public:
    explicit AccountMgr(QWidget *parent = 0);

    static QString stringFromType(AccountUserType type);

protected:
    void done(int code);

private:
};

#endif // ACCOUNTMGR_H
