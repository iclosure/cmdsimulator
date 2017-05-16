#ifndef MODELMGRWIDGET_H
#define MODELMGRWIDGET_H

#include <QPushButton>
#include <QLabel>

// - class JClickableLabel -

class JClickableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit JClickableLabel(const QString &text, QWidget *parent = 0);

Q_SIGNALS:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void leaveEvent(QEvent *);

private:
    bool q_mousePressed;
};

// - class IconButton -

class IconButton : public QPushButton
{
    Q_OBJECT
public:
    explicit IconButton(const QPixmap &pixmap, QWidget *parent = 0);

    QPixmap pixmap() const;
    void setPixmap(const QPixmap &pixmap);

protected:
    void paintEvent(QPaintEvent *);

private:
    QPixmap q_pixmap;
};

// - class ModelMgrWidget -

class QHBoxLayout;

class ModelMgrWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ModelMgrWidget(QWidget *parent = 0);

    bool init();

    void setCurrentModel(const QString &model, bool notify = true);

Q_SIGNALS:
    void currentModelChanged(const QString &model);
    void currentIndexChanged(const QString &model);

public Q_SLOTS:
    void updateModels();

private:
    QHBoxLayout *q_horiLayoutMain;
    QStringList q_modelStack;
};

#endif // MODELMGRWIDGET_H
