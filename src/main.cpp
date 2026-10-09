#include <raylib.h>
#include <iostream>
#include "music_manager.h"

#include "ui.h"

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

    UI ui(m);
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
    CloseAudioDevice();
    CloseWindow();

    return 0;
}