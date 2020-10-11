#include "cog.h"

#include "rang.h"
#include "colors.h"

Cog::Cog() {
    level = 0;
    hp = 0;
    lured = false;
    soaked = false;
    jumped = false;
    executive = false;
}

Cog::Cog(int lvl) {
    level = lvl;
    hp = (lvl+1)*(lvl+2);
    lured = false;
    soaked = false;
    jumped = false;
    executive = false;
}

Cog::Cog(int lvl, bool exe) {
    level = lvl;
    hp = exe ? (lvl+1)*(lvl+2)*1.5 : (lvl+1)*(lvl+2);
    lured = false;
    soaked = false;
    jumped = false;
    executive = exe;
}

Cog& Cog::operator=(const Cog& other) {
    level = other.level;
    hp = other.hp;
    soaked = other.soaked;
    lured = other.lured;
    executive = other.executive;
    return *this;
}

std::string Cog::getLevelName() {
    if (level == 0) {
        return "";
    } else if (executive) {
        return std::to_string(level) + ".exe";
    }
    return std::to_string(level);
}

void Cog::hit(int damage) { // deal raw damage
    hp -= damage;
    if (hp < 0) {
        hp = 0;
    }
}

std::ostream& operator<<(std::ostream& out, const Cog& cog) {
    if (cog.soaked) {
        out << SOAKED;
    }
    if (cog.lured) {
        out << LURED;
    }
    if (cog.hp == 0) {
        out << DEAD;
    }
    if (cog.level == 0) {
    } else if (cog.executive) {
        out << "Level " << std::to_string(cog.level) << ".exe: " << cog.hp;
    } else {
        out << "Level " << std::to_string(cog.level) << ": " << cog.hp;
    }
    out << rang::style::reset;
    return out;
}
