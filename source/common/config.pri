################################################################
# Data Analyse application
#
# Copyright (C) 2015 Smartsoft
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the Smartsoft License, Version 1.0
################################################################

######################################################################
# Definitions
######################################################################

######################################################################
# Copyright Information
######################################################################

FRAMEWORK_ROOT = $$PWD/../..

QMAKE_TARGET_COMPANY = "Smartsoft"
QMAKE_TARGET_DESCRIPTION = "Data Analyse"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2015-2017 Smartsoft. All rights reserved."

win32:RC_ICONS = $${FRAMEWORK_ROOT}/config/images/logo.ico

#VER_MAJ = 1
#VER_MIN = 0
#VER_PAT = 0
#VERSION = $${VER_MAJ}.$${VER_MIN}.$${VER_PAT}

TARGET_VER_MAJ = 1
TARGET_VER_MIN = 0
TARGET_VER_PAT = 0
DEFINES += TARGET_VER_MAJ TARGET_VER_MIN TARGET_VER_PAT

######################################################################
# UiLoader will be enable or not.
######################################################################

#APP_CONFIG           += uitools

######################################################################
# widget module will be enable or not.
######################################################################

APP_CONFIG           += widgets

######################################################################
# sql module will be enable or not.
######################################################################

APP_CONFIG           += sql

######################################################################
# Build the static/shared libraries.
# If QwtDll is enabled, a shared library is built, otherwise
# it will be a static library.
######################################################################

#APP_CONFIG           += QwtDll

######################################################################
# Install paths
######################################################################

APP_INSTALL_PREFIX = $$[QT_INSTALL_PREFIX]

unix {
    APP_INSTALL_PREFIX    = /usr/local/$${TARGET}-VERSION
}

win32 {
    APP_INSTALL_PREFIX    = C:/$${TARGET}-VERSION
}

APP_INSTALL_BINS      = $${APP_INSTALL_PREFIX}/bin
APP_INSTALL_DOCS      = $${APP_INSTALL_PREFIX}/doc
APP_INSTALL_HEADERS   = $${APP_INSTALL_PREFIX}/include
