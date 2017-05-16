#include "precomp.h"
#include <QApplication>
#include <QTextCodec>
#include <QDateTime>
#include "startpagemgr.h"
#include "mainwindow.h"
#include "jstylesheet.h"
#include "globalconfig.h"

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
    app.setApplicationName("Command Simulator");

#if 0
    // license
    if (!LicenseDialog::check()) {
        return -1;
    }
#endif
    app.setApplicationDisplayName(QStringLiteral("指令仿真软件"));
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

    // splas screen
    StartPageMgr *startPageMgr = new StartPageMgr();
#if 0
    if (startPageMgr->exec() != QDialog::Accepted) {
        return -1;
    }
#endif
    MainWindow *mainWindow = new MainWindow();
    mainWindow->setAttribute(Qt::WA_DeleteOnClose);
    bool result = startPageMgr->start(mainWindow);
    startPageMgr->deleteLater();
    if (!result) {
        mainWindow->deleteLater();
        return -1;
    }

    int ret = app.exec();

    return ret;
}
