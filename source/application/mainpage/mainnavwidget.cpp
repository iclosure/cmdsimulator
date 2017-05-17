#include "precomp.h"
#include "mainnavwidget.h"
#include "ccommand/ccommandpage.h"
#include "scommand/scommandpage.h"
#include "config/configpage.h"

MainNavWidget::MainNavWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *vertLayoutMain = new QVBoxLayout(this);

    d_mainTab = new QTabWidget(this);
    vertLayoutMain->addWidget(d_mainTab);

    d_cCommandPage = new CCommandPage(this);
    d_sCommandPage = new SCommandPage(this);
    d_configPage = new ConfigPage(this);

    d_mainTab->addTab(d_cCommandPage, QStringLiteral("CÖ¸Áî"));
    d_mainTab->addTab(d_sCommandPage, QStringLiteral("SÖ¸Áî"));
    d_mainTab->addTab(d_configPage, QStringLiteral("ÅäÖÃ"));
}

bool MainNavWidget::init()
{
    bool result = true;

    //
    result = result && d_cCommandPage->init();

    //
    result = result && d_sCommandPage->init();

    //
    result = result && d_configPage->init();

    return result;
}

CCommandPage *MainNavWidget::cCommandPage() const
{
    return d_cCommandPage;
}

SCommandPage *MainNavWidget::sCommandPage() const
{
    return d_sCommandPage;
}

ConfigPage *MainNavWidget::configPage() const
{
    return d_configPage;
}
