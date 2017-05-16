#ifndef JSQLCUSTOMTYPE_H
#define JSQLCUSTOMTYPE_H

#include <QDate>
#include <QTime>
#include <QList>

/**
 * @brief The AccountUserType enum
 */
enum AccountUserType {
    AccountUserInvalid = -1,    // 无效
    AccountUserAdmin,           // 系统管理员
    AccountUserSecurity,        // 安全保密员
    AccountUserAudit,           // 安全审计员
    AccountUserNormal           // 软件用户
};

/**
 * @brief The UserSecurity enum
 */
enum UserSecurity {
    CanReadDatabase = 0x01,     // 可查看数据库
    CanWriteDatabase = 0x02,    // 可修改数据库
    CanImportData = 0x04,       // 可导入数据
    CanExportData = 0x08,       // 可导出数据
    CanWriteRuleFile = 0x10,    // 可修改规则文件
    CanReadRuleFile = 0x20      // 可查看规则文件
};

// 用户信息
struct JAccountInfo
{
    QString XH;                 // 序号
    QString userName;           // 用户名
    QString password;           // 密码
    AccountUserType userType;   // 用户类型（权限）
    unsigned int security;      // 用户权限
};

// 数据文件名称信息
struct JFileNameInfo
{
    QString JX;     // 机型
    QString JH;     // 机号
    QDate minRQ;    // 日期下限（上限无效时作为查询量）
    QDate maxRQ;    // 日期上限
    int JC;         // 架次

    JFileNameInfo()
    {
        JC = 0;
    }

    JFileNameInfo(const JFileNameInfo &other)
    {
        *this = other;
    }

    JFileNameInfo &operator =(const JFileNameInfo &other)
    {
        if (this == &other) {
            return *this;
        }

        JX = other.JX;
        JH = other.JH;
        minRQ = other.minRQ;
        maxRQ = other.maxRQ;
        JC = other.JC;

        return *this;
    }

    QString toString() const
    {
        return QString("%1-%2-%3-%4").arg(JX).arg(JH).arg(minRQ.toString("yyyyMMdd")).arg(JC);
    }
};

// 主表数据结构
struct stMainTableInfo
{
    QString JX;     // 机型
    QString JH;     // 机号
    QDate RQ;       // 日期
    int JC;         // 架次
    QString DYXHB;  // 对应信号表
    QString DYZB;   // 对应值表
    QString DYRDFW; // 对应热点范围表

    stMainTableInfo()
    {
        JC = 0;
    }

    stMainTableInfo(const stMainTableInfo &other)
    {
        *this = other;
    }

    stMainTableInfo &operator =(const stMainTableInfo &other)
    {
        if (this == &other) {
            return *this;
        }

        JX = other.JX;
        JH = other.JH;
        RQ = other.RQ;
        JC = other.JC;
        DYXHB = other.DYXHB;
        DYZB = other.DYZB;
        DYRDFW = other.DYRDFW;

        return *this;
    }
};

// 信号表数据结构
struct stSignalInfo
{
    QString YXHM;   // 原信号名
    QString BM;     // 别名
    QString SSJX;   // 所属机型
    QString SSJH;   // 所属机号
    QDate SSRQ;     // 所属日期
    int SSJC;       // 所属架次
    bool SXBZ;      // 筛选标志
    bool GJBZ;      // 关键标志
    bool XZBZ;      // 选择标志

    stSignalInfo()
    {
        SSJC = 0;
        SXBZ = false;
        GJBZ = false;
        XZBZ = false;
    }

    stSignalInfo(const QString &YXHM) : YXHM(YXHM)
    {
        SSJC = 0;
        SXBZ = false;
        GJBZ = false;
        XZBZ = false;
    }

    stSignalInfo(const stSignalInfo &other)
    {
        *this = other;
    }

    stSignalInfo &operator =(const stSignalInfo &other)
    {
        if (this == &other) {
            return *this;
        }

        YXHM = other.YXHM;
        BM = other.BM;
        SSJX = other.SSJX;
        SSJH = other.SSJH;
        SSRQ = other.SSRQ;
        SSJC = other.SSJC;
        SXBZ = other.SXBZ;
        GJBZ = other.GJBZ;
        XZBZ = other.XZBZ;

        return *this;
    }

    bool operator ==(const QString &YXHM)
    {
        return (this->YXHM == YXHM);
    }

    bool operator ==(const stSignalInfo &other)
    {
        return YXHM == other.YXHM;
    }
};

// 信号筛选数据结构
struct stSignalSiftInfo
{
    QString YXHM;   // 原信号名
    QString BM;     // 别名
    bool SXBZ;      // 筛选标志

    stSignalSiftInfo()
    {
        SXBZ = false;
    }

    stSignalSiftInfo(const QString &YXHM) : YXHM(YXHM)
    {
        SXBZ = false;
    }

    stSignalSiftInfo(const stSignalSiftInfo &other)
    {
        *this = other;
    }

    stSignalSiftInfo &operator =(const stSignalSiftInfo &other)
    {
        if (this == &other) {
            return *this;
        }

        YXHM = other.YXHM;
        BM = other.BM;
        SXBZ = other.SXBZ;

        return *this;
    }

    bool operator ==(const QString &YXHM)
    {
        return this->YXHM == YXHM;
    }

    bool operator ==(const stSignalSiftInfo &other)
    {
        return YXHM == other.YXHM;
    }
};

// 值表数据结构
struct stValueInfo
{
    QTime time;             // 时间
    QList<double> values;   // 值

    stValueInfo()
    {

    }

    stValueInfo(const stValueInfo &other)
    {
        *this = other;
    }

    stValueInfo &operator =(const stValueInfo &other)
    {
        if (this == &other) {
            return *this;
        }

        time = other.time;
        values = other.values;

        return *this;
    }
};

#endif // JSQLCUSTOMTYPE_H

