#ifndef VISUALIZADOR_H
#define VISUALIZADOR_H

#include <QDialog>
#include <QImage>

namespace Ui {
class Visualizador;
}

class Visualizador : public QDialog
{
    Q_OBJECT

public:
    explicit Visualizador(QWidget *parent = 0);
    ~Visualizador();

    void setImagenes(QString fileImOriginal, QString fileImProcesada );

protected:
    void paintEvent( QPaintEvent * );

private:
    Ui::Visualizador *ui;
    QImage imOriginal, imProcesada;
};

#endif // VISUALIZADOR_H
