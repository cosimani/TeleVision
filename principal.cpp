#include "principal.h"
#include "ui_principal.h"
#include <QFileDialog>
#include <QPainter>
#include <QMouseEvent>
#include <QLabel>
#include <QDebug>
#include <QtWidgets>
#include <QKeyEvent>
#include <QMessageBox>
#include <QDir>
#include <QFileInfo>

Principal::Principal( QWidget * parent ) : QWidget( parent ),
                                           ui( new Ui::Principal ),
                                           pixmapMapa( new PixmapMapa( this ) ),
                                           cClasificador1( new Cuadradito( this ) ),
                                           cClasificador2( new Cuadradito( this ) ),
                                           cClasificador3( new Cuadradito( this ) ),
                                           cClasificador4( new Cuadradito( this ) ),
                                           process( new QProcess( this ) ),
                                           visualizador( new Visualizador( this ) )

{
    ui->setupUi(this);

    rutaImagenActual = "/home/cosimani/Proyecto/2018/CursoFAMAF/TeleVision/television/images/cordoba457_cesar.tif";
    pixmapMapa->cargarImagen( rutaImagenActual );

    connect( ui->pbAbrir, SIGNAL( pressed() ), this, SLOT( slot_abrirImagen() ) );

    connect( ui->cbClasif1, SIGNAL( stateChanged( int ) ), this, SLOT( slot_cbClasificadoresCambiados() ) );
    connect( ui->cbClasif2, SIGNAL( stateChanged( int ) ), this, SLOT( slot_cbClasificadoresCambiados() ) );
    connect( ui->cbClasif3, SIGNAL( stateChanged( int ) ), this, SLOT( slot_cbClasificadoresCambiados() ) );
    connect( ui->cbClasif4, SIGNAL( stateChanged( int ) ), this, SLOT( slot_cbClasificadoresCambiados() ) );

    connect( ui->pbKmeans, SIGNAL( pressed() ), this, SLOT( slot_aplicarKmeans() ) );
    connect( ui->pbSupervisado, SIGNAL( pressed() ), this, SLOT( slot_aplicarSupervisado() ) );

    this->setAcceptDrops( true );

    cClasificador1->configurar( 50, QColor( 0, 0, 255, 150 ), QPoint( 100, 150 ), "cultivo" );

    cClasificador2->configurar( 50, QColor( 255, 0, 255, 150 ), QPoint( 300, 150 ), "ciudad" );

    cClasificador3->configurar( 50, QColor( 255, 0, 0, 150 ), QPoint( 400, 150 ), "agua" );

    cClasificador4->configurar( 50, QColor( 255, 255, 0, 150 ), QPoint( 500, 150 ), "roca" );

    connect( process, SIGNAL( errorOccurred( QProcess::ProcessError ) ),
             this, SLOT( slot_errorEjecutandoScript( QProcess::ProcessError ) ) );

    connect( process, SIGNAL( finished( int, QProcess::ExitStatus ) ),
             this, SLOT( slot_scriptFinalizado( int, QProcess::ExitStatus ) ) );

    connect( process, SIGNAL( started() ), this, SLOT( slot_isStarted() ) );

    connect( process, SIGNAL( readyReadStandardOutput() ), this, SLOT( slot_consola() ) );
    connect( process, SIGNAL( readyReadStandardError() ), this, SLOT( slot_errorDeConsola() ) );





}

Principal::~Principal()  {
    delete ui;
}

void Principal::paintEvent( QPaintEvent * )  {
//    QPainter painter( this );
//    painter.drawImage( 0, 0, im );
}

