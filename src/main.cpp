#include <raylib.h>
#include <iostream>
#include "music_manager.h"
#include "library_json.h"
#include "texture_manager.hpp"
#include "ui.h"
#include "debug.h"

#define WIDTH 800
#define HEIGHT 600

/*
    Task: SO i have to jump to an index .Know if i'm currently in solo songs or in a playlist, which is easy with song source
    and if im in a playlist i have to load the songs around that index using my 3 song loading system and using modulo so i
    don't go outside the playlist unless the user clicks another playlist or song in another playlist.
    I also need to change things so the app works even if there are less than 3 songs.
    My loop is not implemented and shuffle also so i have to implement those
*/

int main() {
    SetTraceLogLevel(LOG_NONE);
    InitWindow(WIDTH, HEIGHT, "Musical");

    InitAudioDevice();
    if(!IsAudioDeviceReady()) {
        return 1;
    }

    TextureManager tex;
    MusicManager m(tex);
    m.load();
    m.play();

    if(debug){
        m.print_songs_data();
        m.print_playlist_data();
    }

    UI ui(m, tex);
    ui.setup();
    
    while(!WindowShouldClose()) {

        Vector2 mouse = GetMousePosition();

        m.update();
        ui.update(mouse);

        if(IsKeyPressed(KEY_RIGHT)) {
            m.skip(10.0f);
        }
        if(IsKeyPressed(KEY_LEFT)) {
            m.skip(-10.0f);  
        }

        BeginDrawing();
        ClearBackground(BLACK);

        ui.draw();

        EndDrawing();
    }

    m.unload();
    tex.unload();
    CloseAudioDevice();
    CloseWindow();

    return 0;
}