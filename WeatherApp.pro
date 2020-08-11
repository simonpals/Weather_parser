QT       += core gui
QT += network webengine webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

DEPENDPATH += $$PWD/./

DEFINES += QT_DEPRECATED_WARNINGS

#QTWEBENGINE_CHROMIUM_FLAGS +="--disable-background-timer-throttling"

SOURCES += \
    htmldataparser.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    htmldataparser.h \
    mainwindow.h

HEADERS +=  qhtmlparser.h

FORMS += \
    mainwindow.ui

win32: LIBS += -L$$PWD/./ -lqhtmlparser

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/./qhtmlparser.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/./libqhtmlparser.a

unix:!macx: LIBS += -L$$PWD/./ -lqhtmlparser

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

unix:!macx: PRE_TARGETDEPS += $$PWD/./libqhtmlparser.a

unix{
    LIBS += -L/usr/lib -ltidy
}
