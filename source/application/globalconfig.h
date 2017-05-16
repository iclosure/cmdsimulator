#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H

#include <QObject>

// - class GlobalConfig -

class GlobalConfigPrivate;

class GlobalConfig
{
public:

private:
    explicit GlobalConfig();
    ~GlobalConfig();

private:
    Q_DISABLE_COPY(GlobalConfig)
    J_DECLARE_PRIVATE(GlobalConfig)
    J_DECLARE_SINGLE_INSTANCE(GlobalConfig)
};

#endif // GLOBALCONFIG_H
