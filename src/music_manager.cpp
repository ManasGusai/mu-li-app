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

    if(songs[curr_index_in_loaded_songs].loaded)
        PlayMusicStream(songs[curr_index_in_loaded_songs].m);
}

void MusicManager::update() {
    if(songs.empty()){
        return;
    }

    if(songs[curr_index_in_loaded_songs].loaded)
        UpdateMusicStream(songs[curr_index_in_loaded_songs].m);

}

void MusicManager::pause() {
    if(songs.empty()){
        return;
    }

    if(songs[curr_index_in_loaded_songs].loaded)
        PauseMusicStream(songs[curr_index_in_loaded_songs].m);
}

void MusicManager::resume() {
    if(songs.empty()){
        return;
    }

    if(songs[curr_index_in_loaded_songs].loaded)
        ResumeMusicStream(songs[curr_index_in_loaded_songs].m);
}

void MusicManager::calculate_neighbour_song_indexes() {
    next_song_index = (current_playing_song + 1) % song_data.size();
    last_song_index = (current_playing_song - 1 + song_data.size()) % song_data.size();

    // std::cout << "previous: " << last_song_index << std::endl;
    // std::cout << "current: " << current_playing_song << std::endl;
    // std::cout << "next: " << next_song_index << std::endl;
}

void MusicManager::next() {
    if(songs.empty()) {
        return;
    }

    UnloadMusicStream(songs[prev_index_in_loaded_songs].m);
    current_playing_song = (current_playing_song + 1) % song_data.size();
    calculate_neighbour_song_indexes();

    std::string next = "song/" + song_data.at(next_song_index).title + ".mp3";

    prev_index_in_loaded_songs = (prev_index_in_loaded_songs + 1) % songs.size();
    curr_index_in_loaded_songs = (curr_index_in_loaded_songs + 1) % songs.size();
    next_index_in_loaded_songs = (next_index_in_loaded_songs + 1) % songs.size();

    songs[next_index_in_loaded_songs].id = song_data.at(next_song_index).id;

    Music n = LoadMusicStream(next.c_str());
    if(n.stream.buffer == 0) {
        throw std::runtime_error("Error Loading " + next);
    }

    songs[next_index_in_loaded_songs].m = n;
    songs[next_index_in_loaded_songs].loaded = true;

}

void MusicManager::previous(){
    if(songs.empty()) {
        return;
    }

    UnloadMusicStream(songs[next_index_in_loaded_songs].m);
    current_playing_song = (current_playing_song - 1 + song_data.size()) % song_data.size();
    calculate_neighbour_song_indexes();

    std::string prev = "song/" + song_data.at(last_song_index).title + ".mp3";
    prev_index_in_loaded_songs = (prev_index_in_loaded_songs - 1 + songs.size()) % songs.size();
    curr_index_in_loaded_songs = (curr_index_in_loaded_songs - 1 + songs.size()) % songs.size();
    next_index_in_loaded_songs = (next_index_in_loaded_songs - 1 + songs.size()) % songs.size();

    songs[prev_index_in_loaded_songs].id = song_data.at(last_song_index).id;
    Music p = LoadMusicStream(prev.c_str());
    if(p.stream.buffer == 0) {
        throw std::runtime_error("Error Loading " + prev);
    }

    songs[prev_index_in_loaded_songs].m = p;
    songs[prev_index_in_loaded_songs].loaded = true;

}

std::string MusicManager::get_current_song() {
    return song_data[current_playing_song].title;
}

void MusicManager::load_songs() {
    std::string path;
    path = "song/" + song_data[song_data.size() - 1].title + ".mp3";
    songs[prev_index_in_loaded_songs].m = LoadMusicStream(path.c_str());
    songs[prev_index_in_loaded_songs].id = song_data.size() - 1;
    songs[prev_index_in_loaded_songs].loaded = true;
    if(songs[prev_index_in_loaded_songs].m.stream.buffer == 0) {
        throw std::runtime_error("Error Loading " + path);
    }

    path = "song/" + song_data.at(0).title + ".mp3";
    songs[curr_index_in_loaded_songs].m = LoadMusicStream(path.c_str());
    songs[curr_index_in_loaded_songs].id = current_playing_song;
    songs[curr_index_in_loaded_songs].loaded = true;
    if(songs[curr_index_in_loaded_songs].m.stream.buffer == 0) {
        throw std::runtime_error("Error Loading " + path);
    }

    path = "song/" + song_data[1].title + ".mp3";
    songs[next_index_in_loaded_songs].m = LoadMusicStream(path.c_str());
    songs[next_index_in_loaded_songs].id = current_playing_song + 1;
    songs[next_index_in_loaded_songs].loaded = true;
    if(songs[next_index_in_loaded_songs].m.stream.buffer == 0) {
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