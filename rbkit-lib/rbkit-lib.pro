QT += core gui webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

VERSION = 0.0.0.pre
TARGET = rbkit
TEMPLATE = lib

CONFIG += staticlib

SOURCES += \
    subscriber.cpp \
    rbkitmainwindow.cpp \
    askhost.cpp \
    gcstat.cpp \
    rbcommands.cpp \
    zmqsockets.cpp

HEADERS +=  \
    subscriber.h \
    rbkitmainwindow.h \
    askhost.h \
    gcstat.h \
    eventdata.h\
    zmqsockets.h

# Include msgpack via pri file
include($$RC_ROOT_SOURCE_DIR/common.pri)

FORMS += \
    rbkitmainwindow.ui \
    askhost.ui
