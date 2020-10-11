#ifndef BATTLE_H
#define BATTLE_H

#include "cogset.h"
#include "gag_collection.h"
#include <vector>
#include <iostream>
#include <map>
#include <stdexcept>

struct Strategy {
    std::vector<Gag> gags;
    std::vector<int> positions;
    size_t config = 0; // 0 default, 1 cross one-liner, 2 vectored gag
};

struct DirectedGag {
    int damage;
    int target;
};

struct DirectedGagComparator {
    bool operator() (const DirectedGag& first, const DirectedGag& second) const {
        if (first.damage == second.damage) {
            return second.target < first.target;
        }
        return first.damage < second.damage;
    }
};

struct CrossDirectedGagComparator {
    bool operator() (const DirectedGag& first, const DirectedGag& second) const {
        if (first.damage == second.damage) {
            return first.target < second.target;
        }
        return first.damage < second.damage;
    }
};

class Battle {
public:
    Battle();
    Battle(std::queue<int> set);
    Battle(std::vector<Cog> set);
    Battle& operator=(const Battle& other) { c = other.c; return *this; }
    ~Battle() {}
    void main(size_t config);
private:
    Cogset c;
    GagCollection gc;
    std::map<std::string, int> position_definition;

    Strategy parse_oneliner(std::string strat);
    Strategy parse_gags(std::string strat);
    void turn(Strategy strat);
    //void print();
    void lureC(std::vector<DirectedGag> lures);
    void sqattack(std::vector<DirectedGag> squirts);
    void zattack(std::vector<DirectedGag> zaps);
};

#endif
