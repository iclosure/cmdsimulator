#ifndef JSQL_GLOBAL_H
#define JSQL_GLOBAL_H

#include <qglobal.h>

// JSQL_VERSION is (major << 16) + (minor << 8) + patch.

#define JSQL_VERSION       0x000001
#define JSQL_VERSION_STR   "0.0.1"

#if defined(Q_WS_WIN) || defined(Q_OS_WIN) || defined(Q_WS_S60)

#if defined(_MSC_VER) // MSVC Compiler
// template-class specialization 'identifier' is already instantiated
#pragma warning(disable: 4660)
#pragma warning(disable: 4819)  // warning : 936
#endif // _MSC_VER

#ifdef JSQL_DLL

#if defined(JSQL_MAKEDLL)
#define JSQL_EXPORT  Q_DECL_EXPORT
#else
#define JSQL_EXPORT  Q_DECL_IMPORT

#if defined(_DEBUG) || defined(DEBUG)
#pragma comment(lib, "jplugins.sqld.lib")
#else
#pragma comment(lib, "jplugins.sql.lib")
#endif

#endif

#endif // JSQL_DLL

#endif // Q_WS_WIN || Q_WS_S60

#ifndef JSQL_EXPORT
#define JSQL_EXPORT
#endif

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

// - namespace JSQL -
#define JSQL_NAMESPACE JSQL
#define JSQL_BEGIN_NAMESPACE namespace JSQL_NAMESPACE {
#define JSQL_END_NAMESPACE }
#define JSQL_USE_NAMESPACE using namespace JSQL_NAMESPACE;

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

namespace JSQL_NAMESPACE {

}

#endif // JSQL_GLOBAL_H
