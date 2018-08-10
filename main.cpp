#include "principal.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Principal w;
    w.resize( 1200, 600 );
    w.move( 100, 50 );
    w.showMaximized();

    return a.exec();
}
