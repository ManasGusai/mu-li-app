#include "button.h"

void Button::update(Vector2 mouse) {
    hovered = CheckCollisionPointRec(mouse, rect);

    if(hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        pressed = true;
        toggle = !toggle;
    }

    if(pressed && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
        if(hovered && onClick){
            onClick();
        }

        pressed = false;
    }
}

void Button::draw() {
    Color color = hovered ? hovered_color : normal_color;
    DrawRectangleRec(rect, color);
    if(tex) {    
        Rectangle source = {0, 0, (float)tex->width, (float)tex->height};
        DrawTexturePro(*tex, source, rect, {0, 0}, 0.0f, WHITE);
    }
    else {
        DrawText(text.c_str(), rect.x + 10, rect.y + 10, 20, WHITE);
    }
}