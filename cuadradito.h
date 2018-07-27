#ifndef CUADRADITO_H
#define CUADRADITO_H

#include <QLabel>
#include <QColor>
#include <QPoint>

/**
 * @brief The Cuadradito class
 * Es un QLabel al que se le setea un QPixmap para representar el area desde donde se tomaran descriptores
 */
class Cuadradito : public QLabel
{
    Q_OBJECT
public:
    explicit Cuadradito( QWidget * parent = nullptr );
    Cuadradito( const Cuadradito & cuadradito , QWidget * parent = nullptr );

    void configurar(int lado, QColor color, QPoint pos, QString nombre );
    void configurar( int lado, QPixmap miPixmap, QPoint pos, QString nombre );

    int getLado() const;
    void setLado(int value);

    QColor getColor() const;
    void setColor(const QColor &value);

    QPoint getPos() const;
    void setPos(const QPoint &value);

    QPixmap getMiPixmap() const;
    void setMiPixmap(const QPixmap &value);

    QString getNombre() const;
    void setNombre(const QString &value);

    void actualizar();

private:
    int lado;
    QColor color;
    QPoint pos;
    QPixmap miPixmap;
    QString nombre;


signals:

public slots:

protected:
    void wheelEvent(QWheelEvent *event);
};

#endif // CUADRADITO_H
