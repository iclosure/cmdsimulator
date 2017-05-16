################################################################
# Data Analyse application
#
# Copyright (C) 2016 Smartsoft
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the Smartsoft License, Version 1.0
################################################################

################################################################
# creating a precompiled header file (only supported on some platforms (Windows - all MSVC project types,
# Mac OS X - Xcode, Makefile, Unix - gcc 3.3 and up)
################################################################

win32:CONFIG += precompile_header
PRECOMPILED_HEADER = $$PWD/precomp.h
INCLUDEPATH += $$PWD

precompile_header:!isEmpty(PRECOMPILED_HEADER) {
    DEFINES += USING_PCH
}

##
win32:CONFIG(release, debug|release): {
    DEFINES += QT_NO_DEBUG_OUTPUT
}

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

################################################################
# tmp
################################################################

exists($$PROJECT_ROOT) {
    macx: {
        J_PLAT_PREFIX = macx
    } else:unix: {
        J_PLAT_PREFIX = unix
    }

    J_TEMP_PREFIX = $${PROJECT_ROOT}/temp/source/$$TARGET/$$J_PLAT_PREFIX
    macx:unix: {
        MOC_DIR = $${J_TEMP_PREFIX}
        OBJECTS_DIR = $${J_TEMP_PREFIX}
        RCC_DIR = $${J_TEMP_PREFIX}
    }
}
