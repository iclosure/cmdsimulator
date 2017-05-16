#-------------------------------------------------
#
# Project created by QtCreator 2015-08-01T00:00:11
#
#-------------------------------------------------

FRAMEWORK_ROOT = $$PWD/../../../..

include($${FRAMEWORK_ROOT}/source/common/build.pri)

# jsql

jsql_headers = \
    $$PWD/jsql_global.h\
    $$PWD/jsql_inc.h

HEADERS += \
    $$jsql_headers \
    $$PWD/sqlite/sqlite3.h

SOURCES += \
    $$PWD/jsql_global.cpp

################################################
## framework
################################################

# for jplugin.sql
framework_headers_jsql.version = Versions
framework_headers_jsql.path = Headers
framework_headers_jsql.files = $$jsql_headers

################################################
