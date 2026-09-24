#pragma once

#include <raylib.h>
#include <string>
#include <unordered_map>

class TextureManager {
private:
    std::unordered_map<std::string, Texture2D> textures;
public:
    Texture2D& get(const std::string& path);
};