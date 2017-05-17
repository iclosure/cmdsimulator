#ifndef MAINVIEWWIDGET_H
#define MAINVIEWWIDGET_H

#include <QWidget>

class JProtocolEditor;
class JBufferView;

class MainViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainViewWidget(QWidget *parent = 0);

    bool init();

    JBufferView *bufferView() const;

signals:

public slots:

private:
    JProtocolEditor *d_protocolEditor;
    JBufferView *d_bufferView;
};

#endif // MAINVIEWWIDGET_H
