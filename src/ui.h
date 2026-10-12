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

    UI(MusicManager& mm, TextureManager& tm) : manager(mm), texmanager(tm)
    {
        bar = {230.0f, 465.0f, 350.0f, 10.0f};
        bar_tip = {bar.x , bar.y + (bar.height / 2.0f), 10.0f};

        nextButton.rect = {600.0f, 500.0f, 70.0f, 40.0f};
        nextButton.text = "next";

        prevButton.rect = {100.0f, 500.0f, 70.0f, 40.0f};
        prevButton.text = "prev";

        play_n_pauseButton.rect = {350.0f, 500.0f, 40.0f, 40.0f};
        play_n_pauseButton.text = "p"; 
        play_n_pauseButton.tex = &texmanager.get("images/icons/pause.png");
        play_n_pauseButton.set_normal_Color(BLACK);

        loopButton.rect = {10.0f, 10.0f, 40.0f, 40.0f};
        loopButton.text = "Loop";
        loopButton.tex = &texmanager.get("images/icons/loopu.png");
        loopButton.set_normal_Color(BLACK);

        cover = {150.0f, 10.0f, 450.0f, 400.0f};

        songLabel.setText(manager.get_current_song());
        songLabel.setPosition({220
        ,prevButton.rect.y - 80.0f});
        songLabel.setFontSize(20);
        songLabel.setColor(MAGENTA);
        songLabel.enableSliding(28, 8, 0.25f);
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

        play_n_pauseButton.onClick = [this]()
        {
            if(play_n_pauseButton.toggle){
                manager.pause();
                pause = true;
                play_n_pauseButton.tex = &texmanager.get("images/icons/play.png");
            }else
            {
                manager.resume();
                pause = false;
                play_n_pauseButton.tex = &texmanager.get("images/icons/pause.png");
            }
        };

        loopButton.onClick = [this]()
        {
            // loopButton.set_normal_Color(LIGHTGRAY);
            if(loopButton.toggle)
            {
                manager.change_mode(PLAYING_MODE::LOOP);
                loopButton.tex = &texmanager.get("images/icons/loopp.png");
            }else
            {
                manager.change_mode(PLAYING_MODE::NORMAL);
                loopButton.tex = &texmanager.get("images/icons/loopu.png");
            }
        };
    }

    void update(Vector2 mouse) {
        level = (manager.song_currently_at() / manager.total_song_duration()) * bar.width  ;
        bar_level = {bar.x , bar.y + 1.0f, level, bar.height - 2.0f};
        bar_tip.cx = bar.x + bar_level.width;

        if(CheckCollisionPointRec(mouse, bar))
        {
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                float x = (mouse.x - bar.x) / bar.width;
                // std::cout << x << std::endl;
                float time = x * manager.total_song_duration();
                // std::cout << manager.total_song_duration() << " " << time << std::endl;
                manager.move_song_to(time);
            }
        }

        if(IsKeyPressed(KEY_SPACE)){
            pause = !pause;
            play_n_pauseButton.toggle = !play_n_pauseButton.toggle;
            manager.pause_song(pause);
        }
        
        if(manager.song_paused())
        {
            play_n_pauseButton.tex = &texmanager.get("images/icons/play.png");
            manager.pause();
        }else
        {
            play_n_pauseButton.tex = &texmanager.get("images/icons/pause.png");
            manager.resume();
        }
        nextButton.update(mouse);
        prevButton.update(mouse);
        play_n_pauseButton.update(mouse);
        loopButton.update(mouse);

        songLabel.setText(manager.get_current_song());
        songLabel.update();
        
    }

    void draw() {
        DrawRectangleRec(cover, BLUE);
        DrawTexturePro(manager.get_current_song_texture(),{0,0, (float)manager.get_current_song_texture().width,
            (float)manager.get_current_song_texture().height},
            cover,{0,0}, 0.0f, WHITE);

        songLabel.draw();
            
        nextButton.draw();
        prevButton.draw();
        play_n_pauseButton.draw();
        loopButton.draw();

        DrawRectangleRec(bar, WHITE);
        DrawRectangleRec(bar_level, RED);
        bar_tip.draw();

        int curr_min   = (int)manager.song_currently_at() / 60;
        int curr_sec   = (int)manager.song_currently_at() % 60;

        int length_min = (int)manager.total_song_duration() / 60;
        int length_sec = (int)manager.total_song_duration() % 60;

        DrawText(TextFormat("%02d:%02d : %02d:%02d", curr_min, curr_sec, length_min,
            length_sec),
            prevButton.rect.x - 10.0f
            , static_cast<int>(prevButton.rect.y) - 40, 20, WHITE);
    }

};
