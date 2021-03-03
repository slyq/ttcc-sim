#include "cog.h"

#include "colors.h"
#include "rang.h"

Cog& Cog::operator=(const Cog& other) {
    level = other.level;
    hp = other.hp;
    trapped = other.trapped;
    luredKnockback = other.luredKnockback;
    luredRounds = other.luredRounds;
    presLured = other.presLured;
    currLureMax = other.currLureMax;
    soaked = other.soaked;
    executive = other.executive;
    version = other.version;
    return *this;
}

std::string Cog::getLevelName() const {
    if (level == 0) return "";
    std::string name = std::to_string(level);
    if (executive) {
        name += ".exe";
    }
    if (version > 1) {
        name += "v" + std::to_string(version);
    } else if (version == 1) {
        name += "v^";
    }
    return name;
}

void Cog::hit(int damage) {
    hp -= damage;
    if (luredRounds) {
        unlure();
    }
    if (hp <= 0) {
        if (version == 2) {
            version = 1;
            hp = maxHP/2;
        } else {
            hp = 0;
        }
    }
}

void Cog::setLured(int state, bool pres, int bonus) {
    if (trapped) {
        trapped = 0;
        unlure();
    } else if (!luredRounds) {
        luredRounds = state;
        luredKnockback = (pres ? 65 : 50) + bonus;
        presLured = pres;
        currLureMax = state;
    }
}

void Cog::unlure() {
    luredKnockback = 0;
    luredRounds = 0;
    presLured = false;
    currLureMax = 0;
}

void Cog::setSoaked(int state) {
    if (!soaked) {
        defense -= 15;
        if (defense < 0) {
            defense = 0;
        }
    }
    // reset if higher than current
    if (state > soaked) {
        soaked = state;
    }
}

void Cog::reduceSoaked(int state) {
    if (soaked > 0) {
        soaked -= state;
        if (soaked < 0) {
            soaked = 0;
        }
        if (soaked == 0) {
            // cog is "queued" to be unsoaked at the start of next round
            // don't apply defense unbuff yet
            toUnsoak = true;
        }
    }
}

void Cog::update() {
    if (luredRounds) {
        // luredRounds += luredRounds > 0 ? -1 : 1;
        --luredRounds;
        if (!luredRounds) {
            unlure();
        }
    }
    if (soaked > 0 || toUnsoak) {
        if (soaked) {
            --soaked;
        }
        if (!soaked || toUnsoak) {
            toUnsoak = false;
            if (level == 1) {
                defense = 2;
            } else if (level >= 14) {
                defense = executive ? 70 : 65;
            } else {
                defense = (level + executive - 1) * 5;
            }
        }
    }
}

int Cog::getLureAccCap() const {
    if (!luredRounds)
        return 0;
    if (currLureMax - luredRounds == 0 || (currLureMax - luredRounds == 1 && presLured))
        return 100;
    return 100 - 5 * (currLureMax - luredRounds);
}

size_t Cog::getPrintSize() const {
    std::string buffer = std::to_string(soaked) + "Level " + getLevelName() + ": ";
    buffer += std::to_string(hp);
    if (hp) {
        if (trapped) {
            buffer += "(" + std::to_string(trapped) + ")";
        }
        if (luredRounds) {
            buffer += "[" + std::to_string(luredRounds) + "," + std::to_string(luredKnockback) + "]";
        }
        if (presLured) {
            buffer += "*";
        }
    }
    return buffer.size();
}

std::ostream& operator<<(std::ostream& out, const Cog& cog) {
    if ((cog.soaked || cog.toUnsoak) && cog.hp != 0) {
        out << SOAKED << cog.soaked;
    } else {
        out << " ";
    }
    if (cog.luredRounds && cog.hp != 0) {
        out << LURED << "[" << cog.luredRounds << "," << cog.luredKnockback << "]";
    }
    if (cog.hp == 0) {
        out << DEAD;
    }
    if (cog.level != 0) {
        out << "Level " << cog.getLevelName() << ": " << cog.hp;
    }
    out << rang::style::reset;
    if (cog.trapped && cog.hp != 0) {
        out << "(" << TRAPPED << cog.trapped << rang::style::reset << ")";
    }
    if (cog.presLured && cog.hp != 0) {
        out << PRESLURED;
    }
    return out;
}
