
HEADERS += \
    $$PWD/scommandpage.h

SOURCES += \
    $$PWD/scommandpage.cpp

include($$PWD/linking/linking.pri)
include($$PWD/fly/fly.pri)
include($$PWD/diffrevise//diffrevise.pri)
include($$PWD/platform/platform.pri)
