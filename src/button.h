#ifndef BUTTON_H
#define BUTTON_H

#include <raylib.h>
#include <string>
#include <functional>

struct Button {
    Rectangle rect;
    std::string text;

    bool hovered = false;
    bool pressed = false;

    std::function<void()> onClick;

    void update(Vector2 mouse);
    void draw();

};

#endif