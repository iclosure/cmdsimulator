#include "precomp.h"
#include "modelmgrwidget.h"

// - class JClickableLabel -

JClickableLabel::JClickableLabel(const QString &text, QWidget *parent) :
    QLabel(text, parent),
    q_mousePressed(false)
{

}

void JClickableLabel::mousePressEvent(QMouseEvent *)
{
    q_mousePressed = true;
}

void JClickableLabel::mouseReleaseEvent(QMouseEvent *)
{
    if (q_mousePressed) {
        Q_EMIT clicked();
    }
    q_mousePressed = false;
}

void JClickableLabel::leaveEvent(QEvent *)
{
    q_mousePressed = false;
}

// - class IconButton -

IconButton::IconButton(const QPixmap &pixmap, QWidget *parent) :
    QPushButton(parent),
    q_pixmap(pixmap)
{

}

QPixmap IconButton::pixmap() const
{
    return q_pixmap;
}

void IconButton::setPixmap(const QPixmap &pixmap)
{
    q_pixmap = pixmap;
    update();
}

void IconButton::paintEvent(QPaintEvent *)
{
    if (q_pixmap.isNull()) {
        return;
    }

    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.drawPixmap(rect(), q_pixmap);
    painter.restore();
}

// - class ModelMgrWidget -

ModelMgrWidget::ModelMgrWidget(QWidget *parent) :
    QWidget(parent)
{
    q_horiLayoutMain = new QHBoxLayout(this);
    q_horiLayoutMain->setContentsMargins(10, 0, 10, 0);
}

bool ModelMgrWidget::init()
{
    bool result = true;

    //
    setCurrentModel(QStringLiteral("数据管理"));

    return result;
}

void ModelMgrWidget::setCurrentModel(const QString &model, bool notify)
{
    if (!q_modelStack.isEmpty()) {
        if (q_modelStack.last() == model) {
            return;     // 模式未改变
        }
    }

    //
    q_modelStack.clear();
    if (model == QStringLiteral("数据管理")) {
        q_modelStack << QStringLiteral("数据管理");
    } else if (model == QStringLiteral("数据查询")) {
        q_modelStack << QStringLiteral("数据管理")
                     << QStringLiteral("数据查询");
    } else if (model == QStringLiteral("数据分析")) {
        q_modelStack << QStringLiteral("数据管理")
                     << QStringLiteral("数据查询")
                     << QStringLiteral("数据分析");
    }

    //
    updateModels();

    //
    if (notify) {
        Q_EMIT currentModelChanged(model);
    }
}

void ModelMgrWidget::updateModels()
{
    //
    QLayoutItem *child;
    while (child = q_horiLayoutMain->takeAt(0)) {
        QWidget *widget = child->widget();
        if (widget) {
            widget->setParent(0);
            widget->deleteLater();
        }
        delete child;
    }

    //
    IconButton *buttonHome = new IconButton(QPixmap(":/application/image/home-1.png"), this);
    buttonHome->setObjectName("buttonHome");
    buttonHome->setFixedSize(25, 25);
    buttonHome->setToolTip(QStringLiteral("主页"));
    q_horiLayoutMain->addWidget(buttonHome);

    QComboBox *comboBoxModel = new QComboBox(this);
    comboBoxModel->addItem(QStringLiteral("数据管理"));
    comboBoxModel->addItem(QStringLiteral("数据查询"));
    comboBoxModel->addItem(QStringLiteral("数据分析"));
    comboBoxModel->setMinimumWidth(QFontMetrics(comboBoxModel->font())
                                   .width(comboBoxModel->itemText(0)) + 50);
    comboBoxModel->setToolTip(QStringLiteral("只切换模式界面，不初始化界面数据"));
    q_horiLayoutMain->addWidget(comboBoxModel);

    //
    connect(buttonHome, &QPushButton::clicked, [=](bool){
        setCurrentModel(QStringLiteral("数据管理"));
    });

    //
    QStringListIterator citerModelStack(q_modelStack);
    while (citerModelStack.hasNext()) {
        const QString &model = citerModelStack.next();
        //

        JClickableLabel *labelName = new JClickableLabel(model, this);
        labelName->setObjectName("labelName");
        labelName->setAlignment(Qt::AlignVCenter);
        labelName->setText(model);
        IconButton *buttonArrow = new IconButton(QPixmap(":/application/image/arrow-1.png"), this);
        buttonArrow->setObjectName("buttonArrow");
        buttonArrow->setFixedSize(20, 25);
        q_horiLayoutMain->addWidget(labelName);
        q_horiLayoutMain->addWidget(buttonArrow);

        //
        connect(labelName, &JClickableLabel::clicked, [=](){
            setCurrentModel(labelName->text());
        });
    }

    //
    comboBoxModel->setCurrentText(q_modelStack.last());

    //
    connect(comboBoxModel, &QComboBox::currentTextChanged, [=](const QString &text){
        setCurrentModel(text, false);
        Q_EMIT currentIndexChanged(text);
    });
}
