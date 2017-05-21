#include "precomp.h"
#include "jprotocol_item.h"

namespace JProtocol {

// ItemPrivate

class ItemPrivate
{
public:
    ItemPrivate(Item *q)
        : q_ptr(q)
        , itemType(Enum::ItemInvalid)
        , itemOffset(1)
        , buffer(Q_NULLPTR)
        , bufferSize(0)
        , bufferOffset(0)
        , defaultVaue(0)
    {

    }

    void init();
    ItemPrivate &operator =(const ItemPrivate &rhs);

private:
    J_DECLARE_PUBLIC(Item)
    int itemType;
    int itemOffset;
    char *buffer;
    qreal bufferSize;
    qreal bufferOffset;
    qreal defaultVaue;
};

void ItemPrivate::init()
{

}

ItemPrivate &ItemPrivate::operator =(const ItemPrivate &rhs)
{
    if (this == &rhs) {
        return *this;
    }
    itemType = rhs.itemType;
    itemOffset = rhs.itemOffset;
    bufferSize = rhs.bufferSize;
    bufferOffset = rhs.bufferOffset;
    defaultVaue = rhs.defaultVaue;
    return *this;
}

// Item

Item::Item(int itemType, QObject *parent)
    : Object(Enum::ObjectItem, parent)
    , d_ptr(new ItemPrivate(this))
{
    Q_D(Item);
    d->itemType = itemType;
    d->init();
}

Item::~Item()
{
    Q_D(Item);
    delete d;
}

Item &Item::operator=(const Item &rhs)
{
    Object::operator =(rhs);
    Q_D(Item);
    d->operator =(*rhs.d_ptr);
    return *this;
}

Item *Item::clone() const
{
    return new Item(*this);
}

int Item::itemType() const
{
    Q_D(const Item);
    return d->itemType;
}

int Item::itemOffset() const
{
    Q_D(const Item);
    return d->itemOffset;
}

qreal Item::bufferSize() const
{
    Q_D(const Item);
    return d->bufferSize;
}

qreal Item::bufferOffset() const
{
    Q_D(const Item);
    return d->bufferOffset;
}

qreal Item::data() const
{
    return 0;
}

QString Item::dataString() const
{
    return "";
}

qreal Item::defaultValue() const
{
    Q_D(const Item);
    return d->defaultVaue;
}

QString Item::typeName() const
{
    return typeString();
}

QString Item::typeString() const
{
    Q_D(const Item);
    return typeString(d->itemType);
}

QString Item::codeName() const
{
    return mark();
}

char *Item::buffer() const
{
    Q_D(const Item);
    return d->buffer;
}

QString Item::typeString(int itemType) const
{
    switch (itemType) {
    case Enum::ItemHeader: return "header";
    case Enum::ItemCounter: return "counter";
    case Enum::ItemCheck: return "check";
    case Enum::ItemFrameCode: return "framecode";
    case Enum::ItemFrame: return "frame";
    case Enum::ItemNumeric: return "numeric";
    case Enum::ItemBitMap: return "bitmap";
    case Enum::ItemBitValue: return "bitvalue";
    case Enum::ItemComplex: return "complex";
    default: return "invalid";
    }
}

int Item::stringType(const QString &str) const
{
    typedef std::map<QString, const int> map_strtype;
    static const map_strtype::value_type map_data[Enum::ItemTotal] = {
        map_strtype::value_type("head", Enum::ItemHeader),
        map_strtype::value_type("counter", Enum::ItemCounter),
        map_strtype::value_type("check", Enum::ItemCheck),
        map_strtype::value_type("framecode", Enum::ItemFrameCode),
        map_strtype::value_type("frame", Enum::ItemFrame),
        map_strtype::value_type("numeric", Enum::ItemNumeric),
        map_strtype::value_type("bitmap", Enum::ItemBitMap),
        map_strtype::value_type("bitvalue", Enum::ItemBitValue),
        map_strtype::value_type("complex", Enum::ItemComplex)
    };
    static const map_strtype _map(map_data, map_data + _countof(map_data));
    map_strtype::const_iterator citer = _map.find(str);
    if (citer == _map.cend()) {
        return "invalid";
    } else {
        return citer->second;
    }
}

void Item::setData(qreal value)
{
    Q_UNUSED(value);
}

void Item::setDefaultValue(qreal value)
{
    Q_D(Item);
    if (value != d->defaultVaue) {
        d->defaultVaue = value;
        emit defaultValueChanged(value);
        setData(d->defaultVaue);
    }
}

Item::Item(const Item &rhs)
    : Object(rhs)
    , d_ptr(new ItemPrivate(this))
{
    Q_D(Item);
    d->operator =(rhs.d_ptr);
}

void Item::setItemType(int value)
{
    Q_D(Item);
    if (value != d->itemType) {
        d->itemType = value;
        emit itemTypeChanged();
    }
}

void Item::setBuffer(char *buffer)
{
    Q_D(Item);
    d->buffer = buffer;
    setData(d->defaultVaue);
}

void Item::setItemOffset(int value)
{
    Q_D(Item);
    if (value != d->itemOffset) {
        d->itemOffset = value;
        emit itemOffsetChanged(value);
    }
}

void Item::setBufferSize(qreal value)
{
    Q_D(Item);
    if (value != d->bufferSize) {
        d->bufferSize = value;
        emit bufferSizeChanged(value);
    }
}

void Item::setBufferOffset(qreal value)
{
    Q_D(Item);
    if (value != d->bufferOffset) {
        d->bufferOffset = value;
        emit bufferOffsetChanged(value);
    }
}

}   // end of namespace JProtocol 
