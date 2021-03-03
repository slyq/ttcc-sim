#ifndef COG_H
#define COG_H

#include <iostream>
#include <string>

class Cog {
public:
    Cog() : level(0), hp(0), maxHP(0), trapped(0), luredKnockback(0), luredRounds(0), presLured(false), currLureMax(0), soaked(0), toUnsoak(false), executive(false), defense(0), version(0) {}
    Cog(int lvl) : Cog(lvl, false, 0) {}
    Cog(int lvl, bool exe, size_t ver)
            : level(lvl),
              hp((lvl + 1) * (lvl + 2) * (exe ? 1.5 : 1)),
              maxHP((lvl + 1) * (lvl + 2) * (exe ? 1.5 : 1)),
              trapped(0),
              luredKnockback(0),
              luredRounds(0),
              presLured(false),
              currLureMax(0),
              soaked(0),
              toUnsoak(false),
              executive(exe),
              defense(lvl == 1 ? 2 : (lvl >= 14 ? (exe ? 70 : 65) : ((lvl + exe - 1) * 5))),
              version(ver) {}
    Cog(std::string lvl)
            : Cog(std::stoi(lvl.substr(0, lvl.find("."))),
                  lvl.find(".exe") != std::string::npos,
                  lvl.find("v") != std::string::npos ? std::stoi(lvl.substr(lvl.find("v") + 1)) : 0) {}
    ~Cog() {}
    Cog& operator=(const Cog& other);
    int getHP() const { return hp; }
    int getLevel() const { return level; }
    std::string getLevelName() const;
    void hit(int damage);
    void setTrap(int trap) { if (!trapped) trapped = trap; }
    void untrap() { trapped = 0; }
    int getTrap() const { return trapped; }
    void setLured(int state, bool pres, int bonus = 0);
    void unlure();
    int getLured() const { return luredRounds; }
    int getLuredKnockback() const { return luredKnockback; }
    bool getPresLured() const { return presLured; };
    void setSoaked(int state);
    void reduceSoaked(int state);
    int getSoaked() const { return soaked; }
    int getDefense() const { return defense; }
    void update();
    int getLureAccCap() const;
    size_t getPrintSize() const;
    friend std::ostream& operator<<(std::ostream& out, const Cog& cog);

protected:
    int level;
    int hp;
    int maxHP;
    int trapped;
    int luredKnockback;
    int luredRounds;
    bool presLured;
    int currLureMax;
    int soaked;
    bool toUnsoak;
    bool executive;
    int defense;
    size_t version;
};

#endif