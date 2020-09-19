#ifndef COG_H
#define COG_H

#include <iostream>
#include <string>

class Cog {
    public:
        Cog();
        Cog(int lvl);
        Cog(int lvl, bool exe);
        ~Cog();
        Cog& operator=(Cog const& other);
        int getHP();
        int getLevel();
        void hit(int damage);
        void lure();
        friend std::ostream& operator<<(std::ostream& out, const Cog& cog);
    protected:
        int hp;
        int level;
        bool soaked;
        bool lured;
        bool executive;
};

#endif
