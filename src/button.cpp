#include "button.h"

void Button::update(Vector2 mouse) {
    hovered = CheckCollisionPointRec(mouse, rect);

    if(hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        pressed = true;
    }

    if(pressed && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
        if(hovered && onClick){
            onClick();
        }

        pressed = false;
    }
}

void Button::draw() {
    Color color = hovered ? DARKGRAY : GRAY;

    DrawRectangleRec(rect, color);
    DrawText(text.c_str(), rect.x + 10, rect.y + 10, 20, WHITE);
}