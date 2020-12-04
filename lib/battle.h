#ifndef BATTLE_H
#define BATTLE_H

#include "cogset.h"
#include "gag_collection.h"
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

struct GagComparator {
    bool operator()(const Gag& first, const Gag& second) const {
        if (first.damage == second.damage) {
            return second.target < first.target;
        }
        return first.damage < second.damage;
    }
};

struct CrossGagComparator {
    bool operator()(const Gag& first, const Gag& second) const {
        if (first.damage == second.damage) {
            return first.target < second.target;
        }
        return first.damage < second.damage;
    }
};

struct OrderedGagComparator {
    bool operator()(const Gag& first, const Gag& second) const { return first.damage <= second.damage; }
};

class Battle {
public:
    Battle() {}
    Battle(std::string file_path);
    Battle(std::string file_path, std::queue<int> set);
    Battle(std::string file_path, std::vector<Cog> set);
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


    struct Strategy {
        std::vector<Gag> gags;
        // 0 right-to-left quickhand output, 1 cross (left-to-right quickhand output), 2 left-to-right input
        size_t config = 0;
        Strategy(std::vector<Gag> g, size_t conf) : gags(g), config(conf) {}
    };

    Cogset getCogset() { return cogset; }
    void setCogset(std::vector<Cog> set) { cogset = Cogset(set); }
    Strategy parseOneliner(std::string strat);
    Gag parseGag(std::string command);

    void turn(Strategy strat);

private:
    Cogset cogset;
    GagCollection gc;
    std::map<std::string, int> posDefinition
            = {{"left", 0}, {"mid-left", 1}, {"mid", 1}, {"mid-right", 2}, {"right", 0}};

    bool autoPres = true;
    bool autoHit = true;
    bool lineInput = true;
};

#endif
