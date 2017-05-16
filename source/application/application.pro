#-------------------------------------------------
#
# Project created by QtCreator 2015-08-01T00:00:11
#
#-------------------------------------------------

QT += core gui xml concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

PROJECT_ROOT = $$PWD/../..

include($${PROJECT_ROOT}/source/common/config.pri)
include($${PROJECT_ROOT}/source/common/build.pri)

TEMPLATE = app
TARGET = $$qtLibraryTarget(cmdsimulator)
DESTDIR = $${PROJECT_ROOT}/bin

DEFINES += \
#    USE_JOFFICE \
    USE_JWIDGETS \
    USE_QWT \
    USE_ENCRYPT

##########
DEFINES += APP_LOG
##########

## - import libraries -

## sources

INCLUDEPATH += $$PWD

HEADERS += \

SOURCES += \
    main.cpp

RESOURCES += \
    resource/application.qrc

##################################################
## import libraries
##################################################

# com.smartsoft.jsmartkits.joffice

contains(DEFINES, USE_JMYOFFICE) {
    win32:CONFIG(release, debug|release): LIBS += -L$${PROJECT_ROOT}/lib/com.smartsoft.jsmartkits.joffice -lcom.smartsoft.jsmartkits.joffice
    else:win32:CONFIG(debug, debug|release): LIBS += -L$${PROJECT_ROOT}/lib/com.smartsoft.jsmartkits.joffice -lcom.smartsoft.jsmartkits.jofficed
    else:macx: LIBS += -framework com.smartsoft.jsmartkits.joffice -F$${PROJECT_ROOT}/lib/
    else:unix: LIBS += -L$${PROJECT_ROOT}/lib/com.smartsoft.jsmartkits.joffice -lcom.smartsoft.jsmartkits.joffice

    macx:{
        INCLUDEPATH += $${PROJECT_ROOT}/lib/com.smartsoft.jsmartkits.joffice.framework/Versions/Current/Headers
        DEPENDPATH += $${PROJECT_ROOT}/lib/com.smartsoft.jsmartkits.joffice.framework/Versions/Current/Headers
    } else {
        INCLUDEPATH += $${PROJECT_ROOT}/lib/com.smartsoft.jsmartkits.joffice/include
        DEPENDPATH += $${PROJECT_ROOT}/lib/com.smartsoft.jsmartkits.joffice/include
    }
    DEFINES += JOFFICE_DLL
}

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

# qwt

contains(DEFINES, USE_QWT) {
    win32:CONFIG(release, debug|release): LIBS += -L$${PROJECT_ROOT}/lib/core/qwt -lqwt
    else:win32:CONFIG(debug, debug|release): LIBS += -L$${PROJECT_ROOT}/lib/core/qwt -lqwtd
    else:macx: LIBS += -framework qwt -F$${PROJECT_ROOT}/lib/core/
    else:unix: LIBS += -L$${PROJECT_ROOT}/lib/core/qwt -lqwt

    macx:{
        INCLUDEPATH += $${PROJECT_ROOT}/lib/core/qwt.framework/Versions/Current/Headers
        DEPENDPATH += $${PROJECT_ROOT}/lib/core/qwt.framework/Versions/Current/Headers
    } else {
        INCLUDEPATH += $${PROJECT_ROOT}/lib/core/qwt/include
        DEPENDPATH += $${PROJECT_ROOT}/lib/core/qwt/include
    }
    DEFINES += QWT_DLL
}

# jplugins.sql

contains(DEFINES, USE_JSQL) {
    win32:CONFIG(release, debug|release): LIBS += -L$${PROJECT_ROOT}/lib/plugins/jplugins.sql -ljplugins.sql
    else:win32:CONFIG(debug, debug|release): LIBS += -L$${PROJECT_ROOT}/lib/plugins/jplugins.sql -ljplugins.sqld
    else:macx: LIBS += -framework jplugins.sql -F$${PROJECT_ROOT}/lib/plugins
    else:unix: LIBS += -L$${PROJECT_ROOT}/lib/plugins/jplugins.sql -ljplugins.sql

    macx:{
        INCLUDEPATH += $${PROJECT_ROOT}/lib/plugins/jplugins.sql.framework/Versions/Current/Headers
        DEPENDPATH += $${PROJECT_ROOT}/lib/plugins/jplugins.sql.framework/Versions/Current/Headers
    } else {
        INCLUDEPATH += $${PROJECT_ROOT}/lib/plugins/jplugins.sql/include
        DEPENDPATH += $${PROJECT_ROOT}/lib/plugins/jplugins.sql/include
    }
    DEFINES += JSQL_DLL
}

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
    srcfiles += $${PROJECT_ROOT}/lib/com.smartsoft.jsmartkits.joffice/com.smartsoft.jsmartkits.joffice
    # com.smartsoft.jsmartkits.jwidgets
    srcfiles += $${PROJECT_ROOT}/lib/com.smartsoft.jsmartkits.jwidgets/com.smartsoft.jsmartkits.jwidgets
    # qwt
    srcfiles += $${PROJECT_ROOT}/lib/core/qwt/qwt
    # jplugins.sql
    srcfiles += $${PROJECT_ROOT}/lib/plugins/jplugins.sql/jplugins.sql
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
