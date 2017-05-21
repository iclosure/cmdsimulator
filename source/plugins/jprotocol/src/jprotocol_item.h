#ifndef JPROTOCOL_ITEM_H
#define JPROTOCOL_ITEM_H

#include "jprotocol_object.h"

namespace JProtocol {

class ItemPrivate;

class Item : public Object
{
    Q_OBJECT
    Q_PROPERTY(int itemType READ itemType NOTIFY itemTypeChanged)
    Q_PROPERTY(int itemOffset READ itemOffset NOTIFY itemOffsetChanged)
    Q_PROPERTY(qreal bufferSize READ bufferSize NOTIFY bufferSizeChanged)
    Q_PROPERTY(qreal bufferOffset READ bufferOffset NOTIFY bufferOffsetChanged)
    Q_PROPERTY(qreal data READ data WRITE setData NOTIFY dataChanged)
    Q_PROPERTY(QString dataString READ dataString NOTIFY dataStringChanged)
    Q_PROPERTY(qreal defaultValue READ defaultValue WRITE setDefaultValue NOTIFY defaultValueChanged)
    Q_PROPERTY(QString typeName READ typeName NOTIFY typeNameChanged)
    Q_PROPERTY(QString typeString READ typeString NOTIFY typeStringChanged)
    Q_PROPERTY(QString codeName READ codeName NOTIFY codeNameChanged)
public:
    explicit Item(int itemType = Enum::ItemInvalid, QObject *parent = 0);
    virtual ~Item();

    Item &operator=(const Item &);
    virtual Item *clone() const;

    int itemType() const;
    int itemOffset() const;
    virtual qreal bufferSize() const;
    virtual qreal bufferOffset() const;
    virtual qreal data() const;
    virtual QString dataString() const;
    qreal defaultValue() const;
    virtual QString typeName() const;
    virtual QString typeString() const;
    virtual QString codeName() const;

    char *buffer() const;
    Q_INVOKABLE QString typeString(int itemType) const;
    Q_INVOKABLE int stringType(const QString &str) const;

signals:
    void itemTypeChanged();
    void itemOffsetChanged(int value);
    void bufferSizeChanged(real value);
    void bufferOffsetChanged(real value);
    void dataChanged(qreal value);
    void dataStringChanged();
    void defaultValueChanged(qreal value);
    void typeNameChanged();
    void typeStringChanged();
    void codeNameChanged();

public slots:
    void setData(qreal value);
    void setDefaultValue(qreal value);

protected:
    Item(const Item &);
    void setItemType(int value);
    virtual void setBuffer(char *buffer);
    virtual void setItemOffset(int value);
    virtual void setBufferSize(qreal value);
    virtual void setBufferOffset(qreal value);

private:
    J_DECLARE_PRIVATE(Item)
    friend class Table;
};

} // end of namespace JProtocol 

#endif // JPROTOCOL_ITEM_H
