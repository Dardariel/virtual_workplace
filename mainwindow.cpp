#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QKeyEvent>
#include <QString>

#include <QSerialPort>

#include <VLCQtCore/Common.h>
#include <VLCQtCore/Instance.h>
#include <VLCQtCore/Media.h>
#include <VLCQtCore/MediaPlayer.h>
#include <VLCQtCore/Enums.h>

#include <QMessageBox>

#include <def_struct.h>

#include <QObject>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)    
    , _media(0)
    , m_serial(new QSerialPort(this))
    , key_test( new KeyboardTest(this) )
    , Modifier( 0x00 )
    , reception_status( RECEPTION_STATUS::WAITING_START )
    , ui(new Ui::MainWindow)
    , positionProtocol( 0 )
{
    ui->setupUi(this);

    ui->hs_Power_state->setStyleSheet("QSlider::groove:horizontal {"
                    "border: 1px solid #999999;"
                    "height: 8px;"
                    "background: palette(midlight);"
                    "margin: 8px 0;"
                    "}"
                    "QSlider::handle:horizontal {"
                    "background: palette(window);"
                    "border: 1px solid #999999;"
                    "width: 25px;"
                    "margin: -8px 0;"
                    "border-radius: 3px;"
                    "}");

    _instance = new VlcInstance( VlcCommon::args(), this);
    _player = new VlcMediaPlayer(_instance);
    _player->setVideoWidget(ui->video);
    slotVideoStateChanged();

    connect(ui->pb_connectVideo, &QPushButton::clicked, this, &MainWindow::slotShow);
    connect(_player, &VlcMediaPlayer::stateChanged, this, &MainWindow::slotVideoStateChanged);

    connect(ui->pb_connectSerial, &QPushButton::clicked, this, &MainWindow::connectSerialPort);

    connect(ui->pb_test, &QPushButton::clicked, this, &MainWindow::slotSerialTest);

    connect(m_serial, &QSerialPort::errorOccurred, this, &MainWindow::handleError);
    connect(m_serial, &QSerialPort::readyRead, this, &MainWindow::slotReadSerialData);

    connect(ui->actionkeyboard_test, &QAction::triggered, key_test, &QDialog::show);

    // for test
    ui->te_log->setMaximumHeight(5000);
}

void MainWindow::slotLog( QString log )
{
    ui->te_log->append( log );
}

void MainWindow::slotKeyboardEvent( uint8_t key, bool press  )
{
    keyProc( key, press );
}

MainWindow::~MainWindow()
{
    delete _player;
    delete _media;
    delete _instance;

    delete ui;

    closeSerialPort();
    delete m_serial;

    delete key_test;
}

void MainWindow::slotShow()
{
    if(_player->state() != Vlc::State::Idle )
    {
        _player->stop();
        delete _media;
    }

    _media = new VlcMedia("v4l2:///dev/video0", _instance); //  вынести в конфиг
    _media->setOption(":v4l2-standard= :live-caching=5"); //  вынести в конфиг

    _player->open(_media);

    ui->pb_connectVideo->setText("reconnect");
}
void MainWindow::slotVideoStateChanged()
{
    switch( _player->state() )
    {
        case Vlc::State::Idle:
            ui->l_stateVideo->setText("Idle");
            break;
        case Vlc::State::Opening:
            ui->l_stateVideo->setText("Opening");
            break;
        case Vlc::State::Buffering:
            ui->l_stateVideo->setText("Buffering");
            break;
        case Vlc::State::Playing:
            ui->l_stateVideo->setText("Playing");
            break;
        case Vlc::State::Paused:
            ui->l_stateVideo->setText("Paused");
            break;
        case Vlc::State::Stopped:
            ui->l_stateVideo->setText("Stopped");
            break;
        case Vlc::State::Ended:
            ui->l_stateVideo->setText("Ended");
            break;
        case Vlc::State::Error:
            ui->l_stateVideo->setText("Error");
            break;
        default:
            ui->l_stateVideo->setText("Unknown state");
            break;
    }
}


