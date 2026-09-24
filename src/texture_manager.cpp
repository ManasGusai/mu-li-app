#include "texture_manager.hpp"

Texture2D& TextureManager::get(const std::string& path) {
    auto it = textures.find(path);

    //agar texture phle load ho gaya h to return form func instead of loading ot again
    if(it != textures.end()) {
        return it->second;
    }

    textures[path] = LoadTexture(path.c_str());

    return textures[path];
}
