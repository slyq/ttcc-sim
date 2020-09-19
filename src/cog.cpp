#include "cog.h"

Cog::Cog() {
    level = 0;
    hp = 0;
    soaked = false;
    lured = false;
    executive = false;
}

Cog::Cog(int lvl) {
    level = lvl;
    hp = (lvl+1)*(lvl+2);
    executive = false;
}

Cog::Cog(int lvl, bool exe) {
    level = lvl;
    hp = exe ? (lvl+1)*(lvl+2)*1.5 : (lvl+1)*(lvl+2);
    executive = exe;
}

Cog::~Cog() {

}

Cog& Cog::operator=(Cog const& other) {
    level = other.level;
    hp = other.hp;
    soaked = other.soaked;
    lured = other.lured;
    executive = other.executive;
    return *this;
}

int Cog::getHP() {
    return hp;
}

int Cog::getLevel() {
    return level;
}

void Cog::hit(int damage) { // deal raw damage
    hp -= damage;
    if (hp < 0) {
        hp = 0;
    }
}

void Cog::lure() {
    lured = true;
}

std::ostream& operator<<(std::ostream& out, const Cog& cog) {
    if (cog.level == 0) {
    } else if (cog.executive) {
        out << "Level " << std::to_string(cog.level) << ".exe: " << cog.hp;
    } else {
        out << "Level " << std::to_string(cog.level) << ": " << cog.hp;
    }
    return out;
}
