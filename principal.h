#ifndef PRINCIPAL_H
#define PRINCIPAL_H

#include <QWidget>
#include <QFrame>
#include "cuadradito.h"
#include "pixmapmapa.h"

QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDropEvent;
QT_END_NAMESPACE


namespace Ui {
class Principal;
}

class Principal : public QWidget
{
    Q_OBJECT

public:
    explicit Principal(QWidget *parent = 0);
    ~Principal();

protected:
    void paintEvent( QPaintEvent * ) override;
    void dragEnterEvent( QDragEnterEvent * event ) override;
    void dropEvent( QDropEvent * event ) override;
    void mousePressEvent( QMouseEvent * event ) override;    

    void keyPressEvent( QKeyEvent * event ) override;

private:
    Ui::Principal *ui;
    PixmapMapa * pixmapMapa;

    Cuadradito * cClasificador1, * cClasificador2, * cClasificador3, * cClasificador4;

private slots:
    void slot_abrirImagen();

    void slot_cbClasificadoresCambiados();
};

#endif // PRINCIPAL_H
