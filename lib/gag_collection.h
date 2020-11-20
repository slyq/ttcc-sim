#ifndef GAG_COLLECTION_H
#define GAG_COLLECTION_H

#include <stdlib.h>
#include <string>
#include <map>
#include <stdexcept>

enum class GagKind {
    PASS,
    TOONUP,
    TRAP,
    LURE,
    SOUND,
    SQUIRT,
    ZAP,
    THROW,
    DROP,
};

struct Gag {
    GagKind kind;
    std::string name;
    unsigned short damage;
    float accuracy;
    int target;
    bool prestiged;

    Gag() : kind(GagKind::PASS) {}
    Gag(GagKind k, std::string n, unsigned short d, float a) : kind(k), name(n), damage(d), accuracy(a) {}
};

class GagCollection {
    public:
        static GagCollection read(const std::string& file_path);
        bool contains(std::string name) const { return gagmap.find(name) != gagmap.end(); }
        const Gag& get(std::string name) { return gagmap[name]; }
    private:
        std::map<std::string, Gag> gagmap;
};

#endif
