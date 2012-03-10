QT       += core gui

TARGET = editor
TEMPLATE = app

SOURCES += main.cpp\
        MainWindow.cpp \
    OpenFile.cpp \
    EditorManager.cpp \
    PythonHighlighter.cpp \
    PythonEdit.cpp

HEADERS  += MainWindow.h \
    OpenFile.h \
    EditorManager.h \
    PythonHighlighter.h \
    PythonEdit.h

FORMS    += MainWindow.ui

OTHER_FILES += \
    TODO.txt
