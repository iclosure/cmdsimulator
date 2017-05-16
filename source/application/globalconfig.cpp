#include "precomp.h"
#include "globalconfig.h"

// - class GlobalConfigPrivate -

class GlobalConfigPrivate
{
public:
    GlobalConfigPrivate(GlobalConfig *parent)
        : q_ptr(parent)
        , accountUserType(AccountUserInvalid)
        , userSecurity(0)
    {

    }

    void init();

private:
    J_DECLARE_PUBLIC(GlobalConfig)
    struct {
        QString type;
        QString connectionName;
        QString userName;
        QString password;
    } database;
    struct {
        QString filePath;
    } baseLine;
    struct {
        QString style;
    } systemTheme;

    //
    QString accountUserName;
    AccountUserType accountUserType;
    unsigned int userSecurity;
};

void GlobalConfigPrivate::init()
{
    //
    QString filePath(QApplication::applicationDirPath()
                     .append("/../config/config.xml"));
    QFile file(filePath);
    if (!file.exists() || !file.open(QFile::ReadOnly)) {
        qCritical() << QStringLiteral("%1 open failure!").arg(filePath);
        return;
    }

    //
    QDomDocument document;
    QString errorMsg;
    int errorLine;
    int errorColumn;
    if (!document.setContent(&file, &errorMsg, &errorLine, &errorColumn)) {
        qCritical() << QStringLiteral("Parse xml data"),
                QStringLiteral("parse data failure! \n\nerrorMsg: %1\nerrorPosition: (%2, %3))")
                .arg(errorMsg).arg(errorLine).arg(errorColumn);
        return;
    }

    //
    if (document.isNull()) {
        return;
    }

    //
    QDomElement emRoot = document.firstChildElement("config");
    if (emRoot.isNull()) {
        return;
    }

    // database
    QDomElement emDatabase = emRoot.firstChildElement("database");
    if (!emDatabase.isNull()) {
        // type
        if (emDatabase.hasAttribute("type")) {
            database.type = emDatabase.attribute("type");
        }
        // connectionName
        if (emDatabase.hasAttribute("connectionName")) {
            database.connectionName = emDatabase.attribute("connectionName");
        }
        // account
        QDomElement emAccount = emDatabase.firstChildElement("account");
        if (!emAccount.isNull()) {
            // userName
            if (emAccount.hasAttribute("userName")) {
                database.userName = emAccount.attribute("userName");
            }
            // password
            if (emAccount.hasAttribute("password")) {
                database.password = emAccount.attribute("password");
            }
        }
    }
    // baseLine
    QDomElement emBaseLine = emRoot.firstChildElement("baseLine");
    if (!emBaseLine.isNull()) {
        // path
        QDomElement emPath = emBaseLine.firstChildElement("path");
        if (!emPath.isNull()) {
            baseLine.filePath = emPath.text().trimmed();
            baseLine.filePath = QFileInfo(baseLine.filePath).absoluteFilePath();
        }
    }
    // system
    QDomElement emSystem = emRoot.firstChildElement("system");
    if (!emSystem.isNull()) {
        // theme
        QDomElement emTheme = emSystem.firstChildElement("theme");
        if (!emTheme.isNull()) {
            // colorTheme
            QDomElement emColorTheme = emTheme.firstChildElement("colorTheme");
            if (!emColorTheme.isNull()) {
                // style
                if (emColorTheme.hasAttribute("style")) {
                    systemTheme.style = emColorTheme.attribute("style");
                }
            }
        }
    }
}

// - class GlobalConfig -

J_IMPLEMENT_SINGLE_INSTANCE(GlobalConfig)

QString GlobalConfig::databaseType() const
{
    Q_D(const GlobalConfig);
    return d->database.type;
}

QString GlobalConfig::databaseConnectionName() const
{
    Q_D(const GlobalConfig);
    return d->database.connectionName;
}

QString GlobalConfig::accountUserName() const
{
    Q_D(const GlobalConfig);
    return d->database.userName;
}

QString GlobalConfig::accountPassword() const
{
    Q_D(const GlobalConfig);
    return d->database.password;
}

QString GlobalConfig::baseLineFilePath() const
{
    Q_D(const GlobalConfig);
    return d->baseLine.filePath;
}

QString GlobalConfig::systemThemeStyle() const
{
    Q_D(const GlobalConfig);
    return d->systemTheme.style;
}

QString GlobalConfig::userName() const
{
    Q_D(const GlobalConfig);
    return d->accountUserName;
}

void GlobalConfig::setUserName(const QString &name)
{
    Q_D(GlobalConfig);
    d->accountUserName = name;
}

AccountUserType GlobalConfig::userType() const
{
    Q_D(const GlobalConfig);
    return d->accountUserType;
}

void GlobalConfig::setUserType(AccountUserType type)
{
    Q_D(GlobalConfig);
    d->accountUserType = type;
}

unsigned int GlobalConfig::userSecurity() const
{
    Q_D(const GlobalConfig);
    return d->userSecurity;
}

void GlobalConfig::setUserSecurity(unsigned int security)
{
    Q_D(GlobalConfig);
    d->userSecurity = security;
}

bool GlobalConfig::canReadDatabase() const
{
    Q_D(const GlobalConfig);
    return (d->userSecurity & CanReadDatabase);
}

bool GlobalConfig::canWriteDatabase() const
{
    Q_D(const GlobalConfig);
    return (d->userSecurity & CanWriteDatabase);
}

bool GlobalConfig::canImportData() const
{
    Q_D(const GlobalConfig);
    return (d->userSecurity & CanImportData);
}

bool GlobalConfig::canExportData() const
{
    Q_D(const GlobalConfig);
    return (d->userSecurity & CanExportData);
}

bool GlobalConfig::canReadRuleFile() const
{
    Q_D(const GlobalConfig);
    return (d->userSecurity & CanReadRuleFile);
}

bool GlobalConfig::canWriteRuleFile() const
{
    Q_D(const GlobalConfig);
    return (d->userSecurity & CanWriteRuleFile);
}

GlobalConfig::GlobalConfig() :
    d_ptr(new GlobalConfigPrivate(this))
{
    Q_D(GlobalConfig);
    d->init();
}

GlobalConfig::~GlobalConfig()
{
    Q_D(GlobalConfig);
    delete d;
}
