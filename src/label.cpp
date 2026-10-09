#include "label.h"

void Label::setText(std::string message)
{
    text = message;
}

void Label::setPosition(Vector2 pos)
{
    position = pos;
}

void Label::setFontSize(int fs)
{
    fontSize = fs;
}

void Label::setColor(Color c) 
{
    color = c;
}

void Label::enableSliding(int visibleChars , int gap , float interval)
{
    sliding = true;
    this->visibleChars = visibleChars;
    this->gap = gap;
    slideInterval = interval;
    startIndex = 0;
    last = std::chrono::steady_clock::now();
}

void Label::update()
{
    if(!sliding || text.empty())
        return;
    
    auto now = std::chrono::steady_clock::now();

    if(now - last >= std::chrono::duration<float>(slideInterval))
    {
        int totalLength = text.size() + gap;
        startIndex = (startIndex + 1) % totalLength;
        last = now;
    }
}

void Label::draw() const
{
    if(!sliding || text.empty())
    {
        DrawText(text.c_str(), static_cast<int>(position.x), static_cast<int>(position.y), fontSize, color);
        return;
    }

    std::string displayText;
    int totalLength = text.size() + gap;

    for(int i = 0; i < visibleChars; i++)
    {
        int index = (startIndex + i) % totalLength;

        if(index < static_cast<int>(text.size()))
        {
            displayText += text[index];
        }
        else
        {
            displayText += " ";
        }

        DrawText(displayText.c_str(), static_cast<int>(position.x), static_cast<int>(position.y)
            , fontSize, color);
    }
}