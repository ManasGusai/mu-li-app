#include <iostream>
#include <fstream>
#include <stdexcept>
#include "music_manager.h"
#include "../third-party/json.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;

void MusicManager::play() {
    if(songs.empty()){
        return;
    }

    if(songs[static_cast<int>(so::curr)].loaded)
        PlayMusicStream(songs[static_cast<int>(so::curr)].m);
}

void MusicManager::update() {
    if(songs.empty()){
        return;
    }

    if(songs[static_cast<int>(so::curr)].loaded)
        UpdateMusicStream(songs[static_cast<int>(so::curr)].m);

}

void MusicManager::pause() {
    if(songs.empty()){
        return;
    }

    if(songs[static_cast<int>(so::curr)].loaded)
        PauseMusicStream(songs[static_cast<int>(so::curr)].m);
}

void MusicManager::resume() {
    if(songs.empty()){
        return;
    }

    if(songs[static_cast<int>(so::curr)].loaded)
        ResumeMusicStream(songs[static_cast<int>(so::curr)].m);
}

void MusicManager::calculate_neighbour_song_indexes() {
    next_song_index = (current_playing_song + 1) % song_data.size();
    last_song_index = (current_playing_song - 1 + song_data.size()) % song_data.size();

    std::cout << "previous: " << last_song_index << std::endl;
    std::cout << "current: " << current_playing_song << std::endl;
    std::cout << "next: " << next_song_index << std::endl;
}

void MusicManager::next() {
    if(songs.empty()) {
        return;
    }

    current_playing_song = (current_playing_song + 1) % song_data.size();
    calculate_neighbour_song_indexes();
    load_nxt_current_prev_songs();

}

void MusicManager::previous(){
    if(songs.empty()) {
        return;
    }

    current_playing_song = (current_playing_song - 1 + song_data.size()) % song_data.size();
    calculate_neighbour_song_indexes();
    load_nxt_current_prev_songs();

}

std::string MusicManager::get_current_song() {
    return song_data[current_playing_song].title;
}

void MusicManager::load_nxt_current_prev_songs() {
    std::string last = "song/" + song_data.at(last_song_index).title + ".mp3";
    std::string current = "song/" + song_data.at(current_playing_song).title + ".mp3";
    std::string next = "song/" + song_data.at(next_song_index).title + ".mp3";

    songs[static_cast<int>(so::prev)].id = song_data.at(last_song_index).id;
    songs[static_cast<int>(so::curr)].id = song_data.at(current_playing_song).id;
    songs[static_cast<int>(so::nxt)].id = song_data.at(next_song_index).id;

    Music n = LoadMusicStream(next.c_str());
    if(n.stream.buffer == 0) {
        throw std::runtime_error("Error Loading " + next);
    }

    Music c = LoadMusicStream(current.c_str());
    if(c.stream.buffer == 0) {
        throw std::runtime_error("Error Loading " + current);
    }

    Music l = LoadMusicStream(last.c_str());
    if(l.stream.buffer == 0) {
        throw std::runtime_error("Error Loading " + last);
    }

    songs[static_cast<int>(so::prev)].m = l;
    songs[static_cast<int>(so::prev)].loaded = true;
    
    songs[static_cast<int>(so::curr)].m = c;
    songs[static_cast<int>(so::curr)].loaded = true;
    
    songs[static_cast<int>(so::nxt)].m = l;
    songs[static_cast<int>(so::curr)].loaded = true;

}

void MusicManager::unload_nxt_and_prev_songs() {
    
}

void MusicManager::load_songs() {
    std::string path;
    path = "song/" + song_data[song_data.size() - 1].title + ".mp3";
    songs[static_cast<int>(so::prev)].m = LoadMusicStream(path.c_str());
    songs[static_cast<int>(so::prev)].id = song_data.size() - 1;
    songs[static_cast<int>(so::prev)].loaded = true;
    if(songs[static_cast<int>(so::prev)].m.stream.buffer == 0) {
        throw std::runtime_error("Error Loading " + path);
    }

    path = "song/" + song_data.at(0).title + ".mp3";
    songs[static_cast<int>(so::curr)].m = LoadMusicStream(path.c_str());
    songs[static_cast<int>(so::curr)].id = current_playing_song;
    songs[static_cast<int>(so::curr)].loaded = true;
    if(songs[static_cast<int>(so::curr)].m.stream.buffer == 0) {
        throw std::runtime_error("Error Loading " + path);
    }

    path = "song/" + song_data[1].title + ".mp3";
    songs[static_cast<int>(so::nxt)].m = LoadMusicStream(path.c_str());
    songs[static_cast<int>(so::nxt)].id = current_playing_song + 1;
    songs[static_cast<int>(so::nxt)].loaded = true;
    if(songs[static_cast<int>(so::nxt)].m.stream.buffer == 0) {
        throw std::runtime_error("Error Loading " + path);
    }


}

void MusicManager::load() {
    json data;
    std::string path = "data/song.json";
    std::ifstream file(path);
    if(!file) {
        std::cout << path << " does not found" <<"\n";
    }

    file >> data;

    auto all_songs = data["songs"];

    for(auto& s : all_songs) {
        Song song;
        if(s.contains("id"))
            song.id     = s["id"];
        if(s.contains("artist"))
            song.artist = s["artist"];
        if(s.contains("title")){
            song.title  = s["title"];
        }
        if(s.contains("icon")) {
            std::string icon_path = s["icon"].get<std::string>() + ".png";
            song.icon = icon_path;
        }

        if(s.contains("id"))
            song_data.emplace(s["id"].get<int>(), song);

    }

    load_songs();

}

void MusicManager::unload() {
    for(size_t i = 0; i < songs.size(); i++) {
        UnloadMusicStream(songs[i].m);
    }
}

void MusicManager::print_songs_data() {
    for(const auto& [key, value] : song_data) {
        std::cout << key << ": " << value.artist << " " << value.id << " " << value.title << "\n";
    }
}