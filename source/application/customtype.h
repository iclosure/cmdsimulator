#ifndef CUSTOMTYPE_H
#define CUSTOMTYPE_H

#include <qglobal.h>
#include <qmetatype.h>
#include "jsqlcustomtype.h"

//
struct JTableSelInfo
{
    JFileNameInfo baseInfo;
    QString symbol; // 信号量

    JTableSelInfo()
    {

    }

    JTableSelInfo(const JTableSelInfo &other)
    {
        *this = other;
    }

    JTableSelInfo &operator =(const JTableSelInfo &other)
    {
        if (this == &other) {
            return *this;
        }

        baseInfo = other.baseInfo;
        symbol = other.symbol;

        return *this;
    }
};

//
Q_DECLARE_METATYPE(JTableSelInfo)

#endif // CUSTOMTYPE_H
