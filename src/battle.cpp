#include "battle.h"

#include "colors.h"
#include "rang.h"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <time.h>

using namespace std;

Battle::Battle(string file_path) : gc(GagCollection::read(file_path)) {}

Battle::Battle(string file_path, queue<int> set) : cogset(Cogset(set)), gc(GagCollection::read(file_path)) {}

Battle::Battle(string file_path, vector<Cog> set) : cogset(Cogset(set)), gc(GagCollection::read(file_path)) {}

void Battle::generate() {
    srand(time(NULL));
    queue<int> set;
    size_t min_level = 7;
    size_t max_level = 15;
    if (rand() % 2 == 0) {
        set.push(rand() % (min_level / 2) + min_level);
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
    cogset = Cogset(set);
}

void Battle::turn(Battle::Strategy strat) {
    vector<vector<Gag>> gags;
    size_t num_gagtracks = 8;
    for (size_t i = 0; i < num_gagtracks + 1; ++i) {
        vector<Gag> gagtrack;
        gags.push_back(gagtrack);
    }
    for (size_t i = 0; i < strat.gags.size(); ++i) {
        if (autoPres) {
            strat.gags[i].prestiged = true;
        }
        if (autoHit) {
            strat.gags[i].accuracy = 100;
        }
        if (strat.gags[i].kind != GagKind::PASS) {
            gags[(int)strat.gags[i].kind].push_back(strat.gags[i]);
        }
    }
    // sort accordingly
    for (size_t i = 0; i < gags.size(); ++i) {
        vector<Gag>& gagtrack = gags[i];
        if (!gagtrack.empty()) {
            if (strat.config == 2) {
                sort(gagtrack.begin(), gagtrack.end(), OrderedGagComparator());
            } else if (i == 5) {
                if (strat.config == 0) {
                    sort(gagtrack.begin(), gagtrack.end(), GagComparator());
                } else if (strat.config == 1) {
                    sort(gagtrack.begin(), gagtrack.end(), CrossGagComparator());
                }
            } else {
                sort(gagtrack.begin(), gagtrack.end(), GagComparator());
            }
        }
    }
    // handle fires first
    if (!gags[num_gagtracks].empty()) {
        cogset.fireTurn(gags[num_gagtracks]);
    }
    for (size_t i = 0; (i < num_gagtracks && !cogset.allDead()); ++i) {
        if (!gags[i].empty()) {
            switch (i) {
            case 1:
                cogset.trapTurn(gags[i]);
                break;
            case 2:
                cogset.lureTurn(gags[i]);
                break;
            case 3:
                cogset.soundTurn(gags[i]);
                break;
            case 4:
                cogset.squirtTurn(gags[i]);
                break;
            case 5:
                cogset.zapTurn(gags[i]);
                break;
            case 6:
                cogset.throwTurn(gags[i]);
                break;
            case 7:
                cogset.dropTurn(gags[i]);
                break;
            default:
                break;
            }
        }
    }
    cogset.load();
    cogset.update();
    posDefinition["right"] = cogset.getSize() - 1;
}

Battle::Strategy Battle::parseOneliner(string strat) {
    posDefinition["right"] = cogset.getSize() - 1;
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
        if (!gc.contains(parser) && parser != "FIRE") {  // strategy first (mult gags or quickhand)
            if (parser.size() == 1 && isdigit(parser[0])) {  // multiple gag
                multiplier = stoi(parser);
                ss >> parser;
                // check if followed with gag or pres
                if (!gc.contains(parser) && parser != "pres") {
                    throw invalid_argument("Please supply the respective gag following your strategy");
                }
            }
            if (parser == "pres") {
                pres = true;
                ss >> parser;
            }
            if (parser.size() == cogset.getSize()) {  // might be a quickhand strategy callout in x and -
                for (size_t i = 0; i < cogset.getSize(); ++i) {
                    if (parser[i] == 'x') {
                        director.push_back(i);
                        ++quickhand;
                    } else if (parser[i] == 'X') {
                        director.push_back(i);
                        director.push_back(i);
                        quickhand += 2;
                    } else if (parser[i] != '-') {
                        if (!gc.contains(parser) && parser != "FIRE") {
                            throw invalid_argument("Unrecognized quickhand strategy " + parser);
                        }
                        break;
                    }
                }
                // check if followed with gag, pres, or cross
                if (quickhand && !(ss >> parser)) {
                    throw invalid_argument("Missing arguments after quickhand strategy");
                }
                if (!gc.contains(parser) && parser != "cross" && parser != "pres" && parser != "FIRE") {
                    throw invalid_argument("Wrong arguments after the quickhand strategy");
                }
            } else if (!gc.contains(parser) && parser != "cross" && parser != "FIRE") {
                throw invalid_argument("Unrecognized gag/command " + parser);
            }
            if (parser == "cross") {
                config = 1;
                if (!(ss >> parser) && quickhand == 0) {
                    break;
                }
            }
        }
        // parsed should now specify a gag
        if (quickhand) {  // quickhand combo
            for (size_t i = 0; i < quickhand; ++i) {
                bool t_pres = false;
                if (parser == "FIRE") {
                    Gag fire;
                    fire.kind = GagKind::FIRE;
                    gags.push_back(fire);
                } else {
                    if (parser == "pres") {
                        t_pres = true;
                        ss >> parser;
                    }
                    if (gc.contains(parser)) {
                        Gag gagchoice = gc.get(parser);
                        gagchoice.prestiged = pres || t_pres;
                        gags.push_back(gagchoice);
                    } else {
                        throw invalid_argument("Unrecognized gag following quickhand strat");
                    }
                }
                if (i != quickhand - 1) {
                    ss >> parser;
                }
            }
            pres = false;
        } else {  // parse gag, then the target following it
            if (parser == "pres") {
                pres = true;
                ss >> parser;
            }
            Gag gagchoice;
            if (parser != "FIRE") {
                gagchoice = gc.get(parser);
            } else {
                gagchoice.kind = GagKind::FIRE;
                gagchoice.damage = 0;
            }
            gagchoice.prestiged = pres;

            if ((gagchoice.kind == GagKind::LURE && gagchoice.name.find("dollar") == string::npos)
                || gagchoice.kind == GagKind::SOUND) {
                for (int i = 0; i < multiplier; ++i) {
                    director.push_back(-1);  // placeholder for multi hit (lure/sound)
                }
            } else if (gc.isSOS(parser)) {
                // sos
                director.push_back(-1);
            } else if (cogset.getSize() == 1) {
                // single target, automatic
                director.push_back(0);
            } else {
                ss >> parser;
                if (parser[0] == '!') {
                    try {
                        int target = stoi(parser.substr(parser.find("!") + 1));
                        if (target < 0 || target >= (int)cogset.getSize()) {
                            throw invalid_argument("out of bounds");
                        }
                        director.push_back(target);
                    } catch (const invalid_argument& e) {
                        throw invalid_argument("Invalid index for " + gagchoice.name);
                    }
                } else if (posDefinition.find(parser) != posDefinition.end()) {  // word position check
                    if (posDefinition[parser] >= (int)cogset.getSize()) {
                        throw invalid_argument("Invalid position for " + gagchoice.name);
                    }
                    for (int i = 0; i < multiplier; ++i) {
                        director.push_back(posDefinition[parser]);
                    }
                } else {  // cog level position check
                    bool directed = false;
                    for (size_t i = 0; i < cogset.getSize(); i++) {
                        if (cogset.getCog(i).getLevelName() == parser) {
                            for (int j = 0; j < multiplier; ++j) {
                                director.push_back(i);
                            }
                            directed = true;
                            break;
                        }
                    }
                    if (!directed) {
                        throw invalid_argument("Position not supplied for " + gagchoice.name);
                    }
                }
            }
            for (int i = 0; i < multiplier; ++i) {
                gags.push_back(gagchoice);
            }
        }
    }
    // check for invalid gag usage & use fires
    for (size_t i = 0; i < gags.size(); ++i) {
        gags[i].target = director[i];
        cogset.gagCheck(gags[i]);
    }
    return Battle::Strategy(gags, config);
}

