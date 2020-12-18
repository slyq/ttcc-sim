#ifndef COGSET_H
#define COGSET_H

#include "cog.h"
#include "gag_collection.h"
#include <queue>
#include <stdexcept>
#include <stdlib.h>

class Cogset {
public:
    Cogset() {}
    Cogset(std::vector<Cog> set) : cogs(set) {}
    Cogset& operator=(const Cogset& other);
    void load();
    void load(std::queue<Cog>& q);
    void update();
    size_t getSize() const { return cogs.size(); }
    const Cog& getCog(int pos) const;
    Cog& getCog(int pos);
    bool allDead() const;
    int numLured() const;
    void gagCheck(const Gag& gagchoice) const;

    void attack(const std::vector<int>& affected);
    void attack(const std::vector<int>& affected, char type);
    void apply(const std::vector<int>& affected, GagKind gk);

    void fireTurn(const std::vector<Gag>& fires);
    void trapTurn(const std::vector<Gag>& traps);
    void lureTurn(const std::vector<Gag>& lures);
    void soundTurn(const std::vector<Gag>& sounds);
    void squirtTurn(const std::vector<Gag>& squirts);
    void zapTurn(const std::vector<Gag>& zaps);
    void throwTurn(const std::vector<Gag>& throws);
    void dropTurn(const std::vector<Gag>& drops);

    friend std::ostream& operator<<(std::ostream& out, const Cogset& cogset);
    void print(const std::vector<int>& affected) const;

    void setPrint(bool printState) { printCogset = printState; }
    void setUpdate(bool updateState) { roundUpdate = updateState; }

private:
    std::vector<Cog> cogs;
    bool printCogset = true;
    bool roundUpdate = false;
};

#endif
