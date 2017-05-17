#ifndef CONFIGPAGE_H
#define CONFIGPAGE_H

#include <QWidget>

class ConfigPage : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigPage(QWidget *parent = 0);

    bool init();

signals:

public slots:

private:
};

#endif // CONFIGPAGE_H
