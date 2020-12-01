#ifndef GAG_COLLECTION_H
#define GAG_COLLECTION_H

#include <map>
#include <set>
#include <stdexcept>
#include <stdlib.h>
#include <string>

enum class GagKind {
    PASS = -1,
    TOONUP = 0,
    TRAP = 1,
    LURE = 2,
    SOUND = 3,
    SQUIRT = 4,
    ZAP = 5,
    THROW = 6,
    DROP = 7,
    FIRE = 8,
};

struct Gag {
    GagKind kind;
    std::string name;
    unsigned short damage;
    int accuracy;
    int target;
    bool prestiged;

    Gag() : kind(GagKind::PASS) {}
    Gag(int t) : kind(GagKind::FIRE), target(t) {}
    Gag(GagKind k, std::string n, unsigned short d, float a) : kind(k), name(n), damage(d), accuracy(a) {}
};

class GagCollection {
public:
    static GagCollection read(const std::string& file_path);
    bool contains(std::string name) const { return gagmap.find(name) != gagmap.end(); }
    bool isSOS(std::string name) const { return sosset.find(name) != sosset.end(); }
    const Gag& get(std::string name) { return gagmap[name]; }

private:
    std::map<std::string, Gag> gagmap;
    std::set<std::string> sosset;
};

#endif
