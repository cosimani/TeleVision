#include "logo.h"
#include "ui_logo.h"
#include <QPainter>

Logo::Logo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Logo),
    isRunning( false ), rotacion( 0 )
{
    ui->setupUi(this);

    imLogo.load("../television/images/logo.jpg");
    imWait.load("../television/images/wait.png");

    timer.setInterval(80);

    connect(&timer, SIGNAL(timeout()), this, SLOT(repaint()));
}

Logo::~Logo()
{
    delete ui;
}

void Logo::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QImage imCopia;

    if ( isRunning )  {

        painter.translate(this->width()/2, this->height()/2);

        painter.rotate(rotacion += 10);

        if ( rotacion >= 360 )
            rotacion = 0;

        imCopia = imWait.scaledToWidth(this->width()/2, Qt::SmoothTransformation );
        painter.drawImage(-imCopia.width()/2, -imCopia.height()/2, imCopia);
    }
    else  {
        imCopia = imLogo.scaledToWidth(this->width(), Qt::SmoothTransformation );
        painter.drawImage(this->width()/2 - imCopia.width()/2, this->height()/2 - imCopia.height()/2, imCopia);
    }

}

void Logo::setIsRunning(bool value)
{
    isRunning = value;

    if ( isRunning )
        timer.start();
    else
        timer.stop();


    this->repaint();
}

