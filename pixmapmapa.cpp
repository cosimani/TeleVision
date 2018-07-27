#include "pixmapmapa.h"

PixmapMapa::PixmapMapa( QWidget * parent ) : QLabel( parent )  {

}

void PixmapMapa::cargarImagen( QString archivo )
{
    this->miPixmap.load( archivo );
    this->setPixmap( this->miPixmap );
    this->resize( this->miPixmap.size() );
    this->move( 0, 0 );
    this->setAttribute( Qt::WA_DeleteOnClose );

}

QPoint PixmapMapa::getPos() const
{
    return pos;
}

void PixmapMapa::setPos(const QPoint &value)
{
    pos = value;
}
