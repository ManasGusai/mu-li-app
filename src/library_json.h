#pragma once

#include <vector>
#include <string>

class JLIB{
private:
    std::vector<std::string> playist_names;

    void write_to_json();
    void print_playlists();
    void make_global_json();
    void make_json_for_playlists();

public:
    JLIB();
    void initialize();
    std::vector<std::string> get_playlist_names() const {
        return playist_names;
    }
};
