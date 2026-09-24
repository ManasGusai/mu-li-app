#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
// #include "src/song.h"
#include "third-party/json.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;


void write_to_json() {
    json data;
    data["songs"] = json::array();

    fs::path songs_dir = fs::current_path() / "song";
    std::cout << "Songs Directory: " <<songs_dir.filename().string();

    int id = 1;
    
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

int main() {

    write_to_json();
}