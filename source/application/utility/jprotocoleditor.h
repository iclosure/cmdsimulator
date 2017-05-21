#ifndef JPROTOCOLEDITOR_H
#define JPROTOCOLEDITOR_H

#include <QWidget>

class JTableView;

class JProtocolEditor : public QWidget
{
    Q_OBJECT
public:
    explicit JProtocolEditor(QWidget *parent = 0);

    QByteArray buffer();
    const QByteArray &buffer() const;
    void setBuffer(const QByteArray &buffer);

signals:

public slots:
    void updateContents();
    void clear();

private:
    JTableView *d_tableView;
    QByteArray d_buffer;
};

#endif // JPROTOCOLEDITOR_H
