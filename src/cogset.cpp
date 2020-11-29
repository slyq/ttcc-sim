#include "cogset.h"

#include "colors.h"
#include <algorithm>
#include <time.h>

Cogset::Cogset(std::queue<int> set) : q(set) {
    srand(time(NULL));
    int starting_size = rand() % 4 + 1;
    // for (int i = 3; i >= 0; --i) { // ceo
    for (int i = 0; i < starting_size; ++i) {  // other boss battles
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

std::ostream& operator<<(std::ostream& out, const Cogset& cogset) {
    for (const Cog& cog : cogset.cogs) {
        out << " " << cog << "\t\t";
    }
    return out;
}

void Cogset::print(std::vector<int> affected) {
    if (affected.size() == cogs.size()) {
        for (size_t i = 0; i < cogs.size(); ++i) {
            if (affected[i]) {
                std::cout << ATTACKED << cogs[i] << ATTACKED << "\t\t";
            } else {
                std::cout << NOTATTACKED << cogs[i] << NOTATTACKED << "\t\t";
            }
        }
    }
    std::cout << std::endl;
}
