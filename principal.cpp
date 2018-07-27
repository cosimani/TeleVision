#include "principal.h"
#include "ui_principal.h"
#include <QFileDialog>
#include <QPainter>
#include <QMouseEvent>
#include <QLabel>
#include <QDebug>
#include <QtWidgets>
#include <QKeyEvent>

Principal::Principal( QWidget * parent ) : QWidget( parent ),
                                           ui( new Ui::Principal ),
                                           pixmapMapa( new PixmapMapa( this ) ),
                                           cClasificador1( new Cuadradito( this ) ),
                                           cClasificador2( new Cuadradito( this ) ),
                                           cClasificador3( new Cuadradito( this ) ),
                                           cClasificador4( new Cuadradito( this ) )

{
    ui->setupUi(this);

    pixmapMapa->cargarImagen( "../television/images/mapa.png" );

    connect( ui->pbAbrir, SIGNAL( pressed() ), this, SLOT( slot_abrirImagen() ) );

    connect( ui->cbClasif1, SIGNAL( stateChanged( int ) ), this, SLOT( slot_cbClasificadoresCambiados() ) );
    connect( ui->cbClasif2, SIGNAL( stateChanged( int ) ), this, SLOT( slot_cbClasificadoresCambiados() ) );
    connect( ui->cbClasif3, SIGNAL( stateChanged( int ) ), this, SLOT( slot_cbClasificadoresCambiados() ) );
    connect( ui->cbClasif4, SIGNAL( stateChanged( int ) ), this, SLOT( slot_cbClasificadoresCambiados() ) );

    this->setAcceptDrops( true );

    cClasificador1->configurar( 50, QColor( 0, 0, 255, 150 ), QPoint( 100, 150 ), "cultivo" );

    cClasificador2->configurar( 50, QColor( 255, 0, 255, 150 ), QPoint( 300, 150 ), "ciudad" );

    cClasificador3->configurar( 50, QColor( 255, 0, 0, 150 ), QPoint( 400, 150 ), "agua" );

    cClasificador4->configurar( 50, QColor( 255, 255, 0, 150 ), QPoint( 500, 150 ), "roca" );

}

Principal::~Principal()  {
    delete ui;
}

void Principal::paintEvent( QPaintEvent * )  {
//    QPainter painter( this );
//    painter.drawImage( 0, 0, im );
}

void Principal::slot_abrirImagen()  {
    QString file = QFileDialog::getOpenFileName( this, "Abri Imagen", "../", "Images (*.png *.jpg *.tiff)" );
    pixmapMapa->cargarImagen( file );

}

void Principal::slot_cbClasificadoresCambiados()
{
    qDebug() << "slot_cbClasificadoresCambiados";
    if ( ui->cbClasif1->isChecked() )  {
        cClasificador1->show();
        cClasificador1->raise();
    }
    else  {
        cClasificador1->hide();
    }

    if ( ui->cbClasif2->isChecked() )  {
        cClasificador2->show();
        cClasificador2->raise();
    }
    else  {
        cClasificador2->hide();
    }

    if ( ui->cbClasif3->isChecked() )  {
        cClasificador3->show();
        cClasificador3->raise();
    }
    else  {
        cClasificador3->hide();
    }

    if ( ui->cbClasif4->isChecked() )  {
        cClasificador4->show();
        cClasificador4->raise();
    }
    else  {
        cClasificador4->hide();
    }

}

void Principal::dragEnterEvent( QDragEnterEvent * event )
{
    qDebug() << "dragEnterEvent";

    if ( event->mimeData()->hasFormat( "application/x-dnditemdata-mapa" ) )  {
        if ( event->source() == this )  {
            event->setDropAction( Qt::MoveAction );
            event->accept();
        }
        else  {
            event->acceptProposedAction();
        }
    }
    else if ( event->mimeData()->hasFormat( "application/x-dnditemdata" ) )  {
        if ( event->source() == this )  {
            event->setDropAction( Qt::MoveAction );
            event->accept();
        }
        else  {
            event->acceptProposedAction();
        }
    }
    else  {
        event->ignore();
    }
}

