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

    PlayMusicStream(songs[current_playing_song]);
}

void MusicManager::update() {
    if(songs.empty()){
        return;
    }

    Music music = songs[current_playing_song];
    if(music.stream.buffer == 0) {
        std::cout << "Error playing music" << "\n";
        return;
    }

    UpdateMusicStream(music);

}

void MusicManager::pause() {
    if(songs.empty()){
        return;
    }

    PauseMusicStream(songs[current_playing_song]);
}

void MusicManager::resume() {
    if(songs.empty()){
        return;
    }

    ResumeMusicStream(songs[current_playing_song]);
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
    load_neighbour_songs();

}

void MusicManager::previous(){
    if(songs.empty()) {
        return;
    }

    current_playing_song = (current_playing_song - 1 + song_data.size()) % song_data.size();
    calculate_neighbour_song_indexes();
    load_neighbour_songs();

}

std::string MusicManager::get_current_song() {
    return song_data[current_playing_song].title;
}

void MusicManager::load_neighbour_songs() {
    std::string next = "song/" + song_data.at(next_song_index).title + ".mp3";
    std::string last = "song/" + song_data.at(last_song_index).title + ".mp3";

    Music n = LoadMusicStream(next.c_str());
    if(n.stream.buffer == 0) {
        throw std::runtime_error("Error Loading " + next);
    }

    Music l = LoadMusicStream(last.c_str());
    if(n.stream.buffer == 0) {
        throw std::runtime_error("Error Loading " + last);
    }

    songs.push_back(l);
    songs.push_back(n);

}

void MusicManager::load_songs() {
    std::string path;
    if(song_data.size() > 0){
        for(size_t i = current_playing_song; i < 5 && i < song_data.size(); i++){
            path = "song/" + song_data.at(i).title + ".mp3";
            std::cout << path << "\n";
            Music music = LoadMusicStream(path.c_str());
            if(music.stream.buffer == 0) {
                throw std::runtime_error("Error Loading " + path);
            }

            songs.push_back(music);
        }
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

    auto songs = data["songs"];

    for(auto& s : songs) {
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

void MusicManager::print_songs_data() {
    for(const auto& [key, value] : song_data) {
        std::cout << key << ": " << value.artist << " " << value.id << " " << value.title << "\n";
    }
}