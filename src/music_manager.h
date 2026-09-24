#pragma once
#include <vector>
#include <unordered_map>
#include "song.h"
#include "texture_manager.hpp"

class MusicManager {
private:
    std::vector<Music> songs;
    int current_playing_song = 0;
    std::unordered_map<int, Song> loaded_songs;
    TextureManager& texmanager;

    void load_songs();

public:
    MusicManager(TextureManager& tm) : texmanager(tm) {}
    void load();
    void update();
    void next();
    void previous();
    
    void play();
    void pause();
    void resume();

    std::string get_current_song();
    Texture2D& get_current_song_texture() {
        return  texmanager.get("cover/" + loaded_songs[current_playing_song + 1].icon);
    }

    void print_songs_data();
};