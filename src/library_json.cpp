#include "library_json.h"
#include <iostream>
#include <filesystem>
#include <fstream>
// #include "src/song.h"
#include "../third-party/json.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;

JLIB::JLIB()
{
    json data;
    data["songs"] = json::array();

    fs::path songs_dir = fs::current_path() / "song";
    std::cout << "Songs Directory: " <<songs_dir.filename().string() << std::endl;
    
    for(const auto& entry : fs::directory_iterator(songs_dir)) {
        auto path = entry.path();

        if(entry.is_directory())
            playist_names.push_back(path.stem().string());
    }
}

void JLIB::initialize()
{
    write_to_json();
    make_global_json();
    print_playlists();
}

void JLIB::make_global_json()
{
    json data;
    data["info"] = json::array();
    fs::path global_dir = fs::current_path() / "data";
    for(size_t i = 0; i < playist_names.size(); i++)
    {
        data["info"].push_back({
            {i+1, playist_names[i]}
        });
    }

    std::string json_path = "data/global.json";

    std::ofstream file(json_path);
    if(!file) {
        std::cout << json_path << " does not found" <<"\n";
    }
    file << data.dump(4);
}

void JLIB::write_to_json() {
    json data;
    data["songs"] = json::array();

    fs::path songs_dir = fs::current_path() / "song";
    std::cout << "Songs Directory: " <<songs_dir.filename().string() << std::endl;

    int id = 0;
    
    for(const auto& entry : fs::directory_iterator(songs_dir)) {
        if(!entry.is_regular_file())
            continue;
        
        auto path = entry.path();

        if(path.extension() == ".mp3") {
            std::string name = path.stem().string();
            data["songs"].push_back({
                {"id",     id},
                {"artist", "Unknown"},
                {"title",  name},
                {"icon",   "default"}
            });
        }
        id++;
    }

    std::string json_path = "data/song.json";

    std::ofstream file(json_path);
    if(!file) {
        std::cout << json_path << " does not found" <<"\n";
    }
    file << data.dump(4);
}

void JLIB::print_playlists()
{
    std::cout << "PLAYLISTS: " << std::endl;
    for(size_t i = 0; i < playist_names.size(); i++)
    {
        std::cout << i + 1 << ": " << playist_names[i] << std::endl;
    }
}