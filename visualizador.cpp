#include "visualizador.h"
#include "ui_visualizador.h"
#include <QPainter>
#include <QDebug>

Visualizador::Visualizador(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Visualizador)
{
    ui->setupUi(this);
}

Visualizador::~Visualizador()
{
    delete ui;
}

void Visualizador::setImagenes(QString fileImOriginal, QString fileImProcesada )
{
    imOriginal.load( fileImOriginal );
    imProcesada.load( fileImProcesada );

    qDebug() << fileImOriginal << fileImProcesada;

    this->repaint();
}


void Visualizador::paintEvent(QPaintEvent *)
{
    QPainter painter( this );

    painter.drawImage( 0, 0, imOriginal.scaled( this->width() / 2, this->height() ) );
    painter.drawImage( this->width() / 2, 0, imProcesada.scaled( this->width() / 2, this->height() ) );
}
