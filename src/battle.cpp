#include "battle.h"
#include "rang.h"
#include "colors.h"
#include <sstream>
#include <cmath>
#include <string>
#include <cctype>
#include <algorithm>
#include <stdlib.h>

using namespace std;

Battle::Battle() { // default test run Battle
    srand(time(NULL));
    gc = GagCollection::read(file_path);
    queue<int> set;
    size_t min_level = 7;
    size_t max_level = 17;
    if (rand() % 2 == 0) {
        set.push(rand() % (min_level/2) + min_level);
    }
    set.push(max_level);
    for (size_t repeat = 0; repeat < 3; ++repeat) {
        for (size_t i = min_level; i < max_level; ++i) {
            int random = rand() % 4;
            if (random > 0) {
                set.push(i);
            }
            if (random > 1) {
                set.push(i);
            }
        }
    }
    c = Cogset(set);
    position_definition["right"] = c.getSize() - 1;
}

Battle::Battle(queue<int> set) : c(Cogset(set)) { // pre generated set
    gc = GagCollection::read(file_path);
    position_definition["right"] = c.getSize() - 1;
}

Battle::Battle(vector<Cog> set) : c(Cogset(set)) { // pre generated set
    gc = GagCollection::read(file_path);
    position_definition["right"] = c.getSize() - 1;
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
        if (auto_pres) {
            d_gag.pres = true;
        } else {
            d_gag.pres = g.prestiged;
        }
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
    if (!sound.empty()) {
        soattack(sound);
    }
    if (!squirt.empty()) {
        sqattack(squirt);
    }
    if (!zap.empty()) {
        zattack(zap);
    }
    if (!thro.empty()) {
        thattack(thro);
    }
    if (!drop.empty()) {
        drattack(drop);
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
        bool pres = false;
        if (!gc.contains(parser)) { // command specifies strategy first (multiple gags or quickhand combo)
            if (parser.size() == 1 && isdigit(parser[0])) { // multiple gag
                multiplier = stoi(parser);
                ss >> parser;
                // check if followed with gag or pres
                if (!gc.contains(parser) && parser != "pres") {
                    throw invalid_argument("Please supply the respective gag following your strategy.");
                }
            }
            if (parser == "pres") {
                pres = true;
                ss >> parser;
            }
            if (parser.size() == c.getSize()) { // might be a quickhand strategy callout in x and -
                for (size_t i = 0; i < c.getSize(); ++i) {
                    if (parser[i] == 'x') {
                        director.push_back(i);
                        ++quickhand;
                    } else if (parser[i] == 'X') {
                        director.push_back(i);
                        director.push_back(i);
                        quickhand += 2;
                    } else if (parser[i] != '-') {
                        if (!gc.contains(parser)) {
                            throw invalid_argument("Unrecognized quickhand strategy!");
                        }
                        break;
                    }
                }
                // check if followed with gag, pres, or cross
                if (quickhand && !(ss >> parser)) {
                    throw invalid_argument("Missing arguments after quickhand strategy.");
                }
                if (!gc.contains(parser) && parser != "cross" && parser != "pres") {
                    throw invalid_argument("Wrong arguments after the quickhand strategy.");
                }
                if (parser == "cross") {
                    config = 1;
                    ss >> parser;
                }
            }/* else if (!gc.contains(parser) && parser != "cross") {
                // check if followed with gag
                throw invalid_argument("Please supply the respective gag following your strategy.");
            }*/ else if (!gc.contains(parser)) {
                throw invalid_argument("Unrecognized gag/command!");
            }
        }
        // command should now specify a gag
        if (quickhand) { // quickhand combo
            for (size_t i = 0; i < quickhand; ++i) {
                bool t_pres = false;
                if (pres) {
                    // (pres [quickhand]) --> everything following is pres
                    if (gc.contains(parser)) {
                        Gag g = gc.get(parser);
                        g.prestiged = true;
                        gags.push_back(g);
                        if (i != quickhand - 1) {
                            ss >> parser;
                        }
                    } else {
                        throw invalid_argument("Unrecognized gag following quickhand strat!");
                    }
                } else {
                    if (parser == "pres") {
                        // ([quickhand] pres [gag]) --> only next is determined pres
                        t_pres = true;
                        ss >> parser;
                    }
                    if (gc.contains(parser)) {
                        Gag g = gc.get(parser);
                        g.prestiged = t_pres;
                        gags.push_back(g);
                        if (i != quickhand - 1) {
                            ss >> parser;
                        }
                    } else {
                        throw invalid_argument("Unrecognized gag following quickhand strat!");
                    }
                    t_pres = false;
                }
            }
            pres = false;
        } else { // parse gag, then the target following it
            if (parser == "pres") {
                pres = true;
                ss >> parser;
            }
            Gag g = gc.get(parser);
            g.prestiged = pres;

            if ((g.kind == GagKind::LURE && g.name.find("dollar") == string::npos) || g.kind == GagKind::SOUND) {
                for (int i = 0; i < multiplier; ++i) {
                    director.push_back(-1); // placeholder for multi hit (lure/sound)
                }
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
    string gag_name;
    string target;
    size_t numtoons = 4;
    for (size_t i = 1; i <= numtoons; ++i) {
        Gag gag;
        bool pr = false;
        cout << PROMPT << "Toon " << i << ": " << rang::style::reset;
        cin >> gag_name;
        if (gag_name == "pres") {
            pr = true;
            cin >> gag_name;
        }
        while (!gc.contains(gag_name) && gag_name != "PASS") {
            cerr << "Unrecognized gag!" << endl;
            cout << PROMPT << "Toon " << i << ": " << rang::style::reset;
            cin >> gag_name;
            if (gag_name == "pres") {
                pr = true;
                cin >> gag_name;
            }
        }
        if (gag_name != "PASS") {
            gag = gc.get(gag_name);
            if ((gag.kind == GagKind::LURE && gag.name.find("dollar") == string::npos) || gag.kind == GagKind::SOUND) {
                director.push_back(-1);
            } else {
                // parse target
                while (true) {
                    cin >> target;
                    if ((target.size() == 1 && !isdigit(target[0])) || position_definition.find(target) == position_definition.end()) {
                        bool directed = false;
                        for (size_t i = 0; i < c.getSize(); i++) {
                            if (c.getCog(i).getLevelName() == target) {
                                director.push_back(i);
                                directed = true;
                                break;
                            }
                        }
                        if (directed) {
                            break;
                        }
                        cerr << "Unrecognized target!" << endl;
                        // --i; // parse again
                    } else {
                        //gag.accuracy = reference_gag.accuracy;
                        if (target.size() == 1 && isdigit(target[0])) {
                            director.push_back(stoi(target));
                        } else {
                            director.push_back(position_definition[target]);
                        }
                        break;
                    }
                }
            }
        } else {
            director.push_back(-1);
        }
        gag.prestiged = pr;
        gags.push_back(gag);
    }
    Strategy strategy;
    strategy.gags = gags;
    strategy.positions = director;
    strategy.config = 0;
    return strategy;
}

void Battle::lureC(vector<DirectedGag> lures) {
    vector<int> affected;
    for (DirectedGag g : lures) {
        if (g.target == -1) {
            for (size_t i = 0; i < c.getSize(); ++i) {
                if (!c.getCog(i).getLured()) {
                    if (g.pres) {
                        c.getCog(i).setLured(2);
                    } else {
                        c.getCog(i).setLured(1);
                    }
                    affected.push_back(i);
                }
            }
            break;
        } else {
            if (g.pres) {
                c.getCog(g.target).setLured(2);
            } else {
                c.getCog(g.target).setLured(1);
            }
            affected.push_back(g.target);
        }
    }
    cout << LURED << "Lure" << rang::style::reset << "\t";
    for (size_t i = 0; i < c.getSize(); ++i) {
        if (find(affected.begin(), affected.end(), i) != affected.end()) {
            cout << ATTACKED << c.getCog(i) << ATTACKED << "\t\t";
        } else {
            cout << NOTATTACKED << c.getCog(i) << NOTATTACKED << "\t\t";
        }
    }
    cout << endl;
}

void Battle::soattack(vector<DirectedGag> sounds) {
    // get raw damage
    int damage = 0;
    for (DirectedGag g : sounds) {
        damage += g.damage;
    }
    // apply prestige bonus
    int maxlvl = -1;
    for (size_t i = 0; i < c.getSize(); ++i) {
        if (c.getCog(i).getLevel() > maxlvl) {
            maxlvl = c.getCog(i).getLevel();
        }
    }
    for (DirectedGag g : sounds) {
        if (g.pres) {
            damage += ceil(maxlvl/2);
        }
    }
    // apply multiple gag bonus if applicable
    if (sounds.size() > 1) {
        damage += ceil(damage * 0.2);
    }
    // unlure all cogs
    for (size_t i = 0; i < c.getSize(); ++i) {
        if (c.getCog(i).getLured()) {
            c.getCog(i).unlure();
        }
    }
    // print
    cout << "Sound\t";
    for (size_t i = 0; i < c.getSize(); ++i) {
        Cog& cog = c.getCog(i);
        cog.hit(damage);
        cout << ATTACKED << cog << ATTACKED << "\t\t";
    }
    cout << endl;
}

void Battle::sqattack(vector<DirectedGag> squirts) {
    vector<int> damages(c.getSize(), 0);
    vector<bool> multi(c.getSize(), false);
    for (DirectedGag g : squirts) {
        if (damages[g.target]) {
            multi[g.target] = true;
        }
        damages[g.target] += g.damage; // accumulate raw damage
        if (g.pres) {
            if (g.target > 0) {
                c.getCog(g.target-1).soak();
            }
            if (g.target < (int)c.getSize() - 1) {
                c.getCog(g.target+1).soak();
            }
        }
        c.getCog(g.target).soak();
    }
    cout << SOAKED << "Squirt" << rang::style::reset << "\t";
    for (size_t i = 0; i < c.getSize(); ++i) {
        //cout << damages[i] << endl;
        Cog& cog = c.getCog(i);
        if (damages[i]) {
            cog.hit(damages[i]);
            if (cog.getLured() == 2) {
                cog.hit(ceil(damages[i] * 0.65));
                cog.unlure();
            } else if (cog.getLured() == 1) {
                cog.hit(ceil(damages[i] * 0.5));
                cog.unlure();
            }
            if (multi[i]) {
                cog.hit(ceil(damages[i] * 0.2));
            }
            cout << ATTACKED << cog << ATTACKED << "\t\t";
        } else {
            cout << NOTATTACKED << cog << NOTATTACKED << "\t\t";
        }
    }
    cout << endl;
}

void Battle::zattack(vector<DirectedGag> zaps) {
    vector<bool> jumped(c.getSize(), false);

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
    vector<int> damages(c.getSize(), 0);
    //sort(zaps.begin(), zaps.end(), DirectedGagComparator());
    for (DirectedGag g : zaps) {
        vector<bool> examined;
        examined.assign(c.getSize(), false);
        size_t jump_count = 0;
        int examine_count = 0;
        int targ = g.target;
        int dir = -1;
        int lasttarg = -1;
        float dropoff = g.pres ? 0.5 : 0.75;
        while (jump_count < 3 && examine_count < (int)c.getSize() && (lasttarg == -1 || abs(targ - lasttarg) <= 2)) {
            /*if (lasttarg != -1 && abs(targ - lasttarg) > 2) {
                break;
            }*/
            if (jump_count == 0 && !soaked[targ]) { // starting on a dry cog
                damages[targ] += g.damage;
                break;
            }
            if ((jump_count == 0 || !jumped[targ]) && soaked[targ]) {
                damages[targ] += ceil((3 - jump_count*dropoff) * g.damage);
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
    cout << ZAPPED << "Zap" << rang::style::reset << "\t";
    for (size_t i = 0; i < c.getSize(); ++i) {
        //cout << damages[i] << endl;
        Cog& cog = c.getCog(i);
        /*if (cog.getLured() && damages[i]) {
            cog.unlure();
        }*/
        cog.hit(damages[i]);
        if (damages[i]) {
            if (cog.getLured()) {
                cog.unlure();
            }
            cout << ATTACKED << cog << ATTACKED << "\t\t";
        } else {
            cout << NOTATTACKED << cog << NOTATTACKED << "\t\t";
        }
    }
    cout << endl;
}

void Battle::thattack(vector<DirectedGag> throws) {
    vector<int> damages(c.getSize(), 0);
    vector<bool> multi(c.getSize(), false);
    for (DirectedGag g : throws) {
        if (damages[g.target]) {
            multi[g.target] = true;
        }
        // accumulate raw damage
        damages[g.target] += g.pres ? g.damage*1.15 : g.damage;
    }
    cout << THROWN << "Throw" << rang::style::reset << "\t";
    for (size_t i = 0; i < c.getSize(); ++i) {
        //cout << damages[i] << endl;
        Cog& cog = c.getCog(i);
        if (damages[i]) {
            cog.hit(damages[i]);
            if (cog.getLured() == 2) {
                cog.hit(ceil(damages[i] * 0.65));
                cog.unlure();
            } else if (cog.getLured() == 1) {
                cog.hit(ceil(damages[i] * 0.5));
                cog.unlure();
            }
            if (multi[i]) {
                cog.hit(ceil(damages[i] * 0.2));
            }
            cout << ATTACKED << cog << ATTACKED << "\t\t";
        } else {
            cout << NOTATTACKED << cog << NOTATTACKED << "\t\t";
        }
    }
    cout << endl;
}

void Battle::drattack(vector<DirectedGag> drops) {
    vector<int> damages(c.getSize(), 0);
    vector<int> multi(c.getSize(), 0);
    for (DirectedGag g : drops) {
        // accumulate raw damage
        damages[g.target] += g.damage;
        multi[g.target]++;
    }
    cout << DROPPED << "Drop" << rang::style::reset << "\t";
    for (size_t i = 0; i < c.getSize(); ++i) {
        //cout << damages[i] << endl;
        Cog& cog = c.getCog(i);
        if (damages[i] && !cog.getLured()) {
            cog.hit(damages[i]);
            if (multi[i] > 1) {
                cog.hit(ceil(damages[i] * (multi[i] + 1) / 10.0));
            }
            cout << ATTACKED << cog << ATTACKED << "\t\t";
        } else {
            cout << NOTATTACKED << cog << NOTATTACKED << "\t\t";
        }
    }
    cout << endl;
}

void Battle::main(bool line_input) {
    string strat;
    position_definition["right"] = c.getSize() - 1;
    while (c.getSize() != 0) {
        cout << endl << "\t";
        for (size_t i = 0; i < c.getSize(); ++i) {
            cout << " " << c.getCog(i) << "\t\t";
        }
        cout << endl << endl;
        do {
            try {
                Strategy strategy;
                if (line_input) { // one liner
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
