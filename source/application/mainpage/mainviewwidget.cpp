#include "precomp.h"
#include "mainviewwidget.h"
#include "jprotocoleditor.h"
#include "jbufferview.h"

MainViewWidget::MainViewWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *vertLayoutMain = new QVBoxLayout(this);

    d_protocolEditor = new JProtocolEditor(this);
    vertLayoutMain->addWidget(d_protocolEditor);

    QHBoxLayout *horiLayoutButton = new QHBoxLayout();
    vertLayoutMain->addLayout(horiLayoutButton);

    QSpinBox *spinBoxCount = new QSpinBox(this);
    spinBoxCount->setMinimumWidth(200);
    horiLayoutButton->addWidget(spinBoxCount);

    QPushButton *buttonSend = new QPushButton(QStringLiteral("·¢ËÍ"), this);
    buttonSend->setMinimumWidth(80);
    horiLayoutButton->addWidget(buttonSend);

    horiLayoutButton->addStretch();

    d_bufferView = new JBufferView(this);
    vertLayoutMain->addWidget(d_bufferView);
}

bool MainViewWidget::init()
{
    bool result = true;

    //

    return result;
}

JBufferView *MainViewWidget::bufferView() const
{
    return d_bufferView;
}
