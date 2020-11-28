#include "cogset.h"
#include <algorithm>
#include <time.h>

Cogset::Cogset(std::queue<int> set) : q(set) {
    srand(time(NULL));
    int starting_size = rand() % 4 + 1;
    // for (int i = 3; i >= 0; --i) { // ceo
    for (int i = 0; i < starting_size; ++i) { // other boss battles
        if (!q.empty()) {
            if (rand() % 100 + 1 < EXE_CHANCE) {
                cogs.insert(cogs.begin(), Cog(q.front(), true));
            } else {
                cogs.insert(cogs.begin(), Cog(q.front()));
            }
            q.pop();
        }
    }
}

Cogset& Cogset::operator=(const Cogset& other) {
    q = other.q;
    cogs = other.cogs;
    return *this;
}

void Cogset::load() {
    // remove dead cogs
    cogs.erase(std::remove_if(cogs.begin(), cogs.end(), [](Cog& c) { return c.getHP() == 0; }), cogs.end());
    if (cogs.size() != 4) {
        // attempt to replace dead cogs
        while (!q.empty() && cogs.size() < 4) {
            if (rand() % 100 + 1 < EXE_CHANCE) {
                cogs.insert(cogs.begin(), Cog(q.front(), true));
            } else {
                cogs.insert(cogs.begin(), Cog(q.front()));
            }
            q.pop();
        }
    }
}

Cog& Cogset::getCog(int pos) {
    if (pos >= (int)cogs.size()) {
        throw std::invalid_argument("Out of bounds");
    }
    return cogs[pos];
}
