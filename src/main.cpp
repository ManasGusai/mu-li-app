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

        BeginDrawing();
        ClearBackground(BLACK);

        DrawTexturePro(m.get_current_song_texture(),{0,0, (float)m.get_current_song_texture().width,
            (float)m.get_current_song_texture().height},
            {0,0, WIDTH, HEIGHT},{0,0}, 0.0f, WHITE);

        DrawText(m.get_current_song().c_str(), 10, 10, 20, WHITE);

        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();

    return 0;
}