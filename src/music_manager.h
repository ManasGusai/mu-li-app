#pragma once
#include <vector>
#include <array>
#include <unordered_map>
#include "song.h"
#include "texture_manager.hpp"

enum class so {
    prev = 0,
    curr = 1,    
    nxt = 2

};

struct loaded_songs {
    int id;
    Music m;
    bool loaded = false;
};

class MusicManager {
private:
    std::array<loaded_songs, 3> songs;
    std::unordered_map<int, Song> song_data;
    TextureManager& texmanager;

    int current_playing_song = 0;
    int next_song_index;
    int last_song_index;

    void load_songs();
    void load_nxt_current_prev_songs();
    void unload_nxt_and_prev_songs();
    void calculate_neighbour_song_indexes();

public:
    MusicManager(TextureManager& tm) : texmanager(tm) {}
    void load();
    void unload();
    void update();
    void next();
    void previous();
    
    void play();
    void pause();
    void resume();

    std::string get_current_song(); 
    Texture2D& get_current_song_texture() {
        return  texmanager.get("cover/" + song_data[current_playing_song].icon);
    }

    void print_songs_data();
};