#include "keyboardtest.h"
#include "ui_keyboardtest.h"

#include <QDebug>

KeyboardTest::KeyboardTest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KeyboardTest)
{
    ui->setupUi(this);
    connect(ui->pb_close, &QPushButton::clicked, this, &QDialog::hide);
}

KeyboardTest::~KeyboardTest()
{
    delete ui;
}

void KeyboardTest::setStruct(const keyboardHID &keys )
{
    ui->le_keycode_1->setText( QString("%1").arg( keys.KEYCODE1 ) );
    ui->le_keycode_2->setText( QString("%1").arg( keys.KEYCODE2 ) );
    ui->le_keycode_3->setText( QString("%1").arg( keys.KEYCODE3 ) );
    ui->le_keycode_4->setText( QString("%1").arg( keys.KEYCODE4 ) );
    ui->le_keycode_5->setText( QString("%1").arg( keys.KEYCODE5 ) );
    ui->le_keycode_6->setText( QString("%1").arg( keys.KEYCODE6 ) );


    ui->cb_bit_0->setCheckState( ( keys.MODIFIER & ( 0x01<<0 ) ) ? Qt::Checked : Qt::Unchecked );
    ui->cb_bit_1->setCheckState( ( keys.MODIFIER & ( 0x01<<1 ) ) ? Qt::Checked : Qt::Unchecked );
    ui->cb_bit_2->setCheckState( ( keys.MODIFIER & ( 0x01<<2 ) ) ? Qt::Checked : Qt::Unchecked );
    ui->cb_bit_3->setCheckState( ( keys.MODIFIER & ( 0x01<<3 ) ) ? Qt::Checked : Qt::Unchecked );
    ui->cb_bit_4->setCheckState( ( keys.MODIFIER & ( 0x01<<4 ) ) ? Qt::Checked : Qt::Unchecked );
    ui->cb_bit_5->setCheckState( ( keys.MODIFIER & ( 0x01<<5 ) ) ? Qt::Checked : Qt::Unchecked );
    ui->cb_bit_6->setCheckState( ( keys.MODIFIER & ( 0x01<<6 ) ) ? Qt::Checked : Qt::Unchecked );
    ui->cb_bit_7->setCheckState( ( keys.MODIFIER & ( 0x01<<7 ) ) ? Qt::Checked : Qt::Unchecked );
}
