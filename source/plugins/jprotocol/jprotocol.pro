#-------------------------------------------------
#
# Project created by QtCreator 2015-08-01T00:00:11
#
#-------------------------------------------------

QT += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

PROJECT_ROOT = $$PWD/../../..

jtarget_url = jplugins.jprotocol

TEMPLATE = lib
win32 {
    TARGET = $$qtLibraryTarget($${jtarget_url})
} else {
    TARGET = $${jtarget_url}
}

macx {
    DESTDIR = $${PROJECT_ROOT}/lib
} else {
    DESTDIR = $${PROJECT_ROOT}/lib/$${jtarget_url}
}

CONFIG += dll warn_on lib_bundle
#CONFIG += no_keywords silent
#DEFINES += QT_NO_DEBUG_OUTPUT

DEFINES +=

##

contains(CONFIG, dll) {
    DEFINES += JPROTOCOL_DLL JPROTOCOL_MAKEDLL
}

win32 {
    CONFIG += debug_and_release
    CONFIG += build_all
} else: macx: {
    DEFINES -= Q_CC_CLANG
    CONFIG += qt_framework
} else: unix {
    CONFIG += dll
    LIBS +=
}

include($$PWD/src/src.pri)

# com.smartsoft.jsmartkits.jwidgets

contains(DEFINES, USE_JWIDGETS) {
    win32:CONFIG(release, debug|release): LIBS += -L$${PROJECT_ROOT}/lib/com.smartsoft.jsmartkits.jwidgets -lcom.smartsoft.jsmartkits.jwidgets
    else:win32:CONFIG(debug, debug|release): LIBS += -L$${PROJECT_ROOT}/lib/com.smartsoft.jsmartkits.jwidgets -lcom.smartsoft.jsmartkits.jwidgetsd
    else:macx: LIBS += -framework com.smartsoft.jsmartkits.jwidgets -F$${PROJECT_ROOT}/lib/
    else:unix: LIBS += -L$${PROJECT_ROOT}/lib/com.smartsoft.jsmartkits.jwidgets -lcom.smartsoft.jsmartkits.jwidgets

    macx:{
        INCLUDEPATH += $${PROJECT_ROOT}/lib/com.smartsoft.jsmartkits.jwidgets.framework/Versions/Current/Headers
        DEPENDPATH += $${PROJECT_ROOT}/lib/com.smartsoft.jsmartkits.jwidgets.framework/Versions/Current/Headers
    } else {
        INCLUDEPATH += $${PROJECT_ROOT}/lib/com.smartsoft.jsmartkits.jwidgets/include
        DEPENDPATH += $${PROJECT_ROOT}/lib/com.smartsoft.jsmartkits.jwidgets/include
    }
    DEFINES += JWIDGETS_DLL
}

macx: CONFIG(qt_framework, qt_framework|qt_no_framework) {
    QMAKE_LFLAGS_SONAME = -Wl,-install_name,$$DESTDIR/
    CONFIG += lib_bundle
    QMAKE_BUNDLE_DATA += \
        framework_headers_jsql
} else {
    copyCommand = @echo ---- module $${jtarget_url} ----
    win32 {
        ## create folder
        destdir = $$DESTDIR/include
        destdir = $$replace(destdir, /, \\)
        ## copy files
        excludefile = $$PWD/copy.ignore
        !exists("$$excludefile"): excludefile = $$PROJECT_ROOT/source/common/copy.ignore
        !exists("$$excludefile"): error("$$excludefile" is not exists!)
        excludefile = $$replace(excludefile, /, \\)
        srcdir = $$PWD/src
        srcdir = $$replace(srcdir, /, \\)
        #exists("$$destdir"): copyCommand += && rd /s /q "$$destdir"
        copyCommand += && xcopy "$$srcdir\\*.h" "$$destdir" /i /s /y /exclude:"$$excludefile"
    } else {
        ## create folder
        destdir = $$DESTDIR/include
        ## copy files
        excludefile = $$PWD/copy.ignore
        !exists("$$excludefile"): excludefile = $$PROJECT_ROOT/source/common/copy.ignore
        !exists("$$excludefile"): error("$$excludefile" is not exists!)
        srcdir = $$PWD/src
        !exists("$$destdir"): copyCommand += && mkdir "$$destdir"
        copyCommand += && cp "$$srcdir/*.h" "$$destdir"
        #copyCommand += && xcopy "$$srcdir/*.h" "$$destdir" /i /s /y /exclude:"$$excludefile"
        copyCommand += && rm -f -r "$$destdir/precomp.h"
    }

    first.depends = $(first)
    # for jplugin.sql
    framework_headers_jsql.commands = $$copyCommand
    first.depends += framework_headers_jsql

    QMAKE_EXTRA_TARGETS += first \
        framework_headers_jsql
}
