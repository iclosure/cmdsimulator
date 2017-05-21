#include "precomp.h"
#include "jprotocol_object.h"

namespace JProtocol {

// ObjectPrivate

class ObjectPrivate
{
public:
    ObjectPrivate(Object *q)
        : q_ptr(q)
        , objectType(Enum::ObjectInvalid)
    {

    }

    void init();
    ObjectPrivate &operator =(const ObjectPrivate &rhs);

private:
    J_DECLARE_PUBLIC(Object)
    int objectType;
    QString id;
    QString domain;
    QString name;
    QString mark;
    QString desc;
};

void ObjectPrivate::init()
{

}

ObjectPrivate &ObjectPrivate::operator =(const ObjectPrivate &rhs)
{
    if (this == &rhs) {
        return *this;
    }
    objectType = rhs.objectType;
    id = rhs.id;
    domain = rhs.domain;
    name = rhs.name;
    mark = rhs.mark;
    desc = rhs.desc;
    return *this;
}

// Object

Object::Object(int objectType, QObject *parent)
    : QObject(parent)
    , d_ptr(new ObjectPrivate(this))
{
    Q_D(Object);
    d->objectType = objectType;
    d->init();
}

Object::~Object()
{
    Q_D(Object);
    delete d;
}

Object &Object::operator=(const Object &rhs)
{
    Q_D(Object);
    d->operator =(*rhs.d_ptr);
    return *this;
}

Object *Object::clone() const
{
    return new Object(*this);
}

int Object::objectType() const
{
    Q_D(const Object);
    return d->objectType;
}

QString Object::id() const
{
    Q_D(const Object);
    return d->id;
}

QString Object::domain() const
{
    Q_D(const Object);
    return d->domain;
}

QString Object::name() const
{
    Q_D(const Object);
    return d->name;
}

QString Object::mark() const
{
    Q_D(const Object);
    return d->mark;
}

QString Object::desc() const
{
    Q_D(const Object);
    return d->desc;
}

int Object::childCount() const
{
    return 0;
}

void Object::setId(const QString &value)
{
    Q_D(Object);
    if (value != d->id) {
        d->id = value;
        emit idChanged(value);
    }
}

void Object::setDomain(const QString &value)
{
    Q_D(Object);
    if (value != d->domain) {
        d->domain = value;
        emit domainChanged(value);
    }
}

void Object::setName(const QString &value)
{
    Q_D(Object);
    if (value != d->name) {
        d->name = value;
        emit nameChanged(value);
    }
}

void Object::setMark(const QString &value)
{
    Q_D(Object);
    if (value != d->mark) {
        d->mark = value;
        emit markChanged(value);
    }
}

void Object::setDesc(const QString &value)
{
    Q_D(Object);
    if (value != d->desc) {
        d->desc = value;
        emit descChanged(value);
    }
}

Object::Object(const Object &rhs)
    : QObject(rhs.parent())
    , d_ptr(new ObjectPrivate(this))
{
    Q_D(Object);
    d->operator =(rhs.d_ptr);
}

}   // end of namespace JProtocol 
