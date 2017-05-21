#include "precomp.h"
#include "mainviewwidget.h"
#include "jprotocoleditor.h"
#include "jbufferview.h"

MainViewWidget::MainViewWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *vertLayoutMain = new QVBoxLayout(this);
    vertLayoutMain->setContentsMargins(0, 0, 0, 0);
    vertLayoutMain->setSpacing(0);

    JSplitter *splitterMain = new JSplitter(Qt::Vertical, this);
    splitterMain->setHandleWidth(6);
    splitterMain->setScales(QList<double>() << 1 << 2);
    splitterMain->setHandleColor(QColor(20, 20, 20, 20));
    vertLayoutMain->addWidget(splitterMain);

    QWidget *widgetTop = new QWidget(this);
    splitterMain->addWidget(widgetTop);

    QVBoxLayout *vertLayoutTop = new QVBoxLayout(widgetTop);
    vertLayoutTop->setContentsMargins(0, 0, 0, 0);
    vertLayoutTop->setSpacing(2);

    d_protocolEditor = new JProtocolEditor(this);
    vertLayoutTop->addWidget(d_protocolEditor);

    QFormLayout *formLayoutButton = new QFormLayout();
    vertLayoutTop->addLayout(formLayoutButton);

    QHBoxLayout *horiLayoutButton = new QHBoxLayout();
    formLayoutButton->addRow(QStringLiteral("发送次数："), horiLayoutButton);

    QSpinBox *spinBoxCount = new QSpinBox(this);
    spinBoxCount->setMinimumWidth(200);
    horiLayoutButton->addWidget(spinBoxCount);

    QPushButton *buttonSend = new QPushButton(QStringLiteral("发送"), this);
    buttonSend->setMinimumWidth(80);
    horiLayoutButton->addWidget(buttonSend);

    horiLayoutButton->addStretch();

    d_bufferView = new JBufferView(this);
    splitterMain->addWidget(d_bufferView);
}

bool MainViewWidget::init()
{
    bool result = true;

    //TEST
    QByteArray data(1024, Qt::Uninitialized);
    for (int i = 0; i < 1024; ++i) {
        data[i] = qrand() % 255;
    }
    d_bufferView->setBuffer(data);

    return result;
}

JBufferView *MainViewWidget::bufferView() const
{
    return d_bufferView;
}
