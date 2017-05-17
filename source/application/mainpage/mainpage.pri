
HEADERS += \
    $$PWD/mainpagewidget.h \
    $$PWD/mainnavwidget.h \
    $$PWD/mainviewwidget.h

SOURCES += \
    $$PWD/mainpagewidget.cpp \
    $$PWD/mainnavwidget.cpp \
    $$PWD/mainviewwidget.cpp

include($$PWD/ccommand/ccommand.pri)
include($$PWD/scommand/scommand.pri)
include($$PWD/config/config.pri)
