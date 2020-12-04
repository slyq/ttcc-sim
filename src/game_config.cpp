#include "game_config.h"

#include <cctype>
#include <fstream>
#include <sstream>

using namespace std;

enum class ParserState {
    LOOKING_FOR_KEY,    // not inside any structures at the moment, waiting for a key or a comment character
    IN_KEY,             // inside key, waiting for equals sign or whitespace
    LOOKING_FOR_VALUE,  // just saw colon, now looking for start of value
    IN_VALUE            // inside value, waiting for newline to end it.
};

bool GameConfig::toBool(string s) {
    if (s == "1" || s == "true" || s == "on") {
        return true;
    }
    if (s == "0" || s == "false" || s == "off") {
        return false;
    }
    throw runtime_error("invalid config");
}

GameConfig GameConfig::read(string file_path) {
    ifstream file(file_path);
    if (!file) {
        throw runtime_error("cannot open game config file!");
    }

    string key_buffer;
    string value_buffer;
    char cursor;
    ParserState state = ParserState::LOOKING_FOR_KEY;
    GameConfig config;

    while (!file.eof()) {
        file.get(cursor);
        if (file.eof()) {
            break;
        }

        switch (state) {
        case ParserState::LOOKING_FOR_KEY:
            if (!isspace(cursor)) {
                key_buffer = toupper(cursor);
                state = ParserState::IN_KEY;
            }
            break;
        case ParserState::IN_KEY:
            if (cursor == ':') {
                state = ParserState::LOOKING_FOR_VALUE;
            } else {
                key_buffer += toupper(cursor);
            }
            break;
        case ParserState::LOOKING_FOR_VALUE:
            if (!isspace(cursor)) {
                value_buffer = cursor;
                state = ParserState::IN_VALUE;
            }
            break;
        case ParserState::IN_VALUE:
            if (cursor == '\n') {
                if (key_buffer == "ALL_PRESTIGE") {
                    config.autoPres = toBool(value_buffer);
                } else if (key_buffer == "NO_MISS") {
                    config.autoHit = toBool(value_buffer);
                } else if (key_buffer == "ROUND_DECAY") {
                    config.roundUpdate = toBool(value_buffer);
                } else if (key_buffer == "LINE_INPUT") {
                    config.lineInput = toBool(value_buffer);
                } else if (key_buffer == "GAG_FILE") {
                    config.gagFilePath = value_buffer;
                }
                state = ParserState::LOOKING_FOR_KEY;
            } else {
                value_buffer += cursor;
            }
            break;
        }
    }

    return config;
}
