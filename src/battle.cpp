#include "battle.h"
#include "rang.h"
#include "colors.h"
#include <sstream>
#include <cmath>
#include <string>
#include <cctype>
#include <algorithm>

using namespace std;

Battle::Battle() { // default test run Battle
    queue<int> set;
    for (size_t repeat = 0; repeat < 3; ++repeat) {
        for (size_t i = 12; i < 15; ++i) {
            set.push(i);
        }
    }
    c = Cogset(set);
    position_definition["left"] = 0;
    position_definition["mid-left"] = 1;
    position_definition["mid"] = 1;
    position_definition["mid-right"] = 2;
    position_definition["right"] = c.getSize() - 1;
    gc = GagCollection();
}

Battle::Battle(queue<int> set) { // pre generated set
    c = Cogset(set);
    position_definition["left"] = 0;
    position_definition["mid-left"] = 1;
    position_definition["mid"] = 1;
    position_definition["mid-right"] = 2;
    position_definition["right"] = c.getSize() - 1;
    gc = GagCollection();
}

Battle::Battle(vector<Cog> set) { // pre generated set
    c = Cogset(set);
    position_definition["left"] = 0;
    position_definition["mid-left"] = 1;
    position_definition["mid"] = 1;
    position_definition["mid-right"] = 2;
    position_definition["right"] = c.getSize() - 1;
    gc = GagCollection();
}

void Battle::turn(Strategy strat) {
    vector<DirectedGag> toonup;
    vector<DirectedGag> trap;
    vector<DirectedGag> lure;
    vector<DirectedGag> sound;
    vector<DirectedGag> squirt;
    vector<DirectedGag> zap;
    vector<DirectedGag> thro;
    vector<DirectedGag> drop;
    /*for (size_t i = 0; i < c.getSize(); ++i) {
        //cout << damages[i] << endl;
        c.getCog(i).soak();
    }*/
    for (size_t i = 0; i < strat.gags.size(); ++i) {
        const Gag& g = strat.gags[i];
        DirectedGag d_gag;
        d_gag.damage = g.damage;
        d_gag.target = strat.positions[i];
        if (g.kind == GagKind::TOONUP) {
            toonup.push_back(d_gag);
        } else if (g.kind == GagKind::TRAP) {
            trap.push_back(d_gag);
        } else if (g.kind == GagKind::LURE) {
            lure.push_back(d_gag);
        } else if (g.kind == GagKind::SOUND) {
            sound.push_back(d_gag);
        } else if (g.kind == GagKind::SQUIRT) {
            squirt.push_back(d_gag);
        } else if (g.kind == GagKind::ZAP) {
            zap.push_back(d_gag);
        } else if (g.kind == GagKind::THROW) {
            thro.push_back(d_gag);
        } else if (g.kind == GagKind::DROP) {
            drop.push_back(d_gag);
        }
    }
    if (strat.config == 1) {
        sort(zap.begin(), zap.end(), CrossDirectedGagComparator());
    } else if (strat.config == 0) {
        sort(zap.begin(), zap.end(), DirectedGagComparator());
    } else {
        reverse(zap.begin(), zap.end());
    }
    if (!lure.empty()) {
        lureC(lure);
    }
    if (!squirt.empty()) {
        sqattack(squirt);
    }
    if (!zap.empty()) {
        zattack(zap);
    }
    c.load();
    position_definition["right"] = c.getSize() - 1;
}

