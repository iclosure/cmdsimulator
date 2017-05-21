#-------------------------------------------------
#
# Project created by QtCreator 2015-08-01T00:00:11
#
#-------------------------------------------------

PROJECT_ROOT = $$PWD/../../../..

include($${PROJECT_ROOT}/source/common/build.pri)

# jprotocol

jprotocol_headers = \
    $$PWD/jprotocol_global.h\
    $$PWD/jprotocol_inc.h

HEADERS += \
    $$jprotocol_headers \
    $$PWD/jprotocol_object.h \
    $$PWD/jprotocol_table.h \
    $$PWD/jprotocol_system.h \
    $$PWD/jprotocol_vehicle.h \
    $$PWD/jprotocol_item.h \
    $$PWD/jprotocol_bit.h \
    $$PWD/jprotocol_counter.h \
    $$PWD/jprotocol_frame.h \
    $$PWD/jprotocol_framecode.h \
    $$PWD/jprotocol_header.h \
    $$PWD/jprotocol_numeric.h \
    $$PWD/jprotocol_check.h \
    $$PWD/jprotocol_complex.h \
    $$PWD/jprotocol_range.h

SOURCES += \
    $$PWD/jprotocol_global.cpp \
    $$PWD/jprotocol_object.cpp \
    $$PWD/jprotocol_table.cpp \
    $$PWD/jprotocol_system.cpp \
    $$PWD/jprotocol_vehicle.cpp \
    $$PWD/jprotocol_item.cpp \
    $$PWD/jprotocol_bit.cpp \
    $$PWD/jprotocol_counter.cpp \
    $$PWD/jprotocol_frame.cpp \
    $$PWD/jprotocol_framecode.cpp \
    $$PWD/jprotocol_header.cpp \
    $$PWD/jprotocol_numeric.cpp \
    $$PWD/jprotocol_check.cpp \
    $$PWD/jprotocol_complex.cpp \
    $$PWD/jprotocol_range.cpp

################################################
## framework
################################################

# for jplugin.jprotocol
framework_headers_jprotocol.version = Versions
framework_headers_jprotocol.path = Headers
framework_headers_jprotocol.files = $$jprotocol_headers

################################################
