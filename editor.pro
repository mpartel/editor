QT       += core gui webkit

TARGET = editor
TEMPLATE = app

SOURCES += main.cpp\
        MainWindow.cpp \
    OpenFile.cpp \
    EditorManager.cpp \
    PythonHighlighter.cpp \
    PythonEdit.cpp \
    TextLineBuffer.cpp \
    IOView.cpp

HEADERS  += MainWindow.h \
    OpenFile.h \
    EditorManager.h \
    PythonHighlighter.h \
    PythonEdit.h \
    TextLineBuffer.h \
    IOView.h \
    common.h

FORMS    += MainWindow.ui

OTHER_FILES += \
    TODO.txt \
    IOView_page.html

RESOURCES += \
    resources.qrc
