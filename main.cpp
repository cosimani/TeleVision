#include "principal.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Principal w;
    w.resize( 1100, 500 );
    w.move( 150, 50 );
    w.show();

    return a.exec();
}
