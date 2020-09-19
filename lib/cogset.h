#ifndef COGSET_H
#define COGSET_H

#include "cog.h"
#include <queue>
#include <stdexcept>
#include <stdlib.h>

class Cogset {
    public:
        Cogset(std::queue<int>);
        void load();
        int getSize();
        Cog& getCog(int pos);
    private:
        std::queue<int> q;
        Cog cogs[4];
        int size = 0;
        const double EXE_CHANCE = 30;
};

#endif