Gag Battle::parseGag(string command) {
    posDefinition["right"] = cogset.getSize() - 1;
    Gag gagchoice;
    stringstream ss(command);
    string gag_name;
    string target;
    bool pr = false;
    ss >> gag_name;
    if (gag_name == "pres") {
        pr = true;
        ss >> gag_name;
    }
    if (!gc.contains(gag_name) && gag_name != "PASS" && gag_name != "FIRE") {
        throw invalid_argument("Unrecognized gag!");
    }
    if (gag_name != "PASS") {
        if (gag_name != "FIRE") {
            gagchoice = gc.get(gag_name);
        } else {
            gagchoice.kind = GagKind::FIRE;
            gagchoice.damage = 0;
        }
        if ((gagchoice.kind == GagKind::LURE && gagchoice.name.find("dollar") == string::npos)
            || gagchoice.kind == GagKind::SOUND || gc.isSOS(gag_name)) {
            gagchoice.target = -1;
        } else if (cogset.getSize() == 1) {
            // single target, automatic
            gagchoice.target = 0;
        } else {
            // parse target
            ss >> target;
            if (target[0] == '!') {
                try {
                    gagchoice.target = stoi(target.substr(target.find("!") + 1));
                    if (gagchoice.target < 0 || gagchoice.target >= (int)cogset.getSize())
                        throw invalid_argument("out of bounds");
                } catch (const invalid_argument& e) {
                    throw invalid_argument("Invalid index");
                }
            } else if (posDefinition.find(target) != posDefinition.end()) {
                if (posDefinition[target] >= (int)cogset.getSize()) {
                    throw invalid_argument("Invalid position");
                }
                gagchoice.target = posDefinition[target];
            } else {
                bool directed = false;
                for (size_t i = 0; i < cogset.getSize(); i++) {
                    if (cogset.getCog(i).getLevelName() == target) {
                        gagchoice.target = i;
                        directed = true;
                        break;
                    }
                }
                if (!directed) {
                    throw invalid_argument("Unrecognized target");
                }
            }
        }
    } else {
        gagchoice.target = -1;
    }
    gagchoice.prestiged = pr;
    // check for invalid gag usage
    cogset.gagCheck(gagchoice);
    return gagchoice;
}


