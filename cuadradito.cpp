#include "cuadradito.h"

#include <QPixmap>
#include <QWheelEvent>
#include <QDebug>

Cuadradito::Cuadradito( QWidget * parent ) : QLabel( parent )  {



}

Cuadradito::Cuadradito( const Cuadradito & cuadradito, QWidget * parent ) : QLabel( parent )  {
    this->lado = cuadradito.lado;
    this->color = cuadradito.color;
    this->pos = cuadradito.pos;
    this->nombre = cuadradito.nombre;
    this->configurar( cuadradito.lado, cuadradito.color, cuadradito.pos, cuadradito.nombre );
}

void Cuadradito::configurar( int lado, QColor color, QPoint pos, QString nombre )
{
    this->lado = lado;
    this->color = color;
    this->pos = pos;
    this->nombre = nombre;

    QPixmap pixmap( this->lado, this->lado );
    pixmap.fill( this->color );
    this->miPixmap = pixmap;
    this->setMiPixmap( this->miPixmap );
    this->move( this->pos );
    this->resize( this->miPixmap.size() );  // Lo agregue porque se cambiaba de tamano
    this->setAttribute( Qt::WA_DeleteOnClose );

    this->hide();
}

void Cuadradito::configurar( int lado, QPixmap pixmap, QPoint pos , QString nombre )
{
    this->lado = lado;
    this->pos = pos;
    this->nombre = nombre;

    pixmap.scaled( this->lado, this->lado );
    this->setMiPixmap( pixmap );
    this->move( this->pos );
    this->resize( this->miPixmap.size() );  // Lo agregue porque se cambiaba de tamano
    this->setAttribute( Qt::WA_DeleteOnClose );

    this->show();
}


int Cuadradito::getLado() const
{
    return lado;
}

void Cuadradito::setLado(int value)
{
    lado = value;
}

QColor Cuadradito::getColor() const
{
    return color;
}

void Cuadradito::setColor(const QColor &value)
{
    color = value;
}

QPoint Cuadradito::getPos() const
{
    return pos;
}

void Cuadradito::setPos(const QPoint &value)
{
    pos = value;
}

QPixmap Cuadradito::getMiPixmap() const
{
    return miPixmap;
}

void Cuadradito::setMiPixmap(const QPixmap &value)
{
    miPixmap = value;
    this->setPixmap( miPixmap );
}

QString Cuadradito::getNombre() const
{
    return nombre;
}

void Cuadradito::setNombre(const QString &value)
{
    nombre = value;
}

void Cuadradito::actualizar()
{
    this->configurar( this->lado, this->miPixmap, this->pos, this->nombre );
    this->show();
}

void Cuadradito::wheelEvent( QWheelEvent * event )
{
    QPoint numDegrees = event->angleDelta() / 8;

    if (!numDegrees.isNull()) {
        QPoint numSteps = numDegrees / 15;

        if ( numSteps.y() == 1 )
            this->lado = this->lado + 1;

        if ( numSteps.y() == -1 )
            this->lado = this->lado - 1;

        QPixmap pixmap = this->pixmap()->scaled( this->lado, this->lado );
        this->setMiPixmap( pixmap );
        this->move( this->pos );
        this->resize( this->lado, this->lado );  // Lo agregue porque se cambiaba de tamano
    }

    event->accept();
}
