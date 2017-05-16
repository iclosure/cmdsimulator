#include "precomp.h"
#include "jlistcoorditem.h"

// - class JListCoordItem -

JListCoordItem::JListCoordItem(int index,
                               const QString &nameX, const QString &nameY,
                               QWidget *parent) :
    QWidget(parent),
    q_index(index)
{
    setObjectName("JListCoordItem");

    QVBoxLayout *vertLayoutMain = new QVBoxLayout(this);
    vertLayoutMain->setContentsMargins(2, 2, 2, 2);
    vertLayoutMain->setSpacing(2);

    q_labelSuffix = new QLabel(QStringLiteral("<?>"), this);
    q_labelSuffix->setAlignment(Qt::AlignCenter);
    vertLayoutMain->addWidget(q_labelSuffix);

    QHBoxLayout *horiLayoutBottom = new QHBoxLayout();
    horiLayoutBottom->setContentsMargins(0, 0, 0, 2);
    vertLayoutMain->addLayout(horiLayoutBottom);

    QFormLayout *formLayoutCoord = new QFormLayout();
    horiLayoutBottom->addLayout(formLayoutCoord);

    q_lineEditAxisX = new QLineEdit(nameX, this);
    q_lineEditAxisX->setReadOnly(true);
    formLayoutCoord->addRow(QStringLiteral("横轴："), q_lineEditAxisX);

    q_lineEditAxisY = new QLineEdit(nameY, this);
    q_lineEditAxisY->setReadOnly(true);
    formLayoutCoord->addRow(QStringLiteral("纵轴："), q_lineEditAxisY);

    horiLayoutBottom->addSpacing(3);

    int buttonSize = formLayoutCoord->sizeHint().height() - 5;

    QPushButton *buttonDelete = new QPushButton(this);
    buttonDelete->setToolTip(QStringLiteral("删除此项"));
    buttonDelete->setIcon(QIcon(":/application/image/delete.png"));
    buttonDelete->setFixedSize(buttonSize, buttonSize);
    buttonDelete->setIconSize(buttonDelete->size());
    horiLayoutBottom->addWidget(buttonDelete);
    horiLayoutBottom->addSpacing(3);

    QPushButton *buttonFocus = new QPushButton(this);
    buttonFocus->setToolTip(QStringLiteral("定位到曲线图"));
    buttonFocus->setIcon(QIcon(":/application/image/focus.png"));
    buttonFocus->setFixedSize(buttonSize, buttonSize);
    buttonFocus->setIconSize(buttonFocus->size());
    horiLayoutBottom->addWidget(buttonFocus);
    horiLayoutBottom->addSpacing(3);

    connect(buttonFocus, &QPushButton::clicked, this, [=](){
        Q_EMIT this->focus(q_index);
    });
    connect(buttonDelete, &QPushButton::clicked, this, [=](){
        Q_EMIT this->remove(q_index);
    });
}

QString JListCoordItem::suffix() const
{
    return q_labelSuffix->text();
}

void JListCoordItem::setSuffix(const QString &suffix)
{
    q_labelSuffix->setText(suffix);
}

QString JListCoordItem::symbolX() const
{
    return q_lineEditAxisX->text().section(" (", 0, 0);
}

QString JListCoordItem::symbolY() const
{
    return q_lineEditAxisY->text().section(" (", 0, 0);
}
