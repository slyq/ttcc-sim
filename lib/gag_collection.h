#ifndef GAG_COLLECTION_H
#define GAG_COLLECTION_H

#include <map>
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
    unsigned short level;
    unsigned short damage;
    unsigned short accuracy;
    int target;
    bool prestiged;
    int passiveEffect;
    bool isSOS;

    Gag() : kind(GagKind::PASS), target(-1) {}
    Gag(GagKind k, unsigned short d, int t, bool p) : kind(k), damage(d), target(t), prestiged(p) {}
    Gag(GagKind k, std::string n, unsigned short l, unsigned short d, unsigned short a, bool s) : kind(k), name(n), level(l), damage(d), accuracy(a), passiveEffect(0), isSOS(s) {}
    Gag(GagKind k, std::string n, unsigned short l, unsigned short d, unsigned short a, int pe, bool s) : kind(k), name(n), level(l), damage(d), accuracy(a), passiveEffect(pe), isSOS(s) {}
};

class GagCollection {
public:
    static GagCollection read(const std::string& file_path);
    bool contains(std::string name) const { return gagmap.find(name) != gagmap.end(); }
    bool isSOS(std::string name) const { return contains(name) && get(name).isSOS; }
    const Gag& get(std::string name) const { return gagmap.at(name); }

private:
    std::map<std::string, Gag> gagmap;
};

#endif
