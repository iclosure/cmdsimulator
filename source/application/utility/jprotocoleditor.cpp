#include "precomp.h"
#include "jprotocoleditor.h"

JProtocolEditor::JProtocolEditor(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *vertLayoutMain = new QVBoxLayout(this);
    vertLayoutMain->setContentsMargins(0, 0, 0, 0);
    vertLayoutMain->setSpacing(0);

    //
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
    d_tableView->horizontalHeader()->hide();
    d_tableView->horizontalHeader()->setStretchLastSection(false);
    d_tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    d_tableView->horizontalHeader()->setSectionsMovable(false);
    d_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    d_tableView->horizontalHeader()->setDefaultSectionSize(22);
    d_tableView->horizontalHeader()->setFixedHeight(22);
    d_tableView->verticalHeader()->hide();
    d_tableView->verticalHeader()->setCascadingSectionResizes(false);
    d_tableView->verticalHeader()->setDefaultAlignment(Qt::AlignRight | Qt::AlignVCenter);
    d_tableView->verticalHeader()->setSectionsMovable(false);
    d_tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    d_tableView->verticalHeader()->setDefaultSectionSize(20);
    d_tableView->verticalHeader()->setFixedWidth(40);
    vertLayoutMain->addWidget(d_tableView);
}

QByteArray JProtocolEditor::buffer()
{
    return d_buffer;
}

const QByteArray &JProtocolEditor::buffer() const
{
    return d_buffer;
}

void JProtocolEditor::setBuffer(const QByteArray &buffer)
{
    d_buffer = buffer;
    updateContents();
}

void JProtocolEditor::updateContents()
{
    //
}

void JProtocolEditor::clear()
{
    d_tableView->clear();
}
