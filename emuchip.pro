TARGET = emuchip-qt
TEMPLATE = app
QT = core gui widgets
CONFIG += qt release 
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
HEADERS += src/qt/mainwindow.h src/chip_emu.h src/qt/displaywidget.h
SOURCES += src/qt/main.cpp src/qt/mainwindow.cpp src/chip_emu.cpp src/qt/displaywidget.cpp
GIT_VERSION = $$system($$quote(git describe --tags))
GIT_TIMESTAMP = $$system($$quote(git log -n 1 --format=format:"%at"))
QMAKE_SUBSTITUTES += $$PWD/src/version.h.in
target.path = /usr/local/bin
INSTALLS += target