void Battle::battle() {
    string strat;
    while (cogset.getSize() != 0 && strat != "END") {
        // print cogs
        cout << endl << "\t" << cogset << endl << endl;
        do {
            // get player strategy
            if (lineInput) {  // one liner
                cout << PROMPT << "Your strategy: " << rang::style::reset;
                getline(cin, strat);
                if (strat == "END") {
                    cout << "Force stop" << endl;
                } else if (strat == "SKIP" || strat == "DELETE" || strat == "FIREALL") {
                    vector<Gag> gags;
                    for (size_t i = 0; i < cogset.getSize(); ++i) {
                        gags.push_back(Gag(GagKind::FIRE, 0, i, false));
                    }
                    turn(Battle::Strategy(gags, 0));
                    break;
                } else {
                    try {
                        turn(parseOneliner(strat));
                        break;
                    } catch (const invalid_argument& e) {
                        cerr << e.what() << endl;
                    }
                }
            } else {  // individual toon directing
                vector<Gag> gags;
                size_t numtoons = 4;
                size_t toonIndex = 1;
                while (toonIndex <= numtoons && strat != "END") {
                    cout << PROMPT << "Toon " << toonIndex << ": " << rang::style::reset;
                    getline(cin, strat);
                    if (strat == "END") {
                        cout << "Force stop" << endl;
                    } else if (strat == "UNDO" && toonIndex > 1) {
                        gags.pop_back();
                        --toonIndex;
                    } else if (strat == "SKIP" || strat == "DELETE" || strat == "FIREALL") {
                        for (size_t i = 0; i < cogset.getSize(); ++i) {
                            gags.push_back(Gag(GagKind::FIRE, 0, i, false));
                        }
                        break;
                    } else {
                        try {
                            gags.push_back(parseGag(strat));
                            ++toonIndex;
                        } catch (const invalid_argument& e) {
                            cerr << e.what() << endl;
                        }
                    }
                }
                if (strat != "END") {
                    turn(Battle::Strategy(gags, 2));
                    break;
                }
            }
        } while (strat != "END");
    }
    if (cogset.getSize() == 0) {
        cout << "You did it!" << endl;
    }
}
