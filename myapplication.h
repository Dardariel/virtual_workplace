#ifndef MYAPPLICATION_H
#define MYAPPLICATION_H

#include <QApplication>

#include <QKeyEvent>

class MyApplication : public QApplication
{
    Q_OBJECT

public:
    MyApplication( int& argc, char** argv ) : QApplication( argc, argv ), enableFilterKeyboard( true )
    {
        this->installEventFilter( this );
    }


    bool eventFilter( QObject*, QEvent* event )
    {
        if (event && ( ( event->type() == QEvent::KeyPress )
                     || (event->type() == QEvent::KeyRelease ) ) )
        {
            if( !enableFilterKeyboard )
            {
                return false;
            }
            QKeyEvent* keyEvent = dynamic_cast< QKeyEvent* >( event );
            emit signalKeyboardEvent( keyEvent->nativeScanCode(), ( event->type() == QEvent::KeyPress ) );

            return true;
        }
        return false;
    }

public slots:
    void slotChangeStateFilterKeyboard( bool st )
    {
        enableFilterKeyboard=st;
    }

signals:
    void signalLog( QString );
    void signalKeyboardEvent( uint8_t, bool );
private:
    bool enableFilterKeyboard;
};




#endif // MYAPPLICATION_H
