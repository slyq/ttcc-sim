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
        void lure() { lured = true; }
        bool getLured() { return lured; }
        void soak() { soaked = true; }
        bool getSoaked() { return soaked; }
        friend std::ostream& operator<<(std::ostream& out, const Cog& cog);
    protected:
        int hp;
        int level;
        bool lured;
        bool soaked;
        bool jumped;
        bool executive;
};

#endif
