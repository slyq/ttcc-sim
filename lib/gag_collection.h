#ifndef GAG_COLLECTION_H
#define GAG_COLLECTION_H

#include <stdlib.h>
#include <string>
#include <vector>
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
    int damage;
    //int target
    //size_t accuracy;
    //bool prestiged;

    Gag() : kind(GagKind::PASS) {}
    Gag(GagKind k, std::string n, int d) : kind(k), name(n), damage(d) {}
};

class GagCollection {
    public:
        GagCollection();
        ~GagCollection() {}
        GagCollection& operator=(const GagCollection& other);
        const Gag& get(size_t row, size_t col) const { return gagsets[row][col]; }
        bool contains(std::string name) const { return gagmap.find(name) != gagmap.end(); }
        const Gag& get(std::string name) { return gagmap[name]; }
    private:
        std::vector<std::vector<Gag>> gagsets;
        std::map<std::string, Gag> gagmap;
        const std::string file_path = "gags.txt";
};

#endif
