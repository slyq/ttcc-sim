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
    Battle();
    Battle(std::queue<int> set);
    Battle(std::vector<Cog> set);
    Battle& operator=(const Battle& other) {
        c = other.c;
        return *this;
    }
    ~Battle() {}
    void generate();
    void battle();

    void setPresState(bool presState) { autoPres = presState; }
    void setHitState(bool hitState) { autoHit = hitState; }
    void setInputState(bool inputState) { lineInput = inputState; }
    void setPrint(bool printState) { printCogset = printState; }

    struct Strategy {
        std::vector<Gag> gags;
        // 0 right-to-left quickhand output, 1 cross (left-to-right quickhand output), 2 left-to-right input
        size_t config = 0;
        Strategy(std::vector<Gag> g, size_t conf) : gags(g), config(conf) {}
    };

    Cogset getCogset() { return c; }
    void setCogset(std::vector<Cog> set) { c = Cogset(set); }
    bool cogsetDead();
    Strategy parseOneliner(std::string strat);
    Gag parseGag(std::string command);
    void gagCheck(const Gag& gagchoice);

    void turn(Strategy strat);

private:
    Cogset c;
    GagCollection gc;
    std::map<std::string, int> posDefinition
            = {{"left", 0}, {"mid-left", 1}, {"mid", 1}, {"mid-right", 2}, {"right", 0}};

    void fireTurn(std::vector<Gag> fires);
    void trapTurn(std::vector<Gag> traps);
    void lureTurn(std::vector<Gag> lures);
    void soundTurn(std::vector<Gag> sounds);
    void squirtTurn(std::vector<Gag> squirts);
    void zapTurn(std::vector<Gag> zaps);
    void throwTurn(std::vector<Gag> throws);
    void dropTurn(std::vector<Gag> drops);

    const std::string file_path = "gags.txt";

    bool autoPres = true;
    bool autoHit = true;
    bool lineInput = true;
    bool printCogset = true;
};

#endif
