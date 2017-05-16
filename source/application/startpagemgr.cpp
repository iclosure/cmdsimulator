#include "precomp.h"
#include "startpagemgr.h"
#include "globalconfig.h"

// - class StartPageMgr -

StartPageMgr::StartPageMgr(QWidget *parent) :
    QDialog(parent, Qt::FramelessWindowHint),
    q_widget(0)
{
    resize(650, 350);

    QVBoxLayout *vertLayoutMain = new QVBoxLayout(this);
    vertLayoutMain->addStretch();

    // logo
    QLabel *labelLogo = new QLabel(this);
    labelLogo->setObjectName("labelLogo");
    QPixmap pmLogo = QPixmap(QApplication::applicationDirPath().append("/../config/images/logo.png"))
                             .scaled(64, 64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    labelLogo->setFixedSize(pmLogo.size());
    labelLogo->setPixmap(pmLogo);
    vertLayoutMain->addWidget(labelLogo, 0, Qt::AlignHCenter);

    QGroupBox *groupBoxLogin = new QGroupBox(this);
    groupBoxLogin->setFixedWidth(350);
    vertLayoutMain->addWidget(groupBoxLogin, 0, Qt::AlignHCenter);
    QVBoxLayout *vertLayoutLogin = new QVBoxLayout(groupBoxLogin);

    // login information
    QFormLayout *formLayoutLogin = new QFormLayout();
    formLayoutLogin->setLabelAlignment(Qt::AlignRight);
    vertLayoutLogin->addLayout(formLayoutLogin);

    QComboBox *comboBoxUserName = new QComboBox(groupBoxLogin);
    formLayoutLogin->addRow(QStringLiteral("用户名："), comboBoxUserName);

    QLineEdit *lineEditPassword = new QLineEdit(groupBoxLogin);
    lineEditPassword->setEchoMode(QLineEdit::Password);
    formLayoutLogin->addRow(QStringLiteral("密  码："), lineEditPassword);

    QHBoxLayout *horiLayoutButtons = new QHBoxLayout();
    vertLayoutLogin->addLayout(horiLayoutButtons);

    QLabel *labelUserType = new QLabel(groupBoxLogin);
    horiLayoutButtons->addWidget(labelUserType);

    horiLayoutButtons->addStretch();

    QPushButton *buttonLogin = new QPushButton(QStringLiteral("登录"), groupBoxLogin);
    QPushButton *buttonExit = new QPushButton(QStringLiteral("取消"), groupBoxLogin);
    horiLayoutButtons->addWidget(buttonLogin);
    horiLayoutButtons->addWidget(buttonExit);

    // progress
    QProgressBar *progressBar = new QProgressBar(this);
    progressBar->setObjectName("progressBar");
    progressBar->setFixedWidth(width() * 0.8);
    progressBar->setValue(35);
    vertLayoutMain->addWidget(progressBar, 0, Qt::AlignHCenter);

    // message
    QLabel *labelMsg = new QLabel(this);
    labelMsg->setObjectName("labelMsg");
    labelMsg->setAlignment(Qt::AlignHCenter);
    labelMsg->setFixedWidth(width() * 0.8);
    vertLayoutMain->addWidget(labelMsg, 0, Qt::AlignHCenter);

    vertLayoutMain->addStretch();

    // initialize
    labelMsg->setText(QStringLiteral("正在初始化……"));

    // stylesheet
    setStyleSheet(JStyleSheet::styleSheet("startPage"));

    //
    progressBar->hide();
    labelMsg->hide();

    //
    connect(comboBoxUserName, &QComboBox::currentTextChanged, this, [=](const QString &text){
        //
        lineEditPassword->clear();
        //
        AccountUserType userType = JSqlTableMgr::instance()->userType(text);
        switch (userType) {
        default:
        case AccountUserInvalid:
            labelUserType->setText(QStringLiteral("<font size=3 color=#cdcdcd>*无效类型</font>"));
            break;
        case AccountUserAdmin:
            labelUserType->setText(QStringLiteral("<font size=3 color=#cdcdcd>*系统管理员</font>"));
            break;
        case AccountUserSecurity:
            labelUserType->setText(QStringLiteral("<font size=3 color=#cdcdcd>*安全保密员</font>"));
            break;
        case AccountUserAudit:
            labelUserType->setText(QStringLiteral("<font size=3 color=#cdcdcd>*安全审计员</font>"));
            break;
        case AccountUserNormal:
            labelUserType->setText(QStringLiteral("<font size=3 color=#cdcdcd>*软件用户</font>"));
            break;
        }
    });
    connect(buttonLogin, &QPushButton::clicked, this, [=](){
        // check
        const QString userName = comboBoxUserName->currentText().trimmed();
        if (userName.isEmpty()) {
            QMessageBox::warning(this, QStringLiteral("警告"),
                                 QStringLiteral("用户名不能为空！"));
            comboBoxUserName->setFocus();
            return;
        }
        //
        if (!JSqlTableMgr::instance()->existsUser(userName)) {
            QMessageBox::critical(this, QStringLiteral("错误"),
                                 QStringLiteral("用户不存在！"));
            comboBoxUserName->setFocus();
            return;
        }
        //
        const QString password = lineEditPassword->text().trimmed();
        if (password.isEmpty()) {
            QMessageBox::warning(this, QStringLiteral("警告"),
                                 QStringLiteral("密码不能为空！"));
            lineEditPassword->setFocus();
            return;
        }
        //
        if (!JSqlTableMgr::instance()->checkUserName(userName, password)) {
            QMessageBox::critical(this, QStringLiteral("错误"),
                                 QStringLiteral("密码不正确！"));
            lineEditPassword->setFocus();
            return;
        }
        //
        GlobalConfig::instance()->setUserName(userName);
        GlobalConfig::instance()->setUserType(JSqlTableMgr::instance()->userType(userName));
        GlobalConfig::instance()->setUserSecurity(JSqlTableMgr::instance()->userSecurity(userName));
        //
        groupBoxLogin->hide();
        labelMsg->show();
        // accept
        this->accept();
    });
    connect(buttonExit, &QPushButton::clicked, this, [=](){
        this->reject();
    });

    //
    comboBoxUserName->addItems(JSqlTableMgr::instance()->allUserName());

    //
    comboBoxUserName->setCurrentText(GlobalConfig::instance()->accountUserName());
}

bool StartPageMgr::start(QWidget *widget)
{
    q_widget = widget;

    // start tasks

    // 初始化核心功能
    bool result = false;
    QMetaObject::invokeMethod(widget, "init", Q_RETURN_ARG(bool, result));
    if (!result) {
        //return false;   // 初始化失败！
    }

    // fileparse - aliasdict

    //
    QApplication::processEvents();

    //
    q_widget->showMaximized();

    return true;
}
