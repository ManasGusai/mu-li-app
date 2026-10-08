#include <raylib.h>
#include <iostream>
#include "music_manager.h"

#define WIDTH 800
#define HEIGHT 600

int main() {

    // SetTraceLogLevel(LOG_NONE);
    InitWindow(WIDTH, HEIGHT, "Musical");

    InitAudioDevice();
    if(!IsAudioDeviceReady()) {
        return 1;
    }

    TextureManager tex;
    MusicManager m(tex);
    m.load();
    m.play();
    //m.print_songs_data();

    
    
    while(!WindowShouldClose()) {

        m.update();

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            m.next();
            m.play();
        }
        if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)){
            m.previous();
            m.play();
        }

        if(IsKeyPressed(KEY_RIGHT)) {
            m.skip(10.0f);
        }
        if(IsKeyPressed(KEY_LEFT)) {
            m.skip(-10.0f);  
        }

        BeginDrawing();
        ClearBackground(BLACK);

        DrawTexturePro(m.get_current_song_texture(),{0,0, (float)m.get_current_song_texture().width,
            (float)m.get_current_song_texture().height},
            {0,0, WIDTH, HEIGHT},{0,0}, 0.0f, WHITE);

        DrawText(m.get_current_song().c_str(), 10, 10, 20, WHITE);

        int curr_min   = (int)m.song_currently_at() / 60;
        int curr_sec   = (int)m.song_currently_at() % 60;

        int length_min = (int)m.total_song_duration() / 60;
        int length_sec = (int)m.total_song_duration() % 60;

        DrawText(TextFormat("%02d:%02d / %02d:%02d", curr_min, curr_sec, length_min,
            length_sec), 30, 130, 30, WHITE);

        EndDrawing();
    }

    m.unload();
    CloseAudioDevice();
    CloseWindow();

    return 0;
}