#ifndef PIXMAPMAPA_H
#define PIXMAPMAPA_H

#include <QLabel>
#include <QPixmap>

class PixmapMapa : public QLabel
{
    Q_OBJECT
public:
    explicit PixmapMapa( QWidget * parent = nullptr );

    void cargarImagen( QString archivo );

    QPoint getPos() const;
    void setPos(const QPoint &value);

private:
    QPixmap miPixmap;
    QPoint pos;
};

#endif // PIXMAPMAPA_H
