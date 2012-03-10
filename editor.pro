QT       += core gui

TARGET = editor
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    OpenFile.cpp \
    EditorManager.cpp

HEADERS  += MainWindow.h \
    OpenFile.h \
    EditorManager.h

FORMS    += MainWindow.ui
