#include "precomp.h"
#include "jbufferview.h"

JBufferView::JBufferView(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *vertLayoutMain = new QVBoxLayout(this);
    vertLayoutMain->setContentsMargins(0, 0, 0, 0);
    vertLayoutMain->setSpacing(0);

    QHBoxLayout *horiLayoutTop = new QHBoxLayout();
    vertLayoutMain->addLayout(horiLayoutTop);

    horiLayoutTop->addStretch();

    QFormLayout *formLayoutColumns = new QFormLayout();
    horiLayoutTop->addLayout(formLayoutColumns);

    d_spinBoxColumns = new QSpinBox(this);
    d_spinBoxColumns->setMinimumWidth(200);
    d_spinBoxColumns->setRange(1, 100);
    d_spinBoxColumns->setValue(32);
    formLayoutColumns->addRow(QStringLiteral("ÁÐÊý£º"), d_spinBoxColumns);

    d_tableView = new JTableView(this);
    d_tableView->setFont(QFont("Consolas", 11));
    d_tableView->setEditTriggers(QTableView::NoEditTriggers);
    d_tableView->setSelectionMode(QTableView::NoSelection);
    d_tableView->setSortingEnabled(false);
    d_tableView->setTextElideMode(Qt::ElideMiddle);
    d_tableView->setAlternatingRowColors(false);
    d_tableView->setFocusPolicy(Qt::NoFocus);
    d_tableView->setFrameShape(QFrame::NoFrame);
    d_tableView->setWordWrap(false);
    d_tableView->horizontalHeader()->setStretchLastSection(false);
    d_tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    d_tableView->horizontalHeader()->setSectionsMovable(false);
    d_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    d_tableView->horizontalHeader()->setDefaultSectionSize(22);
    d_tableView->horizontalHeader()->setFixedHeight(22);
    d_tableView->verticalHeader()->setCascadingSectionResizes(false);
    d_tableView->verticalHeader()->setDefaultAlignment(Qt::AlignRight | Qt::AlignVCenter);
    d_tableView->verticalHeader()->setSectionsMovable(false);
    d_tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    d_tableView->verticalHeader()->setDefaultSectionSize(20);
    d_tableView->verticalHeader()->setFixedWidth(40);
    vertLayoutMain->addWidget(d_tableView);

    connect(d_spinBoxColumns, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, [=](){
        updateContents();
    });
}

void JBufferView::setBuffer(char *buffer, int size)
{
    d_buffer = QByteArray(buffer, size);
    updateContents();
}

void JBufferView::setBuffer(const QByteArray &buffer)
{
    d_buffer = buffer;
    updateContents();
}

void JBufferView::updateContents()
{
    //
    d_tableView->clear();

    //
    if (d_buffer.isEmpty()) {
        return;
    }

    int columnCount = d_spinBoxColumns->value();

    // horizontal header
    d_tableView->setColumnCount(columnCount);
    QStringList labels;
    for (int i = 0; i < columnCount; ++i) {
        labels.append(QString("%1").arg(i, 2, 10, QChar(' ')));
    }
    d_tableView->setHorizontalHeaderLabels(labels);

    // rows
    d_tableView->setRowCount(qCeil((qreal)d_buffer.size() / columnCount));
    for (int i = 0; i < d_buffer.count(); ++i) {
        char data = d_buffer.at(i);
        int row = i / columnCount, column = i % columnCount;
        d_tableView->setItemData(row, column, Qt::AlignCenter, Qt::TextAlignmentRole);
        d_tableView->setItemData(row, column,
                                 QString("%1").arg((uchar)data, 2, 16, QChar('0')).toUpper());
    }
}

void JBufferView::clear()
{
    d_buffer.clear();
    updateContents();
}
