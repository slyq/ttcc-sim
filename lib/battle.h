#ifndef BATTLE_H
#define BATTLE_H

#include "cogset.h"
#include "gag_collection.h"
#include <vector>
#include <iostream>
#include <map>
#include <stdexcept>

struct GagComparator {
    bool operator() (const Gag& first, const Gag& second) const {
        if (first.damage == second.damage) {
            return second.target < first.target;
        }
        return first.damage < second.damage;
    }
};

struct CrossGagComparator {
    bool operator() (const Gag& first, const Gag& second) const {
        if (first.damage == second.damage) {
            return first.target < second.target;
        }
        return first.damage < second.damage;
    }
};

struct OrderedGagComparator {
    bool operator() (const Gag& first, const Gag& second) const {
        return first.damage <= second.damage;
    }
};

class Battle {
public:
    Battle();
    Battle(std::queue<int> set);
    Battle(std::vector<Cog> set);
    Battle& operator=(const Battle& other) { c = other.c; return *this; }
    ~Battle() {}
    void main(bool line_input);

    void setPresState(bool pres_state) { auto_pres = pres_state; }

    struct Strategy {
        std::vector<Gag> gags;
        size_t config = 0; // 0 default, 1 cross one-liner, 2 left-to-right input
    };
private:
    Cogset c;
    GagCollection gc;
    std::map<std::string, int> position_definition = {{"left", 0}, {"mid-left", 1}, {"mid", 1}, {"mid-right", 2}, {"right", 0}};

    Strategy parse_oneliner(std::string strat);
    Strategy parse_gags();
    void turn(Strategy strat);
    //void print();
    void lure_turn(std::vector<Gag> lures);
    void sound_turn(std::vector<Gag> sounds);
    void squirt_turn(std::vector<Gag> squirts);
    void zap_turn(std::vector<Gag> zaps);
    void throw_turn(std::vector<Gag> throws);
    void drop_turn(std::vector<Gag> drops);
    const std::string file_path = "gags.txt";

    bool auto_pres = true;
};

#endif