void Principal::slot_abrirImagen()  {
    QString file = QFileDialog::getOpenFileName( this, "Abri Imagen", "../", "Images (*.png *.jpg *.tif)" );
    pixmapMapa->cargarImagen( file );

    rutaImagenActual = file;

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

void Principal::slot_aplicarKmeans()
{

    if ( ui->leClusters->text().toInt() < 2 || ui->leClusters->text().toInt() > 6 )  {
        QMessageBox::information( this, "Fuera de rango", "La cantidad de clusters disponibles debe ser entre 2 y 6" );
        return;
    }

    // Se deben enviar los siguientes parametros:
    // directorio del proyecto: por ejemplo: /home/cosimani/Proyecto/2018/Curso FAMAF/TeleVisión/TeleVision
    // ruta completa de la imagen: por ejemplo /home/cosimani/pictures/im.png
    // clusters de K-Means
    // donde almacenar la resultante:
    //    por ejemplo: /home/cosimani/Proyecto/2018/Curso FAMAF/TeleVisión/TeleVision/processedImages/unsupervised/im_001.png


    if ( process->state() == QProcess::NotRunning )  {

        QString script = "../television/python/TeleVision_nosupervisado.py";

        QString dirProyecto = "/home/cosimani/Proyecto/2018/CursoFAMAF/TeleVision/television";

        QStringList parametros;
        parametros << script;
        parametros << dirProyecto;
        parametros << rutaImagenActual;
        parametros << ui->leClusters->text();

        // Donde almacenar?

        QDir directory( dirProyecto + "/processedImages/unsupervised" );

        QStringList fileFilter;
        fileFilter << "*.jpg" << "*.png" << "*.bmp" << "*.gif" << "*.tif";
        QStringList imageFiles = directory.entryList( fileFilter );

        int ultimoNumero = 0;

        for ( int i = 0; i < imageFiles.size(); i++ )
        {
            QFileInfo info( imageFiles.at( i ) );
            QString nombreArchivo = info.baseName();  // nombre sin extension
            int comienzoNumero = nombreArchivo.lastIndexOf( "_" ) + 1;
            int numero = nombreArchivo.mid( comienzoNumero, nombreArchivo.size() ).toInt();

            if ( ultimoNumero <= numero )
                ultimoNumero = numero;
        }

        QString sNumero = QString::number( ++ultimoNumero ).rightJustified(4, '0');

        QFileInfo infoRutaImagenActual( rutaImagenActual );

        // Hacemos esta copia para poder mostrar la imagen original en el Visualizador, ya que en slot_consola se setea
        // como imagen actual a la que se acaba de procesar.
        rutaImagenOriginal = rutaImagenActual;

        rutaImagenActualProcesada = dirProyecto + "/processedImages/unsupervised/" +
                                    infoRutaImagenActual.baseName() + "_" + sNumero + ".jpg";

        parametros << rutaImagenActualProcesada;

        process->start( "python3", parametros );
    }
    else
    {
        QStringList argumentos = process->arguments();
        QString mensaje;

        if ( ! argumentos.isEmpty() )
        mensaje = "Es el siguiente: \n " + process->arguments().at( 0 );

        QMessageBox::critical( this, "Proceso en ejecucion", "Aun no ha finalizado un script ejecutado \n"
                                     "anteriormente. " + mensaje );
    }


}

void Principal::slot_aplicarSupervisado()
{
    int cantidadClases = 0;

    if ( ui->cbClasif1->isChecked() )
        cantidadClases++;
    if ( ui->cbClasif2->isChecked() )
        cantidadClases++;
    if ( ui->cbClasif3->isChecked() )
        cantidadClases++;
    if ( ui->cbClasif4->isChecked() )
        cantidadClases++;

    if ( cantidadClases < 2 )  {
        QMessageBox::information( this, "Fuera de rango", "La cantidad de clases debe ser entre 2 y 4" );
        return;
    }

    // Se deben enviar los siguientes parametros:
    // directorio del proyecto: por ejemplo: /home/cosimani/Proyecto/2018/Curso FAMAF/TeleVisión/TeleVision
    // ruta completa de la imagen: por ejemplo /home/cosimani/pictures/im.png
    // clases definidad, es decir, cuantas muestras se definieron
    // donde almacenar la resultante:
    //    por ejemplo: /home/cosimani/Proyecto/2018/Curso FAMAF/TeleVisión/TeleVision/processedImages/unsupervised/im_001.png
    // Los demas parametros son>
    //   x1 clasif1  x2 clasif1  y1 clasif1  y2 clasif1
    //   x1 clasif2  x2 clasif2  y1 clasif2  y2 clasif2
    //   x1 clasif3  x2 clasif3  y1 clasif3  y2 clasif3
    //   x1 clasif4  x2 clasif4  y1 clasif4  y2 clasif4

    if ( process->state() == QProcess::NotRunning )  {

        QString script = "../television/python/TeleVision_supervisado.py";

        QString dirProyecto = "/home/cosimani/Proyecto/2018/CursoFAMAF/TeleVision/television";

        QStringList parametros;
        parametros << script;
        parametros << dirProyecto;
        parametros << rutaImagenActual;
        parametros << QString::number( cantidadClases );


        // Donde almacenar?

        QDir directory( dirProyecto + "/processedImages/supervised" );

        QStringList fileFilter;
        fileFilter << "*.jpg" << "*.png" << "*.bmp" << "*.gif" << "*.tif";
        QStringList imageFiles = directory.entryList( fileFilter );

        int ultimoNumero = 0;

        for ( int i = 0; i < imageFiles.size(); i++ )
        {
            QFileInfo info( imageFiles.at( i ) );
            QString nombreArchivo = info.baseName();  // nombre sin extension
            int comienzoNumero = nombreArchivo.lastIndexOf( "_" ) + 1;
            int numero = nombreArchivo.mid( comienzoNumero, nombreArchivo.size() ).toInt();

            if ( ultimoNumero <= numero )
                ultimoNumero = numero;
        }

        QString sNumero = QString::number( ++ultimoNumero ).rightJustified(4, '0');

        QFileInfo infoRutaImagenActual( rutaImagenActual );

        // Hacemos esta copia para poder mostrar la imagen original en el Visualizador, ya que en slot_consola se setea
        // como imagen actual a la que se acaba de procesar.
        rutaImagenOriginal = rutaImagenActual;

        rutaImagenActualProcesada = dirProyecto + "/processedImages/supervised/" +
                                    infoRutaImagenActual.baseName() + "_" + sNumero + ".jpg";

        parametros << rutaImagenActualProcesada;


        // Aca se calcular los x1 x2 y1 y2 ...

        int x1Clasif1 = cClasificador1->getPos().x();
        int x2Clasif1 = x1Clasif1 + cClasificador1->getLado();
        int y1Clasif1 = cClasificador1->getPos().y();
        int y2Clasif1 = y1Clasif1 + cClasificador1->getLado();

        int x1Clasif2 = cClasificador2->getPos().x();
        int x2Clasif2 = x1Clasif2 + cClasificador2->getLado();
        int y1Clasif2 = cClasificador2->getPos().y();
        int y2Clasif2 = y1Clasif2 + cClasificador2->getLado();

        int x1Clasif3 = cClasificador3->getPos().x();
        int x2Clasif3 = x1Clasif3 + cClasificador3->getLado();
        int y1Clasif3 = cClasificador3->getPos().y();
        int y2Clasif3 = y1Clasif3 + cClasificador3->getLado();

        int x1Clasif4 = cClasificador4->getPos().x();
        int x2Clasif4 = x1Clasif4 + cClasificador4->getLado();
        int y1Clasif4 = cClasificador4->getPos().y();
        int y2Clasif4 = y1Clasif4 + cClasificador4->getLado();

        parametros << QString::number( x1Clasif1 );
        parametros << QString::number( x2Clasif1 );
        parametros << QString::number( y1Clasif1 );
        parametros << QString::number( y2Clasif1 );

        parametros << QString::number( x1Clasif2 );
        parametros << QString::number( x2Clasif2 );
        parametros << QString::number( y1Clasif2 );
        parametros << QString::number( y2Clasif2 );

        parametros << QString::number( x1Clasif3 );
        parametros << QString::number( x2Clasif3 );
        parametros << QString::number( y1Clasif3 );
        parametros << QString::number( y2Clasif3 );

        parametros << QString::number( x1Clasif4 );
        parametros << QString::number( x2Clasif4 );
        parametros << QString::number( y1Clasif4 );
        parametros << QString::number( y2Clasif4 );

        process->start( "python3", parametros );
    }
    else
    {
        QStringList argumentos = process->arguments();
        QString mensaje;

        if ( ! argumentos.isEmpty() )
        mensaje = "Es el siguiente: \n " + process->arguments().at( 0 );

        QMessageBox::critical( this, "Proceso en ejecucion", "Aun no ha finalizado un script ejecutado \n"
                                     "anteriormente. " + mensaje );
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


void Principal::slot_errorEjecutandoScript(QProcess::ProcessError error)
{
    QString mensajeError;

    switch(error)  {
    case QProcess::FailedToStart:  mensajeError = "Script not found, resource error";  break;
    case QProcess::Crashed:  mensajeError = "Crashed";  break;
    case QProcess::Timedout:  mensajeError = "Timeout";  break;
    case QProcess::ReadError:  mensajeError = "Read Error";  break;
    case QProcess::WriteError:  mensajeError = "Write Error";  break;
    case QProcess::UnknownError:  mensajeError = "Error desconocido";  break;
    default:;
    }

    QMessageBox::critical(this, "Mensaje de Error", "El mensaje de error con la ejecucion del \n"
                                                    "Script en python3 es: " + mensajeError);
}

// Esta metodo lee todo lo que esta mostrando la consola de python.
void Principal::slot_consola()
{
    QByteArray ba = process->readAllStandardOutput();
    qDebug() << ba.trimmed();

    pixmapMapa->cargarImagen( ba.trimmed() );

    rutaImagenActual = ba.trimmed();
}

void Principal::slot_errorDeConsola()
{
    qDebug() << "Errores en Consola";
    QByteArray ba = process->readAllStandardError();

    qDebug() << "slot_errorDeConsola" << ba;
}

void Principal::slot_scriptFinalizado( int, QProcess::ExitStatus exitStatus )
{
    ui->logo->setIsRunning( false );

    if ( exitStatus == QProcess::CrashExit )  {
        QMessageBox::critical(this, "Mensaje de Error", "El script finalizo con el siguiente mensaje: Crashed");
        return;
    }

    visualizador->setImagenes( rutaImagenOriginal, rutaImagenActualProcesada );
    visualizador->showMaximized();

//    if ( etapa == NADA && exitStatus == QProcess::NormalExit )
//        emit signal_nuevaEtapa( DATA_PREPARATION_LISTO );
//    else if ( etapa == DATA_PREPARATION_LISTO && exitStatus == QProcess::NormalExit )
//        emit signal_nuevaEtapa( UNSUPERVISED_DICTIONARY_LEARNING_LISTO );
//    else if ( etapa == UNSUPERVISED_DICTIONARY_LEARNING_LISTO && exitStatus == QProcess::NormalExit )
//        emit signal_nuevaEtapa( COMPUTE_BoVW_VECTORS_LISTO );
//    else if ( etapa == COMPUTE_BoVW_VECTORS_LISTO && exitStatus == QProcess::NormalExit )
//        emit signal_nuevaEtapa( TRAIN_CLASSIFIERS_LISTO );
}

void Principal::slot_isStarted()
{
    ui->logo->setIsRunning(true);
}

