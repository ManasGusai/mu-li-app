#pragma once

#include "playlist.h"
#include "song.h"

class JLIB{
private:
    std::vector<Playlist> playlists;
    std::vector<Song> song_data;
    int total_songs = 0;

    void generate_song_metadata();
    void check_playlists();
    void print_playlists();
    void make_global_json();
    void make_json_for_playlists();
    bool is_playlist_changed();

public:
    JLIB();
    void initialize();
    std::vector<Song> get_song_data() const
    {
        return song_data;
    }

    std::vector<Playlist> get_playlist_data() const {
        return playlists;
    }

    int get_total_songs() const 
    {
        return total_songs;
    }
};
