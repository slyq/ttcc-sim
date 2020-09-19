#include "cogset.h"

Cogset::Cogset(std::queue<int> set) : q(set) {
    srand(time(NULL));
    // for (int i = 3; i >= 0; --i) { // ceo
    for (int i = rand() % 4 + 1; i >= 0; --i) { // other boss battles
        if (!q.empty()) {
            if (rand() % 100 + 1 < EXE_CHANCE) {
                cogs[i] = Cog(q.front(), true);
            } else {
                cogs[i] = Cog(q.front());
            }
            q.pop();
            ++size;
        }
    }
}

void Cogset::load() {
    // shift right
    Cog temp[4];
    size_t index = 3;
    size = 0;
    for (int i = 3; i >= 0; --i) {
        if (cogs[i].getHP() != 0) {
            temp[index--] = cogs[i];
            ++size;
        }
    }
    if (size != 4) { // there were some cogs not alive
        while (!q.empty() && size < 4) {
            temp[index--] = Cog(q.front());
            q.pop();
            ++size;
        }
        for (size_t i = 0; i < 4; ++i) {
            cogs[i] = temp[i];
        }
    }
}

int Cogset::getSize() {
    return size;
}

Cog& Cogset::getCog(int pos) {
    if (pos > 3) {
        throw std::invalid_argument("Out of bounds");
    }
    return cogs[pos];
}
