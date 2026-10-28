#pragma once
#include <vector>
#include <array>
#include <unordered_map>
#include "song.h"
#include "texture_manager.hpp"
#include "library_json.h"

#include <algorithm>

enum class PLAYING_MODE {
    NORMAL,
    LOOP,
    SHUFFLE
};

struct loaded_songs {
    int id;
    Music m;
    bool loaded = false;
};

class MusicManager {
private:
    JLIB jl;

    std::array<loaded_songs, 3> songs;
    std::unordered_map<int, Song> song_data;
    TextureManager& texmanager;
    PLAYING_MODE mode;

    int current_playing_song = 0;
    int next_song_index;
    int last_song_index;

    int prev_index_in_loaded_songs = 0;
    int curr_index_in_loaded_songs = 1;
    int next_index_in_loaded_songs = 2;

    bool paused = false;

    void load_songs();
    void calculate_neighbour_song_indexes();

public:
    MusicManager(TextureManager& tm) : texmanager(tm) {
        mode = PLAYING_MODE::NORMAL;
        jl.initialize();
    }
    void load();
    void unload();
    void update();
    void next();
    void previous();
    void play();
    void pause();
    void resume();
    
    void change_mode(PLAYING_MODE current_mode) {
        mode = current_mode;
    }

    void skip(float time) const {
        SeekMusicStream(songs[curr_index_in_loaded_songs].m, static_cast<int>(
            std::clamp((song_currently_at() + time), 0.0f, total_song_duration() )));
    }

    void move_song_to(float time) const
    {
        SeekMusicStream(songs[curr_index_in_loaded_songs].m, static_cast<int>(
        std::clamp((time), 0.0f, total_song_duration() )));
    }

    float total_song_duration() const {
        return GetMusicTimeLength(songs[curr_index_in_loaded_songs].m);
    }

    float song_currently_at() const {
        return GetMusicTimePlayed(songs[curr_index_in_loaded_songs].m);
    }

    bool song_paused() const { return paused; }
    void pause_song(bool flag) { paused = flag; }

    std::string get_current_song(); 
    Texture2D& get_current_song_texture() {
        return  texmanager.get("images/cover/" + song_data[current_playing_song].icon);
    }

    void print_songs_data();
};