#include "precomp.h"
#include <QApplication>
#include <QTextCodec>
#include <QDateTime>

int main(int argc, char **argv)
{
#if QT_VERSION >= 0x050700
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    // textcodec
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
#if QT_VERSION < 0x050000
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    QTextCodec::setCodecForCStrins(QTextCodec::codecForLocale());
#endif

    QApplication app(argc, argv);
    app.setFont(QFont("Microsoft Yahei", 10));
    app.setOrganizationName("Smartsoft");
    app.setApplicationName("Data Analyse");

#if 0
    // license
    if (!LicenseDialog::check()) {
        return -1;
    }
#endif
    app.setApplicationDisplayName(QStringLiteral("数据分析软件"));
    app.setApplicationVersion("1.0.0");
    app.setWindowIcon(QIcon(QApplication::applicationDirPath()
                            .append("/images/logo.png")));
    // install log-handler
#ifdef APP_LOG
    JLogManager::installMessageHandler();
    JLogManager::setLogType(JLogManager::LogFile);
    //JLogManager::setLogType(JLogManager::LogConsole);
#endif

    JWidgetsCore::instance()->init();

    qsrand((uint)QDateTime::currentDateTime().toMSecsSinceEpoch());

    // 记载系统语言翻译器
    if (!JWidgetsCore::instance()->loadSystemLang()) {
        qWarning() << "loading system language translator failure!";
    }

    // stylesheet
    app.setStyleSheet(JStyleSheet::styleSheet("default"));

    //TEST
#if 1

#if 0
    // 生成数据记录文件
    QString filePath = QApplication::applicationDirPath()
            .append("/../data/examples/%1%2-%3-%4_%5.txt")
            .arg("X")
            .arg("1002")
            .arg(QDate::currentDate().toString("yyyyMMdd"))
            .arg(2)
            .arg(QStringLiteral("表头 2"));

    QStringList symbols;
    symbols << "Tim" << "Width" << "Mach" << "H" << "Latitude" << "Longitude" << "Address";
    for (int i = 0; i < 100; ++i) {
        symbols << QString("SYMBOL_VAR_ITEM_%1").arg(i);
    }

    JFlyRecordGenerater::instance()->generateFlyRecordFile(
                filePath,
                symbols,
                1000000);
#elif 0
    // 生成数据记录文件
    QString filePath = QApplication::applicationDirPath()
            .append("/../data/examples/%1%2-%3-%4_%5.txt")
            .arg("Y")
            .arg("1005")
            .arg(QDate::currentDate().toString("yyyyMMdd"))
            .arg(4)
            .arg(QStringLiteral("表头 3"));

    JFlyRecordGenerater::instance()->generateFlyRecordFile(
                filePath,
                QStringList()
                << "T" << "Mach" << "Width" << "Height" << "Latitude" << "Longitude" << "Address" << "Var1" << "Var3" << "Var4",
                500);
#else
    QStringList filePaths;
    filePaths << QString(QApplication::applicationDirPath()
                         .append(QStringLiteral("/data/X1002-20160303-3_表头 2.txt")));
    filePaths << QString(QApplication::applicationDirPath()
                         .append(QStringLiteral("/data/X1002-20160303-3_表头 3.txt")));
    filePaths << QString(QApplication::applicationDirPath()
                         .append(QStringLiteral("/data/Y1005-20160306-4_表头 3.txt")));
#endif

#if 1
    //
    JRuleDict::instance()->loadRuleFile(QApplication::applicationDirPath()
                                        .append("/../config/rule.xml"));
    //JRuleDict::instance()->addSymbol(QStringLiteral("时间"), QStringList() << "variable1");
    //
    JSqlTableMgr::instance()->loadConfig(QApplication::applicationDirPath()
                                         .append("/../config/config.xml"));

    //JFlyRecord::instance()->parseFile(filePaths);

#endif

#endif

#if 1
    JMyExcel::instance();
#endif

    // splas screen
    StartPageMgr *startPageMgr = new StartPageMgr();
    if (startPageMgr->exec() != QDialog::Accepted) {
        return -1;
    }

    QString msg = QStringLiteral("登录软件：%1（%2）")
            .arg(AccountMgr::stringFromType(GlobalConfig::instance()->userType()))
            .arg(GlobalConfig::instance()->userName());
#if QT_VERSION >= 0x050600
    qInfo() << msg;
#else
    qWarning() << msg;
#endif

    //
    switch (GlobalConfig::instance()->userType()) {
    case AccountUserAdmin:
    {
        AccountMgr *accountMgr = new AccountMgr();
        accountMgr->setAttribute(Qt::WA_DeleteOnClose);
        accountMgr->resize(800, 480);
        accountMgr->show();
        break;
    }
    case AccountUserSecurity:
    {
        SecuritMgr *securityMgr = new SecuritMgr();
        securityMgr->setAttribute(Qt::WA_DeleteOnClose);
        securityMgr->resize(800, 480);
        securityMgr->show();
        break;
    }
    case AccountUserAudit:
    {
        LogViewMgr *logViewMgr = new LogViewMgr();
        logViewMgr->setAttribute(Qt::WA_DeleteOnClose);
        logViewMgr->resize(800, 480);
        logViewMgr->showMaximized();
        break;
    }
    case AccountUserNormal:
    {
        MainWindow *mainWindow = new MainWindow();
        mainWindow->setAttribute(Qt::WA_DeleteOnClose);
        bool result = startPageMgr->start(mainWindow);
        startPageMgr->deleteLater();
        if (!result) {
            mainWindow->deleteLater();
            return -1;
        }
        break;
    }
    default:
        break;
    }

    int ret = app.exec();

    //
    msg = QStringLiteral("退出软件：%1（%2）")
            .arg(AccountMgr::stringFromType(GlobalConfig::instance()->userType()))
            .arg(GlobalConfig::instance()->userName());
#if QT_VERSION >= 0x050600
    qInfo() << msg;
#else
    qWarning() << msg;
#endif

    return ret;
}
