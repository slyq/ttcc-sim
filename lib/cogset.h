#ifndef COGSET_H
#define COGSET_H

#include "cog.h"
#include <queue>
#include <stdexcept>
#include <stdlib.h>

class Cogset {
public:
    Cogset() {}
    Cogset(std::queue<int>);
    Cogset(std::vector<Cog> set) : cogs(set) {}
    Cogset& operator=(const Cogset& other);
    void load();
    size_t getSize() { return cogs.size(); }
    Cog& getCog(int pos);
    friend std::ostream& operator<<(std::ostream& out, const Cogset& cogset);
    void print(std::vector<int> affected);
private:
    std::queue<int> q;
    std::vector<Cog> cogs;
    const double EXE_CHANCE = 30;
};

#endif
