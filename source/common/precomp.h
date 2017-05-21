#ifndef PRECOMP_H
#define PRECOMP_H

#ifdef QT_CORE_LIB
#include <QtCore>
#endif

#ifdef QT_GUI_LIB
#include <QtGui>
#endif

#if (QT_VERSION < 0x050000)
#else
#ifdef QT_WIDGETS_LIB
#include <QtWidgets>
#endif
#endif
#ifdef QT_SQL_LIB
#include <QtSql>
#endif

#ifdef QT_XML_LIB
#include <QtXml>
#endif

#ifdef QT_QUICK_LIB
#include <QtQuick>
#endif

#ifdef QT_CHARTS_LIB
#include <QtCharts>
#endif

#ifdef QT_AXCONTAINER_LIB
#include <ActiveQt>
#endif

#ifdef QT_CONCURRENT_LIB
#include <QtConcurrent>
#endif

#if defined(Q_WS_MAC) || defined(Q_OS_MACX)
#ifdef QT_OPENGL_LIB
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#endif
#else
#ifdef QT_OPENGL_LIB
#include <GL/glut.h>
#endif
#endif

#if (QT_VERSION < 0x050000) && (defined(Q_OS_WIN) || defined(Q_WS_WIN))
#include <Windows.h>
#endif

#if defined(Q_OS_WIN) || defined(Q_WS_WIN)

#if _MSC_VER >= 1700
//#pragma execution_character_set("UTF-8")
#endif

#endif

// - common -

// - private pointer
#ifndef J_DECLARE_PRIVATE
#define J_DECLARE_PRIVATE(name) \
    Q_DECLARE_PRIVATE(name) \
    name ## Private *d_ptr;
#endif

#ifndef J_DECLARE_PUBLIC
#define J_DECLARE_PUBLIC(name) \
    Q_DECLARE_PUBLIC(name) \
    name *q_ptr;
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

// extern library

#ifdef USE_JOFFICE
#include "joffice_inc.h"
#endif

#ifdef USE_JWIDGETS
#include "jwidgets_inc.h"
#endif

#ifdef USE_JSQL
#include "jsql_inc.h"
#endif

#ifdef USE_QWT
#include "qwt.h"
#endif

#endif // PRECOMP_H
