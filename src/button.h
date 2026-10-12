#ifndef BUTTON_H
#define BUTTON_H

#include <raylib.h>
#include <string>
#include <functional>

struct Button {
    Rectangle rect;
    std::string text;
    Color hovered_color = DARKGRAY;
    Color normal_color = GRAY;
    Texture2D* tex = nullptr;

    bool hovered = false;
    bool pressed = false;
    bool toggle = false;

    std::function<void()> onClick;

    void update(Vector2 mouse);
    void draw();
    void set_normal_Color(Color color) { normal_color = color; }
    void set_hovered_color(Color color) { hovered_color = color; }

};

#endif