void MainWindow::connectSerialPort()
{
    m_serial->setPortName( "/dev/ttyUSB0" );  //  вынести в конфиг
    m_serial->setBaudRate( QSerialPort::Baud115200 );
    m_serial->setDataBits( QSerialPort::DataBits::Data8 );
    m_serial->setParity( QSerialPort::Parity::NoParity );
    m_serial->setStopBits( QSerialPort::StopBits::OneStop );
    m_serial->setFlowControl( QSerialPort::FlowControl::NoFlowControl );

    if (m_serial->open(QIODevice::ReadWrite))
    {
            ui->l_stateSerial->setText( "Connected" );
    }
    else
    {
            QMessageBox::critical(this, tr("Error"), m_serial->errorString());
            ui->l_stateSerial->setText( "Open error" );
    }
}

void MainWindow::closeSerialPort()
{
    if (m_serial->isOpen())
    {
            m_serial->close();
    }

    ui->l_stateSerial->setText( "Disconnected" );
}


void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
            QMessageBox::critical(this, tr("Critical Error"), m_serial->errorString());
            closeSerialPort();
    }
}

void MainWindow::keyProc( uint8_t key, bool press )
{
    ui->te_log->append( ( press ? "Press " : "Release " ) + QString("%1").arg( key ) );

    keyboardHID keys;
    keys.RESERVED = 0x00;


    switch( key )
    {
        case KEY_RIGHT_ALT:
        {
            if( press )
            {
                Modifier |= 0x01<<6;
            }
            else
            {
                Modifier &= ~( 0x01<<6 );
            }
            break;
        }
        case KEY_RIGHT_SHIFT:
        {
            if( press )
            {
                Modifier |= 0x01<<5;
            }
            else
            {
                Modifier &= ~( 0x01<<5 );
            }
            break;
        }
        case KEY_RIGHT_CTRL:
        {
            if( press )
            {
                Modifier |= 0x01<<4;
            }
            else
            {
                Modifier &= ~( 0x01<<4 );
            }
            break;
        }
        case KEY_LEFT_ALT:
        {
            if( press )
            {
                Modifier |= 0x01<<2;
            }
            else
            {
                Modifier &= ~( 0x01<<2 );
            }
            break;
        }
        case KEY_LEFT_SHIFT:
        {
            if( press )
            {
                Modifier |= 0x01<<1;
            }
            else
            {
                Modifier &= ~( 0x01<<1 );
            }
            break;
        }
        case KEY_LEFT_CTRL:
        {
            if( press )
            {
                Modifier |= 0x01<<0;
            }
            else
            {
                Modifier &= ~( 0x01<<0 );
            }
            break;
        }
        default:
        {
            if( press )
            {
                if( listKeys.size() == 6 )
                {
                    ui->te_log->append("max keys 6");
                }
                else
                {
                    listKeys.append( key );
                }
            }
            else
            {

                for( int i=0; i < listKeys.size(); i++ )
                {
                    if( listKeys.at(i) == key )
                    {
                        listKeys.removeAt( i );
                        break;
                    }
                }
            }

        }
    }
    keys.MODIFIER = Modifier;
    keys.KEYCODE1= ( listKeys.size() >=1 ) ? listKeys.at(0) : 0x00;
    keys.KEYCODE2= ( listKeys.size() >=2 ) ? listKeys.at(1) : 0x00;
    keys.KEYCODE3= ( listKeys.size() >=3 ) ? listKeys.at(2) : 0x00;
    keys.KEYCODE4= ( listKeys.size() >=4 ) ? listKeys.at(3) : 0x00;
    keys.KEYCODE5= ( listKeys.size() >=5 ) ? listKeys.at(4) : 0x00;
    keys.KEYCODE6= ( listKeys.size() >=6 ) ? listKeys.at(5) : 0x00;

    //ui->te_log->append( QString("MODIFIER = %1  -  %2").arg( keys.MODIFIER ).arg( keys.MODIFIER & ( 0x01<<0 ) ) );

    key_test->setStruct( keys );

    // send struct to controller

    QList<uint8_t> data;
    data.append(Modifier);
    for(int i=0; i<7; i++)
    {
            if(i<listKeys.size())
            {
                data.append(listKeys.at(i));
            }
            else
            {
                data.append(0x00);
            }
    }
    // добить до 7
    sendData(TypesSend::KEYBOARD, data );
}

void MainWindow::slotSerialTest()
{
    QList< uint8_t > data;

    data.append(0x01<<5);
    data.append(0x41);
    data.append(0x42);
    data.append(0x43);
    data.append(0x44);
    data.append(0x45);
    data.append(0x46);
    sendData(TypesSend::KEYBOARD, data);

}


void MainWindow::sendData(TypesSend type, QList< uint8_t > &data)
{
    if(!m_serial->isOpen())
    {
        return;
    }
    uint i=0;
    QByteArray pack;
    pack[i++]=SOH;
    pack[i++]=STX;
    pack[i++]=static_cast<char>(type);
    pack[i++]=static_cast<char>(data.size());
    for(auto it : data )
    {
        pack[i++]=it;
    }
    pack[i++]=crcCalc( pack.data()+4, data.size() );
    pack[i++]=ETX;
    pack[i++]=EOT;
    ui->te_log->append( "write data = " + pack.toHex() + " len=" + QString::number( pack.size() )  );
    m_serial->write( pack.data(), pack.size() );
}

char MainWindow::crcCalc(char *data, uint8_t size)
{
    char crc = 0x00;
    for(int i=0; i<size; i++)
    {
        crc+=*(data+i)*211;
    }
    return crc;
}

void MainWindow::slotReadSerialData()
{
    bufProtocol.append( m_serial->readAll() );

    int watchdog=300;

    //positionProtocol
    int len=2;

    while( !bufProtocol.isEmpty() )
    {
        switch( reception_status )
        {
        case RECEPTION_STATUS::WAITING_START:
        case RECEPTION_STATUS::FIRST_START_BIT_ACCEPTED:
        case RECEPTION_STATUS::FIRST_END_BIT_RECEIVED:
            if( bufProtocol.at( positionProtocol ) == SOH )
            {
                    positionProtocol++;
                    reception_status=RECEPTION_STATUS::FIRST_START_BIT_ACCEPTED;
            }
            else if( bufProtocol.at( positionProtocol ) == STX )
            {
                    positionProtocol++;
                    reception_status=RECEPTION_STATUS::SECOND_START_BIT_ACCEPTED;
            }
            else if( bufProtocol.at( positionProtocol ) == EOT )
            {

                    ui->te_log->append( "read data = " + bufProtocol.toHex()  );
                    // processing

                    bufProtocol.remove( 0, 1+positionProtocol );
                    positionProtocol=0;
                    reception_status=RECEPTION_STATUS::WAITING_START;
                    return;
            }
            else
            {
                    bufProtocol.remove( 0, 1+positionProtocol );
                    reception_status=RECEPTION_STATUS::WAITING_START;
                    positionProtocol=0;
            }
            break;
        case RECEPTION_STATUS::SECOND_START_BIT_ACCEPTED:
            if( positionProtocol > 3 )
            {
                    len=2+bufProtocol.at( 3 );
                    if( positionProtocol == ( len + 3 ) )
                    {
                        if( bufProtocol.at( positionProtocol ) == ETX )
                        {
                            reception_status=RECEPTION_STATUS::FIRST_END_BIT_RECEIVED;
                            positionProtocol++;
                        }
                    }
                    else
                    {
                        positionProtocol++;
                    }
            }
            else
            {
                positionProtocol++;
            }

            break;
        default:
            reception_status=RECEPTION_STATUS::WAITING_START;
            bufProtocol.clear();
        }
        if( !watchdog--)
        {
            return;
            reception_status=RECEPTION_STATUS::WAITING_START;
            bufProtocol.clear();
        }
    }
}
