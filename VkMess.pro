#-------------------------------------------------
#
# Project created by QtCreator 2013-10-24T21:03:01
#
#-------------------------------------------------

QT       += core gui webkitwidgets xml network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VkMess
TEMPLATE = app


SOURCES += main.cpp \
    vkauth.cpp \
    messages.cpp \
    enterfilter.cpp

HEADERS  += \
    vkauth.h \
    messages.h \
    enterfilter.h

RESOURCES += \
    source.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/source/BASS/ -lbass
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/source/BASS/ -lbass
else:unix: LIBS += -L$$PWD/source/BASS/ -lbass

INCLUDEPATH += $$PWD/source/BASS
DEPENDPATH += $$PWD/source/BASS
