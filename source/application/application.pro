#-------------------------------------------------
#
# Project created by QtCreator 2015-08-01T00:00:11
#
#-------------------------------------------------

QT += core gui xml concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

PROJECT_ROOT = $$PWD/../..

DEFINES += \
    USE_JWIDGETS

include($${PROJECT_ROOT}/source/common/config.pri)
include($${PROJECT_ROOT}/source/common/build.pri)

TEMPLATE = app
TARGET = $$qtLibraryTarget(cmdsimulator)
DESTDIR = $${PROJECT_ROOT}/bin

##########
DEFINES += APP_LOG
##########

## - import libraries -

## sources

INCLUDEPATH += $$PWD

HEADERS += \
    centralwidget.h \
    customtype.h \
    globalconfig.h \
    mainwindow.h \
    startpagemgr.h \
    statusbarmgr.h

SOURCES += \
    main.cpp \
    centralwidget.cpp \
    globalconfig.cpp \
    mainwindow.cpp \
    startpagemgr.cpp \
    statusbarmgr.cpp

RESOURCES += \
    resource/application.qrc

include($$PWD/utility/utility.pri)
include($$PWD/mainpage/mainpage.pri)

## copy libraries

copyCommand = @echo ---- module $${TARGET}: copy libraries ----

macx: {
    ##
} else:win32|unix: {

    destdir = $${PROJECT_ROOT}/bin
    srcfiles =

    ## lib-suffix
    win32:CONFIG(debug, debug|release):lib_suffix=d.dll
    else:win32:CONFIG(release, debug|release):lib_suffix=.dll
    else:unix:lib_suffix=.so

    win32:destdir = $$replace(destdir, /, \\)

    # com.smartsoft.jsmartkits.joffice
    srcfiles += $${PROJECT_ROOT}/lib/com.smartsoft.jsmartkits.joffice
    # com.smartsoft.jsmartkits.jwidgets
    srcfiles += $${PROJECT_ROOT}/lib/com.smartsoft.jsmartkits.jwidgets
    # jplugins.sql
    srcfiles += $${PROJECT_ROOT}/lib/jplugins.sql/jplugins.sql
    ## copy files
    for (srcfile, srcfiles) {
        srcfile = $$srcfile$${lib_suffix}
        exists("$$srcfile"): {
            win32:srcfile = $$replace(srcfile, /, \\)
            copyCommand += && $(COPY_FILE) "$$srcfile" "$$destdir"
        }
    }
}

first.depends = $(first)
application.commands = $$copyCommand
first.depends += application

QMAKE_EXTRA_TARGETS += first \
    application
