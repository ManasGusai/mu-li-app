#pragma once
#include <string>
#include <raylib.h>

enum class SongSource
{
    Solo,
    Playlist
};

struct Song {
    int id;
    std::string artist;
    std::string title;
    std::string icon;
    std::string path;
    SongSource source;
};