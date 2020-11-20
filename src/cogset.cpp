#include "cogset.h"

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

Cogset::Cogset(std::vector<Cog> set) {
    cogs = set;
}

Cogset& Cogset::operator=(const Cogset& other) {
    q = other.q;
    cogs = other.cogs;
    return *this;
}

void Cogset::load() {
    // shift right
    for (std::vector<Cog>::iterator it = cogs.begin(); it != cogs.end(); ++it) {
        if (it->getHP() == 0) {
            cogs.erase(it);
            --it;
        }
    }
    if (cogs.size() != 4) { // there were some cogs not alive
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

size_t Cogset::getSize() {
    return cogs.size();
}

Cog& Cogset::getCog(int pos) {
    if (pos > 3) {
        throw std::invalid_argument("Out of bounds");
    }
    return cogs[pos];
}
