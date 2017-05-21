#ifndef JBUFFERVIEW_H
#define JBUFFERVIEW_H

#include <QWidget>

class QSpinBox;
class JTableView;

class JBufferView : public QWidget
{
    Q_OBJECT
public:
    explicit JBufferView(QWidget *parent = 0);

    void setBuffer(char *buffer, int size);
    void setBuffer(const QByteArray &buffer);

signals:

public slots:
    void updateContents();
    void clear();

private:
    QByteArray d_buffer;
    QSpinBox *d_spinBoxColumns;
    JTableView *d_tableView;
};

#endif // JBUFFERVIEW_H
