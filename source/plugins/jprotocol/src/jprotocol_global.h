#ifndef JPROTOCOL_GLOBAL_H
#define JPROTOCOL_GLOBAL_H

#include <qglobal.h>

// JPROTOCOL_VERSION is (major << 16) + (minor << 8) + patch.

#define JPROTOCOL_VERSION       0x000001
#define JPROTOCOL_VERSION_STR   "0.0.1"

#if defined(Q_WS_WIN) || defined(Q_OS_WIN) || defined(Q_WS_S60)

#if defined(_MSC_VER) // MSVC Compiler
// template-class specialization 'identifier' is already instantiated
#pragma warning(disable: 4660)
#pragma warning(disable: 4819)  // warning : 936
#endif // _MSC_VER

#ifdef JPROTOCOL_DLL

#if defined(JPROTOCOL_MAKEDLL)
#define JPROTOCOL_EXPORT  Q_DECL_EXPORT
#else
#define JPROTOCOL_EXPORT  Q_DECL_IMPORT

#if defined(_DEBUG) || defined(DEBUG)
#pragma comment(lib, "jplugins.jprotocold.lib")
#else
#pragma comment(lib, "jplugins.jprotocol.lib")
#endif

#endif

#endif // JPROTOCOL_DLL

#endif // Q_WS_WIN || Q_WS_S60

#ifndef JPROTOCOL_EXPORT
#define JPROTOCOL_EXPORT
#endif

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

// - namespace JProtocol  -
#define JPROTOCOL_namespace JProtocol
#define JPROTOCOL_BEGIN_NAMESPACE namespace JProtocol _NAMESPACE {
#define JPROTOCOL_END_NAMESPACE }
#define JPROTOCOL_USE_NAMESPACE using namespace JProtocol _NAMESPACE;

// - private pointer
#ifndef J_DECLARE_PRIVATE
#define J_DECLARE_PRIVATE(name) \
    name ## Private *d_ptr; \
    Q_DECLARE_PRIVATE(name)
#endif

#ifndef J_DECLARE_PUBLIC
#define J_DECLARE_PUBLIC(name) \
    name *q_ptr; \
    Q_DECLARE_PUBLIC(name)
#endif

#ifndef J_DECLARE_SINGLE_INSTANCE
#define J_DECLARE_SINGLE_INSTANCE(Class) \
public: \
    static Class *instance(); \
    static void releaseInstance(); \
private: \
    static Class *_instance;
#endif

#ifndef J_IMPLEMENT_SINGLE_INSTANCE
#define J_IMPLEMENT_SINGLE_INSTANCE(Class) \
Class *Class::_instance = 0; \
\
Class *Class::instance() { \
    if (Class::_instance == 0) { \
        Class::_instance = new Class; \
    } \
    return Class::_instance; \
} \
\
void Class::releaseInstance() { \
    if (Class::_instance != 0) { \
        delete Class::_instance; \
        Class::_instance = 0; \
    } \
}
#endif

#include <memory>
#include <utility>

#ifndef J_FRIEND_HANDLEPTR
#define J_FRIEND_HANDLEPTR() \
    template<typename T> friend class ::std::_Ref_count; \
    template<typename T> friend class ::std::shared_ptr;
#endif

#ifndef J_HANDLE_CAST
#define J_HANDLE_CAST(_type_, _object_a) \
    _type_ ## Ptr::dynamicCast<_type_>(_object_)
#endif

namespace JProtocol {

#ifndef J_HANDLEPTR_CLASS
#define J_HANDLEPTR_CLASS

template<typename X> class HandlePtr : public ::std::shared_ptr<X>
{
public:
    HandlePtr() : ::std::shared_ptr() {}
    HandlePtr(X *ptr) : ::std::shared_ptr() {}
    template<typename _Dt>
    HandlePtr(X *ptr, _Dt deleter) : ::std::shared_ptr(ptr, deleter) {}
};

#endif // J_HANDLEPTR_CLASS

#include <QObject>

class Enum : public QObject
{
    Q_OBJECT
public:
    enum ObjectType {
        ObjectInvalid = -1,
        ObjectVehicle,
        ObjectSystem,
        ObjectTable,
        ObjectItem,

        ObjectTotal
    };

    enum ItemType {
        ItemInvalid = -1,
        ItemHeader,
        ItemCounter,
        ItemCheck,
        ItemFrameCode,
        ItemFrame,
        ItemNumeric,
        ItemBitMap,
        ItemBitValue,
        ItemComplex,

        ItemTotal
    };

    enum CheckType {
        CheckNone,
        CheckSum8,
        CheckSum16,
        CheckSum32,
        CheckCrc8,
        CheckCrc16,
        CheckXor8,
        CheckXor16,

        CheckTotal
    };

    enum CounterType {
        CounterInvalid = -1,
        Counter8,
        Counter16,
        Counter32,
        Counter64,

        CounterTotal
    };
};

} // end of namespace JProtocol

#endif // JPROTOCOL_GLOBAL_H
