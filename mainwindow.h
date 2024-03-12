#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>

#include <keyboardtest.h>

#include <QList>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QSerialPort;

class VlcInstance;
class VlcMedia;
class VlcMediaPlayer;


#define SOH 0x01
#define STX 0x02
#define ETX 0x03
#define EOT 0x04

#define ACK 0x06
#define NAK 0x15

// протокол
// SOH <type> <len> STX <data...> <ETX> <crc> <EOT>
// type - тип пакета устройство, клавиатура
// len - длинна пакета
// data... - информационный блок пакета длинной len
// crc - rjynhjkmyfz cevvf dsta...



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    enum class TypesSend : char
    {
        DEVICE = 0,
        KEYBOARD = 1
    };

    QSerialPort *m_serial;

    VlcInstance *_instance;
    VlcMedia *_media;
    VlcMediaPlayer *_player;

    KeyboardTest *key_test;

    QList< uint8_t > listKeys;
    uint8_t Modifier;

    void keyProc( uint8_t key, bool press );

public slots:
    void slotShow();
    void slotVideoStateChanged();

    void connectSerialPort();
    void closeSerialPort();
    void handleError(QSerialPort::SerialPortError error);

    void slotSerialTest();

    void slotReadSerialData();

    void slotKeyboardEvent(uint8_t key , bool press);

    void slotLog( QString log );
signals:
    void signalChangeStateFilterKeyboard( bool );
private:

    void sendData(TypesSend type, QList< uint8_t > &data);

    char crcCalc(char *data, uint8_t size);

    Ui::MainWindow *ui;

    enum RECEPTION_STATUS
    {
        WAITING_START=0,
        FIRST_START_BIT_ACCEPTED=1,
        SECOND_START_BIT_ACCEPTED=2,
        FIRST_END_BIT_RECEIVED=3
    } reception_status;

    QByteArray bufProtocol;
    int positionProtocol;
};
#endif // MAINWINDOW_H
