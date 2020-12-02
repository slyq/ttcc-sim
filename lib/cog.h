#ifndef COG_H
#define COG_H

#include <iostream>
#include <string>

class Cog {
public:
    Cog() : level(0), hp(0), trapped(0), lured(0), soaked(false), executive(false) {}
    Cog(int lvl) : Cog(lvl, false) {}
    Cog(int lvl, bool exe)
            : level(lvl),
              hp((lvl + 1) * (lvl + 2) * (exe ? 1.5 : 1)),
              trapped(0),
              lured(0),
              soaked(false),
              executive(exe) {}
    Cog(std::string lvl) : Cog(std::stoi(lvl.substr(0, lvl.find("."))), lvl.find(".exe") != std::string::npos) {}
    ~Cog() {}
    Cog& operator=(const Cog& other);
    int getHP() const { return hp; }
    int getLevel() const { return level; }
    std::string getLevelName();
    void hit(int damage);
    void setTrap(int trap) { trapped = trap; }
    void untrap() { trapped = 0; }
    int getTrap() const { return trapped; }
    void setLured(int state) { lured = state; }
    void unlure() { lured = 0; }
    int getLured() const { return lured; }
    void soak() { soaked = true; }
    bool getSoaked() const { return soaked; }
    friend std::ostream& operator<<(std::ostream& out, const Cog& cog);

protected:
    int level;
    int hp;
    int trapped;
    int lured;
    bool soaked;
    bool executive;
};

#endif