Strategy Battle::parse_oneliner(string strat) {
    vector<Gag> gags;
    vector<int> director;
    stringstream ss(strat);
    string parser;
    size_t config = 0;
    // each loop parses a gag+target or a multi zap combo
    while (ss >> parser) {
        int multiplier = 1;
        size_t quickhand = 0;
        if (!gc.contains(parser)) { // command specifies strategy first (multiple gags or quickhand combo)
            if (parser.size() == 1 && isdigit(parser[0])) { // multiple gag
                multiplier = stoi(parser);
            } else if (parser.size() == c.getSize()) { // might be a quickhand strategy callout in x and -
                for (size_t i = 0; i < c.getSize(); ++i) {
                    if (parser[i] == 'x') {
                        director.push_back(i);
                        ++quickhand;
                    } else if (parser[i] == 'X') {
                        director.push_back(i);
                        director.push_back(i);
                        quickhand += 2;
                    } else if (parser[i] != '-') {
                        throw invalid_argument("Unrecognized quickhand strategy!");
                    }
                }
            } else {
                throw invalid_argument("Unrecognized gag/command!");
            }
            // check if followed with gag
            if ((!(ss >> parser) || !gc.contains(parser)) && parser != "cross") {
                throw invalid_argument("Please supply the respective gag following your strategy.");
            }
            if (parser == "cross") {
                config = 1;
                ss >> parser;
            }
        }
        // command should now specify a gag
        if (quickhand) { // quickhand combo
            for (size_t i = 0; i < quickhand; ++i) {
                if (gc.contains(parser)) {
                    const Gag& g = gc.get(parser);
                    gags.push_back(g);
                    if (i != quickhand - 1) {
                        ss >> parser;
                    }
                } else {
                    throw invalid_argument("Unrecognized gag!");
                }
            }
        } else { // parse gag, then the target following it
            const Gag& g = gc.get(parser);

            if ((g.kind == GagKind::LURE && g.name.find("dollar") == string::npos) || g.kind == GagKind::SOUND) {
                director.push_back(-1); // placeholder for lure/sound
            } else if (c.getSize() == 1) {
                director.push_back(0);
            } else {
                ss >> parser;
                if (position_definition.find(parser) != position_definition.end()) { // word position check
                    for (int i = 0; i < multiplier; ++i) {
                        director.push_back(position_definition[parser]);
                    }
                } else { // cog level position check
                    bool directed = false;
                    for (size_t i = 0; i < c.getSize(); i++) {
                        if (c.getCog(i).getLevelName() == parser) {
                            for (int j = 0; j < multiplier; ++j) {
                                director.push_back(i);
                            }
                            directed = true;
                            break;
                        }
                    }
                    if (!directed) {
                        throw invalid_argument("Position not supplied for " + g.name);
                    }
                }
            }
            for (int i = 0; i < multiplier; ++i) {
                gags.push_back(g);
            }
        }
    }
    Strategy strategy;
    strategy.gags = gags;
    strategy.positions = director;
    strategy.config = config;
    return strategy;
}

Strategy Battle::parse_gags(string strat) {
    vector<Gag> gags;
    vector<int> director;
    string gag;
    string target;
    size_t numtoons = 4;
    for (size_t i = 1; i <= numtoons; ++i) {
        cout << PROMPT << "Toon " << i << ": " << rang::style::reset;
        cin >> gag;
        if (gag != "PASS") {
            cin >> target;
            if (!gc.contains(gag) && (target.size() == 1 || !isdigit(target[0])) && position_definition.find(target) == position_definition.end()) {
                cerr << "Unrecognized command!" << endl;
                --i;
            } else {
                gags.push_back(gc.get(gag));
                if (target.size() == 1 && isdigit(target[0])) {
                    director.push_back(stoi(target));
                } else {
                    director.push_back(position_definition[target]);
                }
            }
        } else {
            gags.push_back(Gag());
            director.push_back(-1);
        }
    }
    Strategy strategy;
    strategy.gags = gags;
    strategy.positions = director;
    strategy.config = 2;
    return strategy;
}

void Battle::main(size_t config) {
    string strat;
    position_definition["right"] = c.getSize() - 1;
    while (c.getSize() != 0) {
        for (size_t i = 0; i < c.getSize(); ++i) {
            cout << c.getCog(i) << "\t\t";
        }
        cout << endl;
        do {
            try {
                Strategy strategy;
                if (config == 0) { // one liner
                    cout << PROMPT << "Your strategy: " << rang::style::reset;
                    getline(cin, strat);
                    strategy = parse_oneliner(strat);
                } else { // individual toon directing
                    strategy = parse_gags(strat);
                }
                if (strat == "END") {
                    cout << "Force stop" << endl;
                    break;
                }
                turn(strategy);
                // print cogs
                /*cout << "Final result: ";
                for (size_t i = 0; i < c.getSize(); ++i) {
                    cout << c.getCog(i) << "\t\t";
                }
                cout << endl;*/
                break;
            } catch (const invalid_argument& e) {
                cerr << e.what() << endl;
            }
        } while (true);
        if (strat == "END") {
            break;
        }
    }
    if (c.getSize() == 0) {
        cout << "You did it!" << endl;
    }
}

