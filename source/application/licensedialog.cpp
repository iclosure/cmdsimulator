#include "precomp.h"
#include "licensedialog.h"
#include "encrypt_inc.h"

using namespace encrypt;

LicenseDialog::LicenseDialog(QWidget *parent)
    : QDialog(parent)
{
    QVBoxLayout *vertLayoutMain = new QVBoxLayout(this);
    vertLayoutMain->setContentsMargins(0, 0, 0, 0);

    QLabel *labelLogo = new QLabel(this);
    labelLogo->setScaledContents(true);
    labelLogo->setPixmap(QPixmap(":/application/image/bk-license.bmp"));
    vertLayoutMain->addWidget(labelLogo);

    QFormLayout *formLayout = new QFormLayout();
    formLayout->setContentsMargins(8, 0, 8, 0);
    vertLayoutMain->addLayout(formLayout);

    QLineEdit *lineEdit = new QLineEdit(this);
    formLayout->addRow(QStringLiteral("输入注册码："), lineEdit);

    QHBoxLayout *horiLayoutBottom = new QHBoxLayout();
    horiLayoutBottom->setContentsMargins(8, 8, 8, 8);
    vertLayoutMain->addLayout(horiLayoutBottom);

    QPushButton *buttonActive = new QPushButton(QStringLiteral("激活"));
    QPushButton *buttonClose = new QPushButton(QStringLiteral("关闭"));

    horiLayoutBottom->addStretch();
    horiLayoutBottom->addWidget(buttonActive);
    horiLayoutBottom->addWidget(buttonClose);

    //
    connect(buttonActive, &QPushButton::clicked, [=](){
        QString text = lineEdit->text();
        if (text.isEmpty()) {
            QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("注册码不能为空！"));
            return;
        }

        CSystemInfo systemInfo;

        CHAR strBIOSInfo[1024] = {0};
        systemInfo.m_biosInfo.GetBiosID(strBIOSInfo);

        //CHAR strCPUID[1024] = {0};
        //systemInfo.m_CPUID.GetFirstCPUID(strCPUID);

        CHAR strDiskSerial[1024] = {0};
        systemInfo.m_diskSerial.GetFirstDiskSerial(strDiskSerial);

        CHAR strMac[1024] = {0};
        systemInfo.m_ethernetMac.GetFirstMac(strMac);

        // Encrypt data sources: BIOS Info , Disk Serial Number and Network Physical Address
        std::string input = std::string(strBIOSInfo) + std::string(strDiskSerial) + std::string(strMac);

        MD5 md5;
        md5.update(input);

        std::string output = md5.toString();

        // 取偶序列
        std::string output1;
        for (size_t i = 0; i < output.size() / 2; i++) {
            output1.append(1, output[i * 2 + 1]);
        }

        for (size_t i = 4; i < output1.size(); i+=5) {
            output1.insert(i, 1, '-');
        }

        if (text == QString::fromStdString(output1)) {
            this->writeSettingsSerialNumber(text);
            QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("注册成功！"));
            this->accept();
        } else {
            QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("注册码无效！"));
        }
    });
    connect(buttonClose, &QPushButton::clicked, [=](){
        this->reject();
    });
}

// 软件加密处理
bool LicenseDialog::check()
{
    // 运行软件时，先检测软件是否已经注册成功？
    QString serial = readSettingsSerialNumber();
    if (!serial.isEmpty()) {
        // 检测是否已经注册过且注册是否成功
        if (license(serial)) {
            return true;
        }
    }

    LicenseDialog licenseDialog;
    licenseDialog.resize(400, 200);
    if (licenseDialog.exec() == QDialog::Accepted) {
        // 再次检测是否已经注册过且注册是否成功
        serial = readSettingsSerialNumber();	// 读取序列号
        if (!serial.isEmpty()) {
            if (license(serial)) {
                return true;
            }
        }
    }

    return false;
}

bool LicenseDialog::license(const QString &serial)
{
    if (serial.isEmpty()) {
        return false;
    }

    CSystemInfo systemInfo;

    CHAR strBIOSInfo[1024] = {0};
    systemInfo.m_biosInfo.GetBiosID(strBIOSInfo);

    //CHAR strCPUID[1024] = {0};
    //systemInfo.m_CPUID.GetFirstCPUID(strCPUID);

    CHAR strDiskSerial[1024] = {0};
    systemInfo.m_diskSerial.GetFirstDiskSerial(strDiskSerial);

    CHAR strMac[1024] = {0};
    systemInfo.m_ethernetMac.GetFirstMac(strMac);

    // Encrypt BIOS INFO + Disk Serial Number + Mac Address
    std::string input = std::string(strBIOSInfo) + std::string(strDiskSerial) + std::string(strMac);

    MD5 md5;
    md5.update(input);

    std::string output = md5.toString();

    //
    std::string output1;
    for (size_t i = 0; i < output.size() / 2; i++) {
        output1.append(1, output[i * 2 + 1]);
    }

    for (size_t i = 4; i < output1.size(); i+=5) {
        output1.insert(i, 1, '-');
    }

    if (serial == QString::fromStdString(output1)) {
        return true;
    } else {
        return false;
    }
}

void LicenseDialog::writeSettingsSerialNumber(const QString &text)
{
    QSettings settings;
    settings.beginGroup("Settings/License");
    settings.setValue("Serial Number", text);
    settings.endGroup();
}

QString LicenseDialog::readSettingsSerialNumber()
{
    QSettings settings;
    settings.beginGroup("Settings/License");
    QString text = settings.value("Serial Number").toString();
    settings.endGroup();
    return text;
}
