#ifndef CCOMMANDPAGE_H
#define CCOMMANDPAGE_H

#include <QWidget>

class CCommandPage : public QWidget
{
    Q_OBJECT
public:
    explicit CCommandPage(QWidget *parent = 0);

    bool init();

signals:

public slots:

private:
};

#endif // CCOMMANDPAGE_H
