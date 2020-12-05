#include "cog.h"

#include "colors.h"
#include "rang.h"

Cog& Cog::operator=(const Cog& other) {
    level = other.level;
    hp = other.hp;
    trapped = other.trapped;
    lured = other.lured;
    soaked = other.soaked;
    executive = other.executive;
    return *this;
}

std::string Cog::getLevelName() const {
    if (level == 0) {
        return "";
    } else if (executive) {
        return std::to_string(level) + ".exe";
    }
    return std::to_string(level);
}

void Cog::hit(int damage) {  // deal raw damage
    hp -= damage;
    if (hp < 0) {
        hp = 0;
    }
}

void Cog::setLured(int state) {
    if (trapped) {
        trapped = 0;
    } else {
        lured = state;
    }
}

void Cog::setSoaked(int state) {
    // reset if higher than current
    if (state > soaked) {
        soaked = state;
    }
}

void Cog::update() {
    if (lured) {
        lured += lured > 0 ? -1 : 1;
    }
    if (soaked > 0) {
        --soaked;
    }
}

size_t Cog::getPrintSize() const {
    std::string buffer = "Level " + getLevelName() + ": ";
    buffer += std::to_string(hp);
    if (hp) {
        if (trapped) {
            buffer += "(" + std::to_string(trapped) + ")";
        }
        if (lured > 0) {
            buffer += "*";
        }
    }
    return buffer.size();
}

std::ostream& operator<<(std::ostream& out, const Cog& cog) {
    if (cog.soaked && cog.hp != 0) {
        out << SOAKED;
    }
    if (cog.lured && cog.hp != 0) {
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
    if (cog.trapped && cog.hp != 0) {
        out << "(" << TRAPPED << cog.trapped << rang::style::reset << ")";
    }
    if (cog.lured > 0 && cog.hp != 0) {
        out << PRESLURED;
    }
    return out;
}
