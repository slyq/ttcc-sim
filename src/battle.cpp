#include "battle.h"

#include "colors.h"
#include "rang.h"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <deque>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <time.h>

using namespace std;

Battle::Battle(string file_path) : gc(GagCollection::read(file_path)) {}

Battle::Battle(string file_path, const queue<int>& set) : cogset(Cogset(set)), gc(GagCollection::read(file_path)) {}

Battle::Battle(string file_path, const vector<Cog>& set) : cogset(Cogset(set)), gc(GagCollection::read(file_path)) {}

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

void Battle::turn(vector<Gag> strat) {
    vector<vector<Gag>> gags;
    size_t num_gagtracks = 8;
    for (size_t i = 0; i < num_gagtracks + 1; ++i) {
        vector<Gag> gagtrack;
        gags.push_back(gagtrack);
    }
    for (size_t i = 0; i < strat.size(); ++i) {
        if (autoPres) {
            strat[i].prestiged = true;
        }
        if (autoHit) {
            strat[i].accuracy = 100;
        }
        if (strat[i].kind != GagKind::PASS) {
            gags[(int)strat[i].kind].push_back(strat[i]);
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

int Battle::validQuickhand(string quickhand) const {
    int counter = 0;
    if (quickhand.size() == cogset.getSize()) {
        for (size_t i = 0; i < cogset.getSize(); ++i) {
            if (quickhand[i] == 'x') {
                ++counter;
            } else if (quickhand[i] == 'X') {
                counter += 2;
            } else if (quickhand[i] != '-') {
                return 0;
            }
        }
    }
    return counter;
}

bool Battle::isPluralGag(string gag) const {
    if (gag[gag.size() - 1] == 's') {
        gag.pop_back();
        return gc.contains(gag);
    }
    return false;
}

/*
how straight/cross usage works:
normally, gags are assigned to the target immediately following it or via a target queue (director) which is fed by the
quickhand strat
for gags that are relevant to the cross, the director temporarily functions as a stack until the cross is no longer
relevant or the command is fully parsed, at which point cross gags in the block are assigned targets until the director is emptied
the director will never serve as both a queue and stack at the same time
*/
vector<Gag> Battle::parseOneliner(string strat) {
    posDefinition["right"] = cogset.getSize() - 1;
    vector<Gag> gags;
    deque<int> director;
    stringstream ss(strat);
    string buffer;
    bool cross = false, blockpres = false, gagpres = false;
    int quickhand = 0, multiplier = 1;
    size_t crossApplyIndex = 0;
    while (ss >> buffer) {  // each loop parses one command block
        if (buffer == "pres") {
            ss >> buffer;
            if (validQuickhand(buffer) || buffer == "cross") {
                blockpres = true;
            } else if (gc.contains(buffer) || isPluralGag(buffer)) {
                gagpres = true;
            } else {
                throw invalid_argument("Expected strategy specification or gag after \"pres\"");
            }
        }
        if (buffer == "cross") {
            cross = true;
            ss >> buffer;
            if (!validQuickhand(buffer) && !(buffer.size() == 1 && isdigit(buffer[0])) && buffer != "pres"
                && !(gc.contains(buffer) || isPluralGag(buffer))) {
                throw invalid_argument("Expected quickhand strat or gag block after \"cross\"");
            }
        }
        if (validQuickhand(buffer)) {
            quickhand = validQuickhand(buffer);
            string temp = buffer;
            ss >> buffer;
            if (buffer == "cross") {
                cross = true;
                ss >> buffer;
            }
            // insert directors
            for (size_t i = 0; i < cogset.getSize(); ++i) {
                if (temp[i] == 'x') {
                    if (cross) {
                        director.push_front(i);
                    } else {
                        director.push_back(i);
                    }
                } else if (temp[i] == 'X') {
                    if (cross) {
                        director.push_front(i);
                        director.push_front(i);
                    } else {
                        director.push_back(i);
                        director.push_back(i);
                    }
                }
            }
            cross = false;
        }
        // must be #, pres, or gag
        if (!(buffer.size() == 1 && isdigit(buffer[0])) && buffer != "pres"
            && !(gc.contains(buffer) || isPluralGag(buffer))) {
            throw invalid_argument("Invalid gag block/command \"" + buffer + "\"");
        }
        crossApplyIndex = gags.size();
        int spread = 0;
        Gag remember;
        do {  // each loop parses a gag block
            if (buffer.size() == 1 && isdigit(buffer[0])) {
                if (quickhand) {
                    // spread by specific amount
                    spread = stoi(buffer);
                } else {
                    // apply same gag on same target multiple times
                    multiplier = stoi(buffer);
                }
                ss >> buffer;
                if (buffer != "pres" && !(gc.contains(buffer) || isPluralGag(buffer))) {
                    throw invalid_argument("Expected gag after multiplier");
                }
            }
            if (buffer == "pres") {
                gagpres = true;
                ss >> buffer;
            }
            if (gc.contains(buffer) || isPluralGag(buffer)) {
                Gag gagchoice;
                if (isPluralGag(buffer)) {
                    if (!spread && quickhand) {
                        // plural without a multiplier - spread across entire quickhand
                        spread = -1;
                    }
                    buffer.pop_back();
                }
                if (spread) {
                    // repeat last gag
                    if (remember.kind == GagKind::PASS) {
                        // first time through, store
                        gagchoice = gc.get(buffer);
                        remember = gagchoice;
                    } else {
                        // retrieve
                        gagchoice = remember;
                    }
                    if (--spread == 0) {
                        // spread is over, reset remembered gag
                        remember = Gag();
                    }
                } else {
                    // get gag
                    gagchoice = gc.get(buffer);

                }
                if (cross && (remember.kind != gagchoice.kind || remember.name != gagchoice.name)) {
                    // non-quickhand crossed gag block has ended
                    cross = false;
                }
                gagchoice.prestiged = gagpres || blockpres;
                if (quickhand > 0 && director.size()) {
                    if ((gagchoice.kind == GagKind::LURE && gagchoice.name.find("dollar") == string::npos) || gagchoice.kind == GagKind::SOUND || gc.isSOS(buffer)) {
                        throw invalid_argument("Cannot use multi-target gags/SOS in a quickhand strategy");
                    }
                    // retrieve target from the "queued" targets parsed from the quickhand
                    gagchoice.target = director.front();
                    director.pop_front();
                } else if ((gagchoice.kind == GagKind::LURE && gagchoice.name.find("dollar") == string::npos) || gagchoice.kind == GagKind::SOUND || gc.isSOS(buffer)) {
                    gagchoice.target = -1;
                } else if (cogset.getSize() == 1) {
                    gagchoice.target = 0;
                } else {
                    // parse target
                    int target = -2;
                    ss >> buffer;
                    if (buffer[0] == '!') {
                        try {
                            target = stoi(buffer.substr(buffer.find("!") + 1));
                            if (target < 0 || target >= (int)cogset.getSize()) {
                                throw invalid_argument("Out of bounds");
                            }
                        } catch (const invalid_argument& e) {
                            throw invalid_argument("Invalid index for \"" + gagchoice.name + "\"");
                        }
                    } else if (posDefinition.find(buffer) != posDefinition.end()) {  // word position check
                        if (posDefinition[buffer] >= (int)cogset.getSize()) {
                            throw invalid_argument("Invalid position for \"" + gagchoice.name + "\"");
                        }
                        target = posDefinition[buffer];
                    } else {  // cog level position check
                        bool directed = false;
                        for (size_t i = 0; i < cogset.getSize(); i++) {
                            if (cogset.getCog(i).getLevelName() == buffer) {
                                for (int j = 0; j < multiplier; ++j) {
                                    target = i;
                                }
                                directed = true;
                                break;
                            }
                        }
                        if (!directed) {
                            throw invalid_argument("Position not supplied for \"" + gagchoice.name + "\"");
                        }
                    }
                    if (cross) {
                        director.push_front(target);
                    } else {
                        gagchoice.target = target;
                    }
                }
                for (int i = 0; i < multiplier; ++i) {
                    gags.push_back(gagchoice);
                }
                if (quickhand) {
                    quickhand -= multiplier;
                }
            } else if (quickhand) {
                throw invalid_argument("Invalid gag \"" + buffer + "\"");
            } else {
                // likely the start of a new command block
                cross = false;
                spread = 0;
                remember = Gag();
            }
            if (!spread) {
                // reset gag prestige state if it is not being spread across
                gagpres = false;
            }
            multiplier = 1;
        } while ((quickhand > 0 || cross) && (spread || ss >> buffer));
        // resolve cross targets
        while (!director.empty()) {
            gags[crossApplyIndex].target = director.front();
            director.pop_front();
            ++crossApplyIndex;
        }
        cross = false;
        blockpres = false;
    }
    // check if all spots have been covered
    if (quickhand) {
        throw invalid_argument("Not enough gags are supplied as required by the quickhand strategy");
    }
    // after while finishes (reached end), if any directors in stack, apply
    // resolve cross targets
    while (!director.empty()) {
        gags[crossApplyIndex].target = director.front();
        director.pop_front();
        ++crossApplyIndex;
    }
    // check invalid gag usages
    for (const Gag& gag : gags) {
        cogset.gagCheck(gag);
    }
    sort(gags.begin(), gags.end(), GagComparator());
    return gags;
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
    if (!gc.contains(gag_name) && gag_name != "PASS") {
        throw invalid_argument("Unrecognized gag \"" + gag_name + "\"");
    }
    if (gag_name != "PASS") {
        gagchoice = gc.get(gag_name);
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
                        throw invalid_argument("Out of bounds");
                } catch (const invalid_argument& e) {
                    throw invalid_argument("Invalid index");
                }
            } else if (posDefinition.find(target) != posDefinition.end()) {
                if (posDefinition[target] >= (int)cogset.getSize()) {
                    throw invalid_argument("Invalid position for \"" + gag_name + "\"");
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
                    throw invalid_argument("Position not supplied for \"" + gag_name + "\"");
                }
            }
        }
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
                    turn(gags);
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
                    } else if (strat == "ALLPASS") {
                        gags.clear();
                        break;
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
                    sort(gags.begin(), gags.end(), GagComparator());
                    turn(gags);
                    break;
                }
            }
        } while (strat != "END");
    }
    if (cogset.getSize() == 0) {
        cout << "You did it!" << endl;
    }
}
