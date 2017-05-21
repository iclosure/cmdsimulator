#ifndef JPROTOCOL_OBJECT_H
#define JPROTOCOL_OBJECT_H

#include <QObject>
#include "jprotocol_global.h"

namespace JProtocol {

class ObjectPrivate;

class Object : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int objectType READ objectType NOTIFY objectTypeChanged)
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString domain READ domain WRITE setDomain NOTIFY domainChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString mark READ mark WRITE setMark NOTIFY markChanged)
    Q_PROPERTY(QString desc READ desc WRITE setDesc NOTIFY descChanged)
    Q_PROPERTY(int childCount READ childCount NOTIFY childCountChanged)
public:
    explicit Object(int objectType = Enum::ObjectInvalid, QObject *parent = 0);
    virtual ~Object();

    Object &operator=(const Object &);
    virtual Object *clone() const;

    int objectType() const;
    QString id() const;
    QString domain() const;
    QString name() const;
    QString mark() const;
    QString desc() const;
    virtual int childCount() const;

signals:
    void objectTypeChanged();
    void idChanged(const QString &value);
    void domainChanged(const QString &value);
    void nameChanged(const QString &value);
    void markChanged(const QString &value);
    void descChanged(const QString &value);
    void childCountChanged();

public slots:
    void setId(const QString &value);
    void setDomain(const QString &value);
    void setName(const QString &value);
    void setMark(const QString &value);
    void setDesc(const QString &value);

protected:
    Object(const Object &);

private:
    J_DECLARE_PRIVATE(Object)
};

} // end of namespace JProtocol 

#endif // JPROTOCOL_OBJECT_H
