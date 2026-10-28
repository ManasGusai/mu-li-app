#include "library_json.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include "../third-party/json.hpp"
#include "debug.h"

using json = nlohmann::json;
namespace fs = std::filesystem;

JLIB::JLIB()
{

}

void JLIB::initialize()
{
    check_playlists();
    generate_song_metadata();
    // write_to_json();
    print_playlists();

    //if(is_playlist_changed())
        make_global_json();
}

bool JLIB::is_playlist_changed()
{
    json data;
    fs::path songs_dir = "data/global.json";

    std::ifstream file(songs_dir);
    if(file.is_open())
    {
        file >> data;
        std::vector<std::string> saved_names;
        std::vector<std::string> current_names;

        //Singles
        

        //Playlists
        for(const auto& p : data["playlists"])
        {
            saved_names.push_back(p["name"].get<std::string>());
        }


        for(const auto& p : playlists){
            current_names.push_back(p.name);
        }

        if(saved_names != current_names || data["total_songs"] != total_songs){
            return true;
            if(debug)
                std::cout << "INFO: CHANGES IN SONGS FOLDER" << std::endl;
        }

    }

    return false;
}

void JLIB::check_playlists()
{
    json data;
    data["songs"] = json::array();

    fs::path songs_dir = fs::current_path() / "song";
    if(debug)
        std::cout << "INFO: SONGS DIRECTORY: " <<songs_dir.filename().string() << std::endl;
    
    std::vector<std::string> playist_names;
    for(const auto& entry : fs::directory_iterator(songs_dir)) {
        auto path = entry.path();

        if(entry.is_directory()) {
            playist_names.push_back(path.stem().string());
        }
    }

    for(size_t i = 0; i < playist_names.size(); i++)
    {
        Playlist p;
        p.name = playist_names[i];
        playlists.push_back(p);
    }
}

/*
    I know this name doesnt make much sense as its just filling the song names
    in the vector but i am using the index later so thats why the name........
*/
void JLIB::generate_song_metadata()
{
    json data;
    fs::path songs_dir = fs::current_path() / "song";

    //For solo
    for(const auto& entry : fs::directory_iterator(songs_dir)) {
        auto path = entry.path();

        if(entry.is_regular_file()) {
            Song song;
            song.title  = path.stem().string();
            song.artist = "unknown";
            song.id     = total_songs;
            song.icon   = "default";
            song.path   = songs_dir.string() + "/" + path.filename().string();
            song.source = SongSource::Solo;

            if(debug)
                std::cout << song.path << std::endl;
            
            song_data.push_back(song);
            total_songs++;
        }
    }

    //For playlists
    for(size_t i = 0; i < playlists.size(); i ++){
        for(const auto& entry : fs::directory_iterator((songs_dir / playlists[i].name)) ) {
            auto path = entry.path();

            if(entry.is_regular_file()) {
                Song song;
                song.title  = path.stem().string();
                song.artist = "unknown";
                song.id     = total_songs;
                song.icon   = "default";
                song.path   = songs_dir.string() + "/" + playlists[i].name + "/" + path.filename().string();
                song.source = SongSource::Playlist;
                
                song_data.push_back(song);

                playlists[i].song_ids.push_back(song.id);
                
                total_songs++;
            }
        }
    }

    if(debug)
        std::cout << "INFO: GENERATED SONG NAMES:" << std::endl;
}

void JLIB::make_global_json()
{
    json data;
    fs::path global_dir = fs::current_path() / "data";
    data["total_songs"] = total_songs;

    //Write song names to json
    for(size_t i = 0; i < song_data.size(); i++)
    {
        data["Songs"].push_back({
            {"id",      song_data[i].id},
            {"title",   song_data[i].title},
            {"artist",  song_data[i].artist},
            {"icon",    song_data[i].icon},
            {"path",    song_data[i].path}
        });
    }

    //Playlist names
    for(const auto& playlist : playlists)
    {
        data["playlists"].push_back({
            {"name", playlist.name},
            {"songids", playlist.song_ids}
        });
    }

    // Song names and id
    std::string json_path = "data/global.json";

    std::ofstream file(json_path);
    if(!file) {
        std::cout << "INFO: " << json_path << " does not found" <<"\n";
    }
    file << data.dump(4);

    if(debug)
        std::cout << "INFO: REWROTE GLOBAL JSON" << std::endl;
}

void JLIB::print_playlists()
{
    std::cout << "INFO: PLAYLISTS: " << std::endl;
    for(size_t i = 0; i < playlists.size(); i++)
    {
        std::cout << i + 1 << ": " << playlists[i].name << std::endl;
    }
}