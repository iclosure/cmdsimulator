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
    formLayout->addRow(QStringLiteral("����ע���룺"), lineEdit);

    QHBoxLayout *horiLayoutBottom = new QHBoxLayout();
    horiLayoutBottom->setContentsMargins(8, 8, 8, 8);
    vertLayoutMain->addLayout(horiLayoutBottom);

    QPushButton *buttonActive = new QPushButton(QStringLiteral("����"));
    QPushButton *buttonClose = new QPushButton(QStringLiteral("�ر�"));

    horiLayoutBottom->addStretch();
    horiLayoutBottom->addWidget(buttonActive);
    horiLayoutBottom->addWidget(buttonClose);

    //
    connect(buttonActive, &QPushButton::clicked, [=](){
        QString text = lineEdit->text();
        if (text.isEmpty()) {
            QMessageBox::warning(this, QStringLiteral("����"), QStringLiteral("ע���벻��Ϊ�գ�"));
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

        // ȡż����
        std::string output1;
        for (size_t i = 0; i < output.size() / 2; i++) {
            output1.append(1, output[i * 2 + 1]);
        }

        for (size_t i = 4; i < output1.size(); i+=5) {
            output1.insert(i, 1, '-');
        }

        if (text == QString::fromStdString(output1)) {
            this->writeSettingsSerialNumber(text);
            QMessageBox::information(this, QStringLiteral("��ʾ"), QStringLiteral("ע��ɹ���"));
            this->accept();
        } else {
            QMessageBox::warning(this, QStringLiteral("����"), QStringLiteral("ע������Ч��"));
        }
    });
    connect(buttonClose, &QPushButton::clicked, [=](){
        this->reject();
    });
}

// ������ܴ���
bool LicenseDialog::check()
{
    // �������ʱ���ȼ������Ƿ��Ѿ�ע��ɹ���
    QString serial = readSettingsSerialNumber();
    if (!serial.isEmpty()) {
        // ����Ƿ��Ѿ�ע�����ע���Ƿ�ɹ�
        if (license(serial)) {
            return true;
        }
    }

    LicenseDialog licenseDialog;
    licenseDialog.resize(400, 200);
    if (licenseDialog.exec() == QDialog::Accepted) {
        // �ٴμ���Ƿ��Ѿ�ע�����ע���Ƿ�ɹ�
        serial = readSettingsSerialNumber();	// ��ȡ���к�
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
