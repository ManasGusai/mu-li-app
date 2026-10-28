#pragma once

#include "button.h"
#include "label.h"
#include "music_manager.h"
#include "circle.h"

class UI {
private:
    MusicManager& manager;
    TextureManager& texmanager;
    Rectangle cover;

    Button nextButton;
    Button prevButton;
    Button play_n_pauseButton;
    Button loopButton;

    Label songLabel;

    Rectangle bar;
    Rectangle bar_level;
    float level;
    Circle bar_tip;

    bool pause = false;
public:

    UI(MusicManager& mm, TextureManager& tm);

    void setup();
    void update(Vector2 mouse);

    void draw();

};
