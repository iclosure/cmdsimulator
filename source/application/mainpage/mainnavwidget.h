#ifndef MAINNAVWIDGET_H
#define MAINNAVWIDGET_H

#include <QWidget>

class QTabWidget;
class CCommandPage;
class SCommandPage;
class ConfigPage;

class MainNavWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainNavWidget(QWidget *parent = 0);

    bool init();
    CCommandPage *cCommandPage() const;
    SCommandPage *sCommandPage() const;
    ConfigPage *configPage() const;

signals:

public slots:

private:
    QTabWidget *d_mainTab;
    SCommandPage *d_sCommandPage;
    CCommandPage *d_cCommandPage;
    ConfigPage *d_configPage;
};

#endif // MAINNAVWIDGET_H
