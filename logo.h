#ifndef LOGO_H
#define LOGO_H

#include <QWidget>
#include <QImage>
#include <QTimer>

namespace Ui {
class Logo;
}

class Logo : public QWidget
{
    Q_OBJECT

public:
    explicit Logo(QWidget *parent = 0);
    ~Logo();

    void setIsRunning(bool value);

protected:
    void paintEvent(QPaintEvent *);

private:
    Ui::Logo *ui;
    QImage imLogo, imWait;
    bool isRunning;
    QTimer timer;
    int rotacion;

};

#endif // LOGO_H
