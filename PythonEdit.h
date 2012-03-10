#ifndef MYTEXTEDIT_H
#define MYTEXTEDIT_H

#include <QTextEdit>

class PythonEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit PythonEdit(QWidget *parent = 0);

protected:
    void keyPressEvent(QKeyEvent *e);

signals:

public slots:

};

#endif // PYTHONEDIT_H
