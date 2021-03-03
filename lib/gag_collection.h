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

enum class SosKind {
    TOONUP = 0,
    TRAP = 1,
    LURE = 2,
    SOUND = 3,
    SQUIRT = 4,
    ZAP = 5,
    THROW = 6,
    DROP = 7,
    ALL = 8,
    ACC = 9,
    ATTACK_DOWN = 10,
};

class ToonSelection {
    public:
    ToonSelection() {}
    ToonSelection(std::string n) : name(n) {}
    virtual bool isBuff() = 0;
    std::string name;
};

class Gag : public ToonSelection {
    public:
    GagKind kind;
    unsigned short level;
    unsigned short damage;
    unsigned short accuracy;
    int target;
    bool prestiged;
    int passiveEffect;
    int bonusEffect = 0;
    
    bool isBuff() { return false; }

    Gag() : target(-1) { kind = GagKind::PASS; }
    Gag(GagKind k, unsigned short d, int t, bool p) : damage(d), target(t), prestiged(p) { kind = k; }
    Gag(GagKind k, std::string n, unsigned short l, unsigned short d, unsigned short a) : ToonSelection(n), kind(k), level(l), damage(d), accuracy(a), passiveEffect(0) {}
    Gag(GagKind k, std::string n, unsigned short l, unsigned short d, unsigned short a, int pe) : ToonSelection(n), kind(k), level(l), damage(d), accuracy(a), passiveEffect(pe) {}
};

class Sos : public ToonSelection {
    public:
    SosKind kind;
    int passiveEffect;
    int duration;

    bool isBuff() { return true; }
    Sos() {}
    Sos(SosKind k, std::string n, int pe, int d) : ToonSelection(n), kind(k), passiveEffect(pe), duration(d) {}
};

class GagCollection {
public:
    static GagCollection read(const std::string& file_path);
    bool contains(std::string name) const { return isGag(name) || isSos(name); }
    bool isGag(std::string name) const { return gags.find(name) != gags.end(); }
    bool isSos(std::string name) const { return soses.find(name) != soses.end();; }
    const Gag& getGag(std::string name) const { return gags.at(name); }
    const Sos& getSos(std::string name) const { return soses.at(name); }
    const std::string& getGagTypeString(GagKind k) const { return gagTypeMap.at(k); }
    const std::string& getSosTypeString(SosKind k) const { return sosTypeMap.at(k); }

private:
    std::map<std::string, Gag> gags;
    std::map<std::string, Sos> soses;
    std::map<GagKind, std::string> gagTypeMap;
    std::map<SosKind, std::string> sosTypeMap;
};

#endif
