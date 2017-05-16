#ifndef LICENSEDIALOG_H
#define LICENSEDIALOG_H

#include <QDialog>

class LicenseDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LicenseDialog(QWidget *parent = 0);

    static bool check();

    static bool license(const QString &serial);

private:
    static void writeSettingsSerialNumber(const QString &text);
    static QString readSettingsSerialNumber();

private:
};

#endif // LICENSEDIALOG_H
