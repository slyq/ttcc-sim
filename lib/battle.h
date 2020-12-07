#ifndef BATTLE_H
#define BATTLE_H

#include "cogset.h"
#include "gag_collection.h"
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

struct GagComparator {
    bool operator()(const Gag& first, const Gag& second) const { return first.damage <= second.damage; }
};

class Battle {
public:
    Battle() {}
    Battle(std::string file_path);
    Battle(std::string file_path, const std::queue<int>& set);
    Battle(std::string file_path, const std::vector<Cog>& set);
    Battle& operator=(const Battle& other) {
        cogset = other.cogset;
        posDefinition = other.posDefinition;
        autoPres = other.autoPres;
        autoHit = other.autoHit;
        lineInput = other.lineInput;
        return *this;
    }
    ~Battle() {}
    void generate();
    void battle();

    void setPresState(bool presState) { autoPres = presState; }
    void setHitState(bool hitState) { autoHit = hitState; }
    void setUpdate(bool updateState) { cogset.setUpdate(updateState); }
    void setInputState(bool inputState) { lineInput = inputState; }
    void setPrint(bool printState) { cogset.setPrint(printState); }

    Cogset& getCogset() { return cogset; }
    void setCogset(std::vector<Cog> set) { cogset = Cogset(set); }
    std::vector<Gag> parseOneliner(std::string strat);
    Gag parseGag(std::string command);

    void turn(std::vector<Gag> strat);

private:
    Cogset cogset;
    GagCollection gc;
    std::map<std::string, int> posDefinition = {
        {"left", 0},
        {"mid-left", 1},
        {"mid", 1},
        {"mid-right", 2},
        {"right", 0}
    };

    int validQuickhand(std::string quickhand) const;
    bool isPluralGag(std::string gag) const;

    bool autoPres = true;
    bool autoHit = true;
    bool lineInput = true;
};

#endif
