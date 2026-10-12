#ifndef CIRCLE_H
#define CIRCLE_H

#include <raylib.h>

struct Circle
{
    float cx;
    float cy;
    float radius;
    Color color = SKYBLUE;

    void draw()
    {
        DrawCircleV({cx, cy}, radius, color);
    }
};

#endif