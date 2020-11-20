#ifndef COG_H
#define COG_H

#include <iostream>
#include <string>

class Cog {
    public:
        Cog();
        Cog(int lvl);
        Cog(int lvl, bool exe);
        ~Cog() {}
        Cog& operator=(const Cog& other);
        int getHP() { return hp;}
        int getLevel() { return level; }
        std::string getLevelName();
        void hit(int damage);
        void setTrap(int trap) { trapped = trap; }
        void untrap() { trapped = 0; }
        int getTrap() { return trapped; }
        void setLured(int state) { lured = state; }
        void unlure() { lured = 0; }
        int getLured() { return lured; }
        void soak() { soaked = true; }
        bool getSoaked() { return soaked; }
        friend std::ostream& operator<<(std::ostream& out, const Cog& cog);
    protected:
        int hp;
        int level;
        int trapped;
        int lured;
        bool soaked;
        bool executive;
};

#endif
