QT       += core gui

TARGET = editor
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    OpenFile.cpp \
    EditorViewManager.cpp

HEADERS  += MainWindow.h \
    OpenFile.h \
    EditorViewManager.h

FORMS    += MainWindow.ui
