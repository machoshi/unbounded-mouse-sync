#ifndef MYSHADE_H
#define MYSHADE_H

#include <QWidget>
#include<QDialog>
#include<QMouseEvent>
#include<QPaintEvent>
#include<QPainter>
class MyShade : public QDialog
{
    Q_OBJECT
public:
    explicit MyShade(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);
    int get_scale()const{
        return scale;
    }
    void set_scale(int _scale){
        scale = _scale;
    }
private:
    int scale = 1;
signals:

};

#endif // MYSHADE_H