void Battle::lureC(vector<DirectedGag> lures) {
    for (DirectedGag g : lures) {
        if (g.target == -1) {
            for (size_t i = 0; i < c.getSize(); ++i) {
                c.getCog(i).lure();
            }
            break;
        }
        c.getCog(g.target).lure();
    }
    cout << LURED << "Lure" << rang::style::reset << " - ";
    for (size_t i = 0; i < c.getSize(); ++i) {
        cout << c.getCog(i) << "\t\t";
    }
    cout << endl;
}

void Battle::sqattack(vector<DirectedGag> squirts) {
    vector<int> damages;
    vector<bool> multi;
    damages.assign(c.getSize(), 0);
    multi.assign(c.getSize(), false);
    for (DirectedGag g : squirts) {
        if (damages[g.target]) {
            multi[g.target] = true;
        }
        damages[g.target] += g.damage; // accumulate raw damage
        if (g.target > 0) {
            c.getCog(g.target-1).soak();
        }
        if (g.target < (int)c.getSize() - 1) {
            c.getCog(g.target+1).soak();
        }
        c.getCog(g.target).soak();
    }
    cout << SOAKED << "Squirt" << rang::style::reset << " - ";
    for (size_t i = 0; i < c.getSize(); ++i) {
        //cout << damages[i] << endl;
        Cog& cog = c.getCog(i);
        if (cog.getLured()) {
            cog.hit(ceil(damages[i] * 0.65));
            cog.unlure();
        }
        if (multi[i]) {
            cog.hit(ceil(damages[i] * 0.2));
        }
        cog.hit(damages[i]);
        cout << c.getCog(i) << "\t\t";
    }
    cout << endl;
}

void Battle::zattack(vector<DirectedGag> zaps) {
    vector<bool> jumped;
    jumped.assign(c.getSize(), false);

    //bool jumped[4] = {false, false, false, false};
    /*for (size_t i = 0; i < c.getSize(); ++i) {
        if (!c.getCog(i).getSoaked()) {
            jumped[i] = true;
        }
    }*/
    vector<bool> soaked;
    for (size_t i = 0; i < c.getSize(); ++i) {
        soaked.push_back(c.getCog(i).getSoaked());
    }
    vector<int> damages;
    damages.assign(c.getSize(), 0);
    //sort(zaps.begin(), zaps.end(), DirectedGagComparator());
    for (DirectedGag g : zaps) {
        vector<bool> examined;
        examined.assign(c.getSize(), false);
        size_t jump_count = 0;
        int examine_count = 0;
        int targ = g.target;
        int dir = -1;
        int lasttarg = -1;
        while (jump_count < 3 && examine_count < (int)c.getSize() && (lasttarg == -1 || abs(targ - lasttarg) <= 2)) {
            /*if (lasttarg != -1 && abs(targ - lasttarg) > 2) {
                break;
            }*/
            if (jump_count == 0 && !soaked[targ]) { // starting on a dry cog
                damages[targ] += g.damage;
                break;
            }
            if ((jump_count == 0 || !jumped[targ]) && soaked[targ]) {
                damages[targ] += ceil((3 - jump_count*0.5) * g.damage);
                jumped[targ] = true;
                examined[targ] = true;
                lasttarg = targ;
                ++jump_count;
                ++examine_count;
            } else if (targ != g.target && (jumped[targ] || !soaked[targ]) && !examined[targ]) {
                ++examine_count;
                examined[targ] = true;
            }
            if (targ == 0) {
                dir = 1;
            } else if (targ == (int)(c.getSize() - 1)) {
                dir = -1;
            }
            targ += dir;
        }
        jumped[g.target] = false;
    }
    cout << ZAPPED << "Zap" << rang::style::reset << " - ";
    for (size_t i = 0; i < c.getSize(); ++i) {
        //cout << damages[i] << endl;
        c.getCog(i).hit(damages[i]);
        cout << c.getCog(i) << "\t\t";
    }
    cout << endl;
}
