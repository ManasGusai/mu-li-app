#pragma once
#include <raylib.h>
#include <string>
#include <chrono>

class Label
{
private:
    std::string text;

    Vector2 position;
    int fontSize;
    Color color;

    //Sliding vars
    bool sliding = false;
    int gap = 12;
    int visibleChars = 16;
    float slideInterval = 0.25f;

    int startIndex = 0;

    std::chrono::steady_clock::time_point last = std::chrono::steady_clock::now();

public:
    void setText(std::string message);
    void setPosition(Vector2 pos);
    void setFontSize(int fs);
    void setColor(Color c);

    void enableSliding(int visibleChars = 20, int gap = 5, float interval = 0.25f);
    void update();
    void draw() const;

};