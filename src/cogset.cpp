#include "cogset.h"

#include "colors.h"
#include "rang.h"
#include <algorithm>
#include <time.h>
#include <cmath>
#include <sstream>
#include <string>

#define PADDING 28

using namespace std;

Cogset::Cogset(queue<int> set) : q(set) {
    srand(time(NULL));
    int starting_size = rand() % 4 + 1;
    // for (int i = 3; i >= 0; --i) { // ceo
    for (int i = 0; i < starting_size; ++i) {  // other boss battles
        if (!q.empty()) {
            if (rand() % 100 + 1 < EXE_CHANCE) {
                cogs.insert(cogs.begin(), Cog(q.front(), true));
            } else {
                cogs.insert(cogs.begin(), Cog(q.front()));
            }
            q.pop();
        }
    }
}

Cogset& Cogset::operator=(const Cogset& other) {
    q = other.q;
    cogs = other.cogs;
    return *this;
}

void Cogset::load() {
    // remove dead cogs
    cogs.erase(remove_if(cogs.begin(), cogs.end(), [](Cog& c) { return c.getHP() == 0; }), cogs.end());
    if (cogs.size() != 4) {
        // attempt to replace dead cogs
        while (!q.empty() && cogs.size() < 4) {
            if (rand() % 100 + 1 < EXE_CHANCE) {
                cogs.insert(cogs.begin(), Cog(q.front(), true));
            } else {
                cogs.insert(cogs.begin(), Cog(q.front()));
            }
            q.pop();
        }
    }
}

void Cogset::update() {
    if (roundUpdate) {
        for (Cog& c : cogs) {
            c.update();
        }
    }
}

Cog& Cogset::getCog(int pos) {
    if (pos >= (int)cogs.size()) {
        throw invalid_argument("Out of bounds");
    }
    return cogs[pos];
}

bool Cogset::allDead() const {
    bool allDead = true;
    for (size_t i = 0; (i < cogs.size() && allDead); ++i) {
        if (cogs[i].getHP() != 0) {
            allDead = false;
        }
    }
    return allDead;
}

void Cogset::attack(const vector<int>& affected, char type) {
    bool activity = false;
    for (int i : affected) {
        if (i) {
            activity = true;
            break;
        }
    }
    if (activity) {
        if (printCogset) {
            cout << "\t";
        }
        if (affected.size() == cogs.size()) {
            for (size_t i = 0; i < cogs.size(); ++i) {
                if (affected[i]) {
                    cogs[i].hit(affected[i]);
                    if (cogs[i].getLured()) {
                        cogs[i].unlure();
                    }
                    if (printCogset) {
                        switch (type) {
                        case 0:
                            cout << DAMAGE;
                            break;
                        case 1:
                            cout << KNOCKBACK;
                            break;
                        case 2:
                            cout << COMBO;
                            break;
                        default:
                            break;
                        }
                        cout << affected[i] << " " << rang::style::reset << string(PADDING - 1 - (int)log10(affected[i]) - 1, ' ');
                    }
                } else {
                    if (printCogset) {
                        cout << string(PADDING, ' ');
                    }
                }
            }
        }
        if (printCogset) {
            cout << endl;
        }
    }
}

void Cogset::attack(const vector<int>& affected) {
    attack(affected, 0);
}

void Cogset::apply(const vector<int>& affected, GagKind gk) {
    if (affected.size() == cogs.size()) {
        for (size_t i = 0; i < cogs.size(); ++i) {
            if (affected[i]) {
                if (gk == GagKind::TRAP) {
                    cogs[i].setTrap(affected[i]);
                } else if (gk == GagKind::LURE) {
                    cogs[i].setLured(affected[i]);
                } else if (gk == GagKind::SQUIRT) {
                    cogs[i].setSoaked(affected[i]);
                }
            }
        }
    }
}

void Cogset::gagCheck(const Gag& gagchoice) const {
    if (gagchoice.target >= (int)cogs.size()) {
        throw invalid_argument("Out of bounds");
    }
    size_t trapcount = 0;
    size_t lurecount = 0;
    for (size_t i = 0; i < cogs.size(); ++i) {
        if (cogs[i].getTrap()) {
            ++trapcount;
        }
        if (cogs[i].getLured()) {
            ++lurecount;
        }
    }
    if (gagchoice.kind == GagKind::TRAP) {
        if (gagchoice.target == -1) {
            // note: the checks below deviate from the true game experience, which is buggy with trap SOS
            if (trapcount == cogs.size()) {
                throw invalid_argument("Cannot place an SOS trap when all cogs are already trapped");
            } else if (lurecount == cogs.size()) {
                throw invalid_argument("Cannot place an SOS trap when all cogs are already lured");
            } else if (trapcount + lurecount == cogs.size()) {
                throw invalid_argument("Cannot place an SOS trap when all cogs are already trapped or lured");
            }
        } else {
            if (cogs[gagchoice.target].getTrap()) {
                throw invalid_argument("Cannot trap an already trapped cog");
            } else if (cogs[gagchoice.target].getLured()) {
                throw invalid_argument("Cannot trap an already lured cog");
            }
        }
    } else if (gagchoice.kind == GagKind::LURE) {
        if (gagchoice.target == -1 && lurecount == cogs.size()) {
            throw invalid_argument("Cannot group lure when all cogs are already lured");
        } else if (gagchoice.target != -1 && cogs[gagchoice.target].getLured()) {
            throw invalid_argument("Cannot lure an already lured cog");
        }
    }
}

void Cogset::fireTurn(const vector<Gag>& fires) {
    vector<int> fired(cogs.size(), 0);
    for (const Gag& g : fires) {
        fired[g.target] = cogs[g.target].getHP();
    }
    attack(fired);
    if (printCogset) {
        cout << "Fire" << rang::style::reset << "\t";
        print(fired);
    }
}

void Cogset::trapTurn(const vector<Gag>& traps) {
    vector<int> setups(cogs.size(), 0);
    for (const Gag& g : traps) {
        if (g.target == -1) {
            // sos gag - hits all
            for (size_t i = 0; i < setups.size(); ++i) {
                if (setups[i]) {
                    // trap conflict - all traps become null
                    for (size_t i = 0; i < setups.size(); ++i) {
                        setups[i] = -1;
                    }
                    break;
                } else if (cogs[i].getTrap() || cogs[i].getLured()) {
                    setups[i] = -1;
                } else if (cogs[i].getHP() != 0) {
                    setups[i] = g.damage;
                    // setups[i] = g.prestiged ? g.damage + cogs[i].getLevel() * 3 : g.damage;
                }
            }
        } else if (cogs[g.target].getHP() != 0) {
            if (setups[g.target] || cogs[g.target].getTrap() || cogs[g.target].getLured()) {
                // trap conflict - all traps targeting this cog become null
                setups[g.target] = -1;
            } else {
                setups[g.target] = g.prestiged ? g.damage + cogs[g.target].getLevel() * 3 : g.damage;
            }
        }
    }
    // set traps
    for (int& i : setups) {
        if (i == -1) {
            i = 0;
        }
    }
    apply(setups, GagKind::TRAP);
    if (printCogset) {
        cout << TRAPPED << "Trap" << rang::style::reset << "\t";
        print(setups);
    }
}

void Cogset::lureTurn(const vector<Gag>& lures) {
    vector<int> damages(cogs.size(), 0);
    vector<int> lured(cogs.size(), 0);
    // lure rounds can stack, and prestige overrides non-prestige
    for (const Gag& g : lures) {
        if (g.target == -1) {
            // group lure
            for (size_t i = 0; i < cogs.size(); ++i) {
                if (cogs[i].getHP() != 0) {
                    if (!cogs[i].getLured()) {
                        if (g.prestiged) {
                            lured[i] = abs(lured[i]) + g.passiveEffect;
                        } else {
                            lured[i] += lured[i] > 0 ? g.passiveEffect : -1*g.passiveEffect;
                        }
                        if (cogs[i].getTrap()) {
                            damages[i] = cogs[i].getTrap();
                        }
                    }
                }
            }
        } else if (cogs[g.target].getHP() != 0) {
            if (!cogs[g.target].getLured()) {
                if (g.prestiged) {
                    lured[g.target] = abs(lured[g.target]) + g.passiveEffect;
                } else {
                    lured[g.target] += lured[g.target] > 0 ? g.passiveEffect : -1*g.passiveEffect;
                }
                if (cogs[g.target].getTrap()) {
                    damages[g.target] = cogs[g.target].getTrap();
                }
            }
        }
    }
    // add extra turn
    for (int& i : lured) {
        if (i > 0) {
            ++i;
        } else if (i < 0) {
            --i;
        }
    }
    // apply lure
    attack(damages);
    apply(lured, GagKind::LURE);
    if (printCogset) {
        cout << LURED << "Lure" << rang::style::reset << "\t";
        print(lured);
    }
}

void Cogset::soundTurn(const vector<Gag>& sounds) {
    // find max
    int maxlvl = -1;
    for (size_t i = 0; i < cogs.size(); ++i) {
        if (cogs[i].getLevel() > maxlvl) {
            maxlvl = cogs[i].getLevel();
        }
    }
    // get damages
    int damage = 0;
    for (const Gag& g : sounds) {
        // add raw damage
        damage += g.damage;
        // apply prestige bonus
        if (g.prestiged) {
            damage += (maxlvl + 1) / 2;
        }
    }

    // damage and print effect
    vector<int> affected(cogs.size(), damage);
    attack(affected);

    // apply multiple gag bonus
    if (sounds.size() > 1) {
        attack(vector<int>(cogs.size(), ceil(damage * 0.2)), 2);
    }

    if (printCogset) {
        cout << "Sound\t";
        print(affected);
    }
}

void Cogset::squirtTurn(const vector<Gag>& squirts) {
    vector<int> damages(cogs.size(), 0);
    vector<bool> multi(cogs.size(), false);
    vector<int> soakRounds(cogs.size(), 0);
    int targ = -1;
    bool sos = false;
    // soak rounds do not stack but can be reset to a higher number
    // same mechanic applies to surrounding cogs for prestige squirt
    for (const Gag& g : squirts) {
        int effectiveRounds = g.passiveEffect + 1 + g.prestiged;
        if (g.target == -1) {
            // sos gag - hits all
            for (size_t i = 0; i < damages.size(); ++i) {
                if (cogs[i].getHP() != 0) {
                    if (damages[i]) {
                        multi[i] = true;
                    }
                    damages[i] += g.damage;
                    if (effectiveRounds > soakRounds[i]) {
                        soakRounds[i] = effectiveRounds;
                    }
                }
            }
            sos = true;
        } else if (cogs[g.target].getHP() != 0) {
            // single target gag
            // obtain first hit target for sos combo
            if (targ == -1) {
                targ = g.target;
            }
            if (damages[g.target]) {
                multi[g.target] = true;
            }
            // accumulate raw damage
            damages[g.target] += g.damage;
            // handle soaking
            if (g.prestiged) {
                if (g.target - 1 >= 0 && cogs[g.target - 1].getHP() != 0 && effectiveRounds > soakRounds[g.target - 1]) {
                    soakRounds[g.target - 1] = effectiveRounds;
                }
                if (g.target + 1 < (int)cogs.size() && cogs[g.target + 1].getHP() != 0 && effectiveRounds > soakRounds[g.target + 1]) {
                    soakRounds[g.target + 1] = effectiveRounds;
                }
            }
            if (effectiveRounds > soakRounds[g.target]) {
                soakRounds[g.target] = effectiveRounds;
            }
        }
    }
    // damage and print effect
    apply(soakRounds, GagKind::SQUIRT);
    vector<int> knockback(cogs.size(), 0);
    vector<int> combo(cogs.size(), 0);
    for (size_t i = 0; i < cogs.size(); ++i) {
        if (damages[i]) {
            if (cogs[i].getLured() > 0) {
                // pres lure knockback
                knockback[i] = ceil(damages[i] * 0.65);
            } else if (cogs[i].getLured() < 0) {
                // lure knockback
                knockback[i] = ceil(damages[i] * 0.5);
            }
            if (sos && targ != -1) {
                combo[i] = ceil(damages[targ] * 0.2);
            } else if (multi[i]) {
                // combo bonus
                combo[i] = ceil(damages[i] * 0.2);
            }
        }
    }
    attack(damages);
    attack(knockback, 1);
    attack(combo, 2);
    if (printCogset) {
        cout << SOAKED << "Squirt" << rang::style::reset << "\t";
        print(damages);
    }
}

void Cogset::zapTurn(const vector<Gag>& zaps) {
    vector<vector<int>> allDamages;
    // keep track of cogs that have been jumped in the same turn
    vector<bool> jumped(cogs.size(), false);
    // obtain soaked cogs valid for zapping
    vector<bool> soaked;
    for (size_t i = 0; i < cogs.size(); ++i) {
        soaked.push_back(cogs[i].getSoaked());
    }
    for (const Gag& g : zaps) {
        vector<int> damages(cogs.size(), 0);
        if (g.target == -1) {
            for (size_t i = 0; i < damages.size(); ++i) {
                damages[i] += g.damage * 3;
            }
            // TODO: double check how sos zap + regular zap work
        } else {
            // examine each zap's effect on all cogs (avoid recalculating on the same cog)
            vector<bool> examined;
            examined.assign(cogs.size(), false);
            size_t jump_count = 0;
            int examine_count = 0;
            int targ = g.target;
            char dir = -1;      // -1 left, 1 right
            int lasttarg = -1;  // keeps track of last cog hit (zap cannot jump more than two spaces)
            float dropoff = g.prestiged ? 0.5 : 0.75;
            while (jump_count < 3 && examine_count < (int)cogs.size() && (lasttarg == -1 || abs(targ - lasttarg) <= 2)) {
                // keep checking until jump count limit reached, all cogs examined, or zap fails to jump
                if (jump_count == 0 && (!soaked[targ] || cogs[targ].getHP() == 0)) {  // starting on a dry/dead cog
                    if (cogs[targ].getHP() != 0) {
                        damages[targ] += g.damage;
                    }
                    break;
                }
                if ((jump_count == 0 || !jumped[targ]) && soaked[targ] && cogs[targ].getHP() != 0) {  // zappable cog
                    damages[targ] += ceil((3 - jump_count * dropoff) * g.damage);
                    jumped[targ] = true;
                    examined[targ] = true;
                    lasttarg = targ;
                    ++jump_count;
                    ++examine_count;
                } else if ((cogs[targ].getHP() == 0 || (targ != g.target && (jumped[targ] || !soaked[targ]))) && !examined[targ]) {  // exhausted cog
                    ++examine_count;
                    examined[targ] = true;
                }
                // change direction if necessary
                if (targ == 0) {
                    dir = 1;
                } else if (targ == (int)(cogs.size() - 1)) {
                    dir = -1;
                }
                targ += dir;
            }
            // undo "jumped" for the targeted cog
            jumped[g.target] = false;
        }
        allDamages.push_back(damages);
    }
    // damage and print effect
    vector<int> sumDamages(cogs.size(), 0);
    for (const vector<int>& d : allDamages) {
        attack(d);
        for (size_t i = 0; i < d.size(); ++i) {
            // cout << d[i] << " ";
            sumDamages[i] += d[i];
        }
        // cout << endl;
    }
    if (printCogset) {
        cout << ZAPPED << "Zap" << rang::style::reset << "\t";
        print(sumDamages);
    }
}

void Cogset::throwTurn(const vector<Gag>& throws) {
    vector<int> damages(cogs.size(), 0);
    vector<bool> multi(cogs.size(), false);
    int targ = -1;
    bool sos = false;
    for (const Gag& g : throws) {
        if (g.target == -1) {
            // sos gag - hits all
            for (size_t i = 0; i < damages.size(); ++i) {
                if (cogs[i].getHP() != 0) {
                    if (damages[i]) {
                        multi[i] = true;
                    }
                    damages[i] += g.damage;
                }
                // damages[i] += g.prestiged ? ceil(g.damage * 1.15) : g.damage;
            }
            sos = true;
        } else if (cogs[g.target].getHP() != 0) {
            // single target gag
            // obtain first hit target for sos combo
            if (targ == -1) {
                targ = g.target;
            }
            if (damages[g.target]) {
                multi[g.target] = true;
            }
            // accumulate raw damage
            damages[g.target] += g.prestiged ? ceil(g.damage * 1.15) : g.damage;
        }
    }
    // damage and print effect
    vector<int> knockback(cogs.size(), 0);
    vector<int> combo(cogs.size(), 0);
    for (size_t i = 0; i < cogs.size(); ++i) {
        if (damages[i]) {
            if (cogs[i].getLured() > 0) {
                // pres lure knockback
                knockback[i] = ceil(damages[i] * 0.65);
            } else if (cogs[i].getLured() < 0) {
                // lure knockback
                knockback[i] = ceil(damages[i] * 0.5);
            }
            if (sos && targ != -1) {
                combo[i] = ceil(damages[targ] * 0.2);
            } else if (multi[i]) {
                // combo bonus
                combo[i] = ceil(damages[i] * 0.2);
            }
        }
    }
    attack(damages);
    attack(knockback, 1);
    attack(combo, 2);
    if (printCogset) {
        cout << THROWN << "Throw" << rang::style::reset << "\t";
        print(damages);
    }
}

void Cogset::dropTurn(const vector<Gag>& drops) {
    vector<int> damages(cogs.size(), 0);
    vector<int> multi(cogs.size(), 0);
    int targ = -1;
    bool sos = false;
    for (const Gag& g : drops) {
        if (g.target == -1) {
            // sos gag - hits all
            for (size_t i = 0; i < damages.size(); ++i) {
                if (cogs[i].getHP() != 0 && !cogs[i].getLured()) {
                    damages[i] += g.damage;
                    multi[i] += 10;
                }
            }
            sos = true;
        } else if (cogs[g.target].getHP() != 0 && !cogs[g.target].getLured()) {
            // single target gag
            // obtain first hit target for sos combo
            if (targ == -1) {
                targ = g.target;
            }
            // accumulate raw damage
            damages[g.target] += g.damage;
            if (g.prestiged) {
                multi[g.target] += 15;
            } else {
                multi[g.target] += 10;
            }
        }
    }
    // damage and print effect

    vector<int> combo(cogs.size(), 0);
    for (size_t i = 0; i < cogs.size(); ++i) {
        if (damages[i]) {
            if (sos && targ != -1) {
                // if sos + another drop, all cogs get hit with combo damage from first cog
                // if just sos, no bonus damage
                combo[i] = ceil(damages[targ] * (multi[targ] + 10) / 100.0);
            } else if (multi[i] > 15) {
                // combo bonus
                combo[i] = ceil(damages[i] * (multi[i] + 10) / 100.0);
            }
        }
    }
    attack(damages);
    attack(combo, 2);
    if (printCogset) {
        cout << DROPPED << "Drop" << rang::style::reset << "\t";
        print(damages);
    }
}

ostream& operator<<(ostream& out, const Cogset& cogset) {
    for (const Cog& cog : cogset.cogs) {
        out << cog << string(PADDING - cog.getPrintSize(), ' ');
    }
    return out;
}

void Cogset::print(const vector<int>& affected) const {
    if (printCogset && affected.size() == cogs.size()) {
        for (size_t i = 0; i < cogs.size(); ++i) {
            if (affected[i]) {
                cout << cogs[i] << ATTACKED << string(PADDING - 1 - cogs[i].getPrintSize(), ' ');
            } else {
                cout << cogs[i] << string(PADDING - cogs[i].getPrintSize(), ' ');
            }
        }
        cout << endl;
    }
}
