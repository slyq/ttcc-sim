#ifndef GAME_CONFIG
#define GAME_CONFIG

#include <cstdint>
#include <string>

class GameConfig {
public:
    bool autoPres = true;
    bool autoHit = true;
    bool roundUpdate = false;
    bool lineInput = true;

    std::string gagFilePath = "gags.txt";

    static GameConfig read(std::string file);
    static bool toBool(std::string s);
};

#endif
