#include "myshade.h"

MyShade::MyShade(QWidget *parent)
    : QDialog{parent}
{
    setWindowFlag(Qt::FramelessWindowHint);
    setWindowFlag(Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    this->setCursor(Qt::BlankCursor);
}

void MyShade::paintEvent(QPaintEvent*){
    QPainter painter(this);
    painter.fillRect(this->rect(),QColor(0,0,0,0x01));
}
