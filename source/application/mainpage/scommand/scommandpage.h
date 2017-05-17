#ifndef SCOMMANDPAGE_H
#define SCOMMANDPAGE_H

#include <QWidget>

class SCommandPage : public QWidget
{
    Q_OBJECT
public:
    explicit SCommandPage(QWidget *parent = 0);

    bool init();

signals:

public slots:
};

#endif // SCOMMANDPAGE_H
