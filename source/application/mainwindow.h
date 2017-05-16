#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class CentralWidget;
class StatusBarMgr;
class JBusyIndicator;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Q_INVOKABLE bool init();

    Q_INVOKABLE bool isBusyIndicatorVisible() const;
    Q_INVOKABLE void setBusyIndicatorVisible(bool visible);

    Q_INVOKABLE bool isMainWindowEnabled() const;
    Q_INVOKABLE void setMainWindowEnabled(bool enabled);

Q_SIGNALS:

public Q_SLOTS:

protected:
    void closeEvent(QCloseEvent *event);

private:
    CentralWidget *q_centralWidget;
    StatusBarMgr *q_statusBarMgr;
    JBusyIndicator *q_busyIndicator;
};

#endif // MAINWINDOW_H
