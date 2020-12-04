#ifndef COG_H
#define COG_H

#include <iostream>
#include <string>

class Cog {
public:
    Cog() : level(0), hp(0), trapped(0), lured(0), soaked(0), executive(false) {}
    Cog(int lvl) : Cog(lvl, false) {}
    Cog(int lvl, bool exe)
            : level(lvl),
              hp((lvl + 1) * (lvl + 2) * (exe ? 1.5 : 1)),
              trapped(0),
              lured(0),
              soaked(0),
              executive(exe) {}
    Cog(std::string lvl) : Cog(std::stoi(lvl.substr(0, lvl.find("."))), lvl.find(".exe") != std::string::npos) {}
    ~Cog() {}
    Cog& operator=(const Cog& other);
    int getHP() const { return hp; }
    int getLevel() const { return level; }
    std::string getLevelName() const;
    void hit(int damage);
    void setTrap(int trap) { trapped = trap; }
    void untrap() { trapped = 0; }
    int getTrap() const { return trapped; }
    void setLured(int state);
    void unlure() { lured = 0; }
    int getLured() const { return lured; }
    void setSoaked(int state);
    int getSoaked() const { return soaked; }
    void update();
    size_t getPrintSize() const;
    friend std::ostream& operator<<(std::ostream& out, const Cog& cog);

protected:
    int level;
    int hp;
    int trapped;
    int lured;
    int soaked;
    bool executive;
};

#endif
