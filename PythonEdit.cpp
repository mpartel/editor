#include "PythonEdit.h"
#include <QDebug>
#include <QKeyEvent>

PythonEdit::PythonEdit(QWidget *parent) :
    QTextEdit(parent)
{
}

void PythonEdit::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Tab) {
        QKeyEvent event = QKeyEvent(QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier, " ");
        for (int i = 0; i < 4; i++) {
            QTextEdit::keyPressEvent(&event);
        }
    } else {
        QTextEdit::keyPressEvent(e);
    }
}
