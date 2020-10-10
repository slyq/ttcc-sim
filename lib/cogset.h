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
        Cogset(std::vector<Cog> set);
        Cogset& operator=(const Cogset& other);
        void load();
        size_t getSize();
        Cog& getCog(int pos);
    private:
        std::queue<int> q;
        std::vector<Cog> cogs;
        const double EXE_CHANCE = 30;
};

#endif