// Hace que la imagen que se esta arrastrando se pegue al soltarlo en otro lugar
void Principal::dropEvent( QDropEvent * event )
{
    qDebug() << "dropEvent";

    if ( event->mimeData()->hasFormat( "application/x-dnditemdata-mapa" ) )  {

        // itemData corresponde al binario de la imagen.
        QByteArray itemData = event->mimeData()->data( "application/x-dnditemdata-mapa" );
        QDataStream dataStream( &itemData, QIODevice::ReadOnly );

        QPixmap pixmap;
        QPoint offset;  // Corresponde al pixel de la imagen desde donde se agarra
        QPoint puntoInicial;
        dataStream >> pixmap >> offset >> puntoInicial;

        PixmapMapa * newPixmapMapa = new PixmapMapa( this );
        newPixmapMapa->setPixmap( pixmap );
        newPixmapMapa->resize( pixmap.size() );
        newPixmapMapa->move( event->pos() - offset );
        newPixmapMapa->show();

        pixmapMapa = newPixmapMapa;
        pixmapMapa->setPos( event->pos() - offset );

//        cClasificador1->setPos( cClasificador1->getPos() + pixmapMapa->getPos() );
//        cClasificador2->setPos( cClasificador2->getPos() + pixmapMapa->getPos() );
//        cClasificador3->setPos( cClasificador3->getPos() + pixmapMapa->getPos() );
//        cClasificador4->setPos( cClasificador4->getPos() + pixmapMapa->getPos() );

        if ( ui->cbClasif1->isChecked() )  {
            cClasificador1->setPos( cClasificador1->getPos() + event->pos() - puntoInicial );
            cClasificador1->actualizar();
            cClasificador1->raise();  // Esto lo pone al frente
        }

        if ( ui->cbClasif2->isChecked() )  {
            cClasificador2->setPos( cClasificador2->getPos() + event->pos() - puntoInicial );
            cClasificador2->actualizar();
            cClasificador2->raise();
        }

        if ( ui->cbClasif3->isChecked() )  {
            cClasificador3->setPos( cClasificador3->getPos() + event->pos() - puntoInicial );
            cClasificador3->actualizar();
            cClasificador3->raise();
        }

        if ( ui->cbClasif4->isChecked() )  {
            cClasificador4->setPos( cClasificador4->getPos() + event->pos() - puntoInicial );
            cClasificador4->actualizar();
            cClasificador4->raise();
        }

        if ( event->source() == this )  {
            event->setDropAction( Qt::MoveAction );
            event->accept();
        }
        else  {
            event->acceptProposedAction();
        }
    }
    else if ( event->mimeData()->hasFormat( "application/x-dnditemdata" ) )  {

        // itemData corresponde al binario de la imagen.
        QByteArray itemData = event->mimeData()->data( "application/x-dnditemdata" );
        QDataStream dataStream( &itemData, QIODevice::ReadOnly );

        QPixmap pixmap;
        QPoint offset;  // Corresponde al pixel de la imagen desde donde se agarra
        QString nombre;
        dataStream >> pixmap >> offset >> nombre;

        // Este nuevo icono corresponde a una copia del que se esta moviendo. Entonces, al mover un icono, cuando se
        // suelta el mismo en otro lugar, se crea este newIcon y se borra el anterior (o sea, el que se estaba moviendo).
        Cuadradito * newCuadradito = new Cuadradito( this );
        newCuadradito->configurar( pixmap.size().width(), pixmap, event->pos() - offset, nombre );

        if ( nombre == "cultivo" )  {
            cClasificador1 = newCuadradito;
        }
        if ( nombre == "ciudad" )  {
            cClasificador2 = newCuadradito;
        }
        if ( nombre == "agua" )  {
            cClasificador3 = newCuadradito;
        }
        if ( nombre == "roca" )  {
            cClasificador4 = newCuadradito;
        }

        if ( event->source() == this )  {
            event->setDropAction( Qt::MoveAction );
            event->accept();
        }
        else  {
            event->acceptProposedAction();
        }
    }
    else  {
        event->ignore();
    }
}

