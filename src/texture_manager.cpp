#include "texture_manager.hpp"

TextureManager::TextureManager() {
    textures["default"] = LoadTexture("cover/default.png");
}

Texture2D& TextureManager::get(const std::string& path) {
    auto it = textures.find(path);

    //agar texture phle load ho gaya h to return form func instead of loading ot again
    if(it != textures.end()) {
        return it->second;
    }

    if(!FileExists(path.c_str())) {
        return textures.at("default");
    }

    textures[path] = LoadTexture(path.c_str());

    return textures[path];
}
