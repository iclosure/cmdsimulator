#include "precomp.h"
#include "globalconfig.h"

// - class GlobalConfigPrivate -

class GlobalConfigPrivate
{
public:
    GlobalConfigPrivate(GlobalConfig *parent)
        : q_ptr(parent)
    {

    }

    void init();

private:
    J_DECLARE_PUBLIC(GlobalConfig)
};

void GlobalConfigPrivate::init()
{
}

// - class GlobalConfig -

J_IMPLEMENT_SINGLE_INSTANCE(GlobalConfig)

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
