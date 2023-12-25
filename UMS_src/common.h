#ifndef COMMON_H
#define COMMON_H
#include"windows.h"
#include<QPoint>
#include<QCursor>
#include<QRect>
struct MouseInput{
    int flag;
    QRect Screen;
    bool Cur_pc;
    INPUT input;
    QPoint point;
};

#endif // COMMON_H
