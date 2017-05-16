#ifndef JLISTCOORDITEM_H
#define JLISTCOORDITEM_H

#include <QWidget>

// - class JListCoordItem -

class QTableWidgetItem;
class QLabel;
class QLineEdit;

class JListCoordItem : public QWidget
{
    Q_OBJECT
public:
    explicit JListCoordItem(int index,
                            const QString &nameX, const QString &nameY,
                            QWidget *parent = 0);

    QString suffix() const;
    void setSuffix(const QString &suffix);

    QString symbolX() const;
    QString symbolY() const;

Q_SIGNALS:
    void focus(int index);
    void remove(int index);

private:
    int q_index;
    QLabel *q_labelSuffix;
    QLineEdit *q_lineEditAxisX;
    QLineEdit *q_lineEditAxisY;
};

#endif // JLISTCOORDITEM_H
