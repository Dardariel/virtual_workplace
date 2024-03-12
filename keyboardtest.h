#ifndef KEYBOARDTEST_H
#define KEYBOARDTEST_H

#include <QDialog>

#include <def_struct.h>

namespace Ui {
class KeyboardTest;
}

class KeyboardTest : public QDialog
{
    Q_OBJECT

public:
    explicit KeyboardTest(QWidget *parent = nullptr);
    ~KeyboardTest();


    void setStruct( const keyboardHID &keys );

private:
    Ui::KeyboardTest *ui;
};

#endif // KEYBOARDTEST_H