//! [1]
void Principal::mousePressEvent( QMouseEvent * event )  {
    qDebug() << "mousePressEvent";

    if( PixmapMapa * pixmapMapaCliqueado = dynamic_cast< PixmapMapa * >( childAt( event->pos() ) ) )  {
        qDebug() << "pixmapMapaCliqueado";
        QPixmap pixmap = *pixmapMapaCliqueado->pixmap();

        QByteArray itemData;
        QDataStream dataStream( &itemData, QIODevice::WriteOnly );
        dataStream << pixmap << QPoint( event->pos() - pixmapMapaCliqueado->getPos() ) << QPoint( event->pos() );
        // Se envia lo siguiente:
        // dataStream << pixmap << offset << puntoInicial;

        QMimeData * mimeData = new QMimeData;
        mimeData->setData( "application/x-dnditemdata-mapa", itemData );

        QDrag *drag = new QDrag( this );
        drag->setMimeData( mimeData );
        drag->setPixmap( pixmap );
        drag->setHotSpot( event->pos() - pixmapMapaCliqueado->getPos() );

        QPixmap tempPixmap = pixmap;
        QPainter painter;
        painter.begin( &tempPixmap );
        painter.fillRect( pixmap.rect(), QColor( 127, 127, 127, 170 ) );
        painter.end();

        pixmapMapaCliqueado->setPixmap( tempPixmap );

        if ( drag->exec( Qt::CopyAction | Qt::MoveAction, Qt::CopyAction ) == Qt::MoveAction )  {
            pixmapMapaCliqueado->close();
        }
        else  {
            pixmapMapaCliqueado->show();
            pixmapMapaCliqueado->setPixmap( pixmap );
        }


    }

    if( Cuadradito * cuadraditoCliqueado = dynamic_cast< Cuadradito * >( childAt( event->pos() ) ) )  {
        QPixmap pixmap = *cuadraditoCliqueado->pixmap();
    //    QPixmap pixmap = child->getMiPixmap();

        QByteArray itemData;
        QDataStream dataStream( &itemData, QIODevice::WriteOnly );
    //    dataStream << pixmap << QPoint( event->pos() - child->pos() );
        dataStream << pixmap << QPoint( event->pos() - cuadraditoCliqueado->getPos() ) << cuadraditoCliqueado->getNombre();
    //! [1]

    //! [2]
        QMimeData * mimeData = new QMimeData;
        mimeData->setData( "application/x-dnditemdata", itemData );
    //! [2]

    //! [3]
        QDrag *drag = new QDrag( this );
        drag->setMimeData( mimeData );
        drag->setPixmap( pixmap );
    //    drag->setHotSpot( event->pos() - child->pos() );
        drag->setHotSpot( event->pos() - cuadraditoCliqueado->getPos() );
    //! [3]

        QPixmap tempPixmap = pixmap;
        QPainter painter;
        painter.begin( &tempPixmap );
        painter.fillRect( pixmap.rect(), QColor( 127, 127, 127, 170 ) );
        painter.end();

        cuadraditoCliqueado->setMiPixmap( tempPixmap );

        if ( drag->exec( Qt::CopyAction | Qt::MoveAction, Qt::CopyAction ) == Qt::MoveAction )  {
            cuadraditoCliqueado->close();
        }
        else  {
            cuadraditoCliqueado->show();
            cuadraditoCliqueado->setMiPixmap( pixmap );
        }
    }
}


void Principal::keyPressEvent( QKeyEvent * event )
{
    Q_UNUSED( event );
    cClasificador1->setPos( QPoint( 100, 100 ) );
    cClasificador1->actualizar();

}
