#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H

#include <QObject>
#include "jsqlcustomtype.h"

// - class GlobalConfig -

class GlobalConfigPrivate;

class GlobalConfig
{
public:
    QString databaseType() const;
    QString databaseConnectionName() const;
    QString accountUserName() const;
    QString accountPassword() const;
    QString baseLineFilePath() const;
    QString systemThemeStyle() const;

    QString userName() const;
    void setUserName(const QString &name);

    AccountUserType userType() const;
    void setUserType(AccountUserType type);

    unsigned int userSecurity() const;
    void setUserSecurity(unsigned int security);

    bool canReadDatabase() const;
    bool canWriteDatabase() const;
    bool canImportData() const;
    bool canExportData() const;
    bool canReadRuleFile() const;
    bool canWriteRuleFile() const;

private:
    explicit GlobalConfig();
    ~GlobalConfig();

private:
    Q_DISABLE_COPY(GlobalConfig)
    J_DECLARE_PRIVATE(GlobalConfig)
    J_DECLARE_SINGLE_INSTANCE(GlobalConfig)
};

#endif // GLOBALCONFIG_H
