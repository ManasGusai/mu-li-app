#pragma once

#include "button.h"
#include "music_manager.h"

class UI {
private:
    MusicManager& manager;
    Rectangle cover;
public:
    Button nextButton;
    Button prevButton;
    Button pauseButton;

    UI(MusicManager& mm) : manager(mm)
    {
        nextButton.rect = {600.0f, 500.0f, 70.0f, 40.0f};
        nextButton.text = "next";

        prevButton.rect = {100.0f, 500.0f, 70.0f, 40.0f};
        prevButton.text = "prev";

        pauseButton.rect = {350.0f, 500.0f, 70.0f, 40.0f};
        pauseButton.text = "pause"; 

        cover = {150.0f, 10.0f, 450.0f, 400.0f};
    }

    void setup() {
        nextButton.onClick = [this]()
        {
            manager.next();
        };

        prevButton.onClick = [this]()
        {
            manager.previous();
        };

        pauseButton.onClick = [this]()
        {
            manager.pause_n_resume();
        };
    }

    void update(Vector2 mouse) {
        if(IsKeyPressed(KEY_SPACE)){
            manager.pause_n_resume();
        }
        nextButton.update(mouse);
        prevButton.update(mouse);
        pauseButton.update(mouse);
    }

    void draw() {
        DrawRectangleRec(cover, BLUE);
        DrawTexturePro(manager.get_current_song_texture(),{0,0, (float)manager.get_current_song_texture().width,
            (float)manager.get_current_song_texture().height},
            cover,{0,0}, 0.0f, WHITE);

        DrawText(manager.get_current_song().c_str(), prevButton.rect.x + prevButton.rect.width + 20.0f
        ,prevButton.rect.y - 80.0f
        , 20, WHITE);
            
        nextButton.draw();
        prevButton.draw();
        pauseButton.draw();

        int curr_min   = (int)manager.song_currently_at() / 60;
        int curr_sec   = (int)manager.song_currently_at() % 60;

        int length_min = (int)manager.total_song_duration() / 60;
        int length_sec = (int)manager.total_song_duration() % 60;

        DrawText(TextFormat("%02d:%02d : %02d:%02d", curr_min, curr_sec, length_min,
            length_sec),
            prevButton.rect.x + 200.0f
            , static_cast<int>(prevButton.rect.y) - 40, 20, WHITE);
    }

};
