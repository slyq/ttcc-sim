#include "battle.h"
#include "colors.h"
#include "game_config.h"
#include "rang.h"
#include <fstream>
#include <string>

using namespace std;

int main() {
    cout << rang::fgB::gray << "Toontown Corporate Clash Battle Simulator by slyq" << rang::style::reset << endl;
    string choice;
    bool presState;
    bool hitState;
    bool noDecayState;
    bool lineInput;
    string gagFile;
    bool writeToFile = false;
    try {
        GameConfig conf = GameConfig::read("conf.txt");
        presState = conf.autoPres;
        hitState = conf.autoHit;
        noDecayState = conf.roundUpdate;
        lineInput = conf.lineInput;
        gagFile = conf.gagFilePath;
    } catch (const runtime_error& e) {
        do {
            cout << CONFIG << "No config file \"conf.txt\" found. Create one? [y/n] " << rang::style::reset;
            getline(cin, choice);
        } while (choice != "y" && choice != "Y" && choice != "n" && choice != "N");
        if (choice == "y" || choice == "Y") {
            writeToFile = true;
        }
        // determine battle settings
        cout << CONFIG << "Assume prestige gags? [Y/n] " << rang::style::reset;
        getline(cin, choice);
        presState = (choice != "n" && choice != "N");
        cout << CONFIG << "Assume perfect accuracy? [Y/n] " << rang::style::reset;
        getline(cin, choice);
        hitState = (choice != "n" && choice != "N");
        cout << CONFIG << "Disregard lure and soak decay? [Y/n] " << rang::style::reset;
        getline(cin, choice);
        noDecayState = (choice != "n" && choice != "N");
        // determine input method
        cout << CONFIG << "Input using one-liners? (will use individual commands otherwise) [Y/n] " << rang::style::reset;
        getline(cin, choice);
        lineInput = (choice != "n" && choice != "N");
        gagFile = "gags.txt";
        cout << endl;
    }
    // confirmation/write
    cout << rang::fgB::yellow << "Current battle config:" << endl << "---------------------------------" << endl;
    cout << (presState ? "A" : "Not a") << "ssuming prestige gags." << endl;
    cout << (hitState ? "A" : "Not a") << "ssuming perfect accuracy." << endl;
    cout << (noDecayState ? "Disregard" : "Consider") << "ing lure and soak decay." << endl;
    cout << "Using " << (lineInput ? "one-line" : "multiple") << " commands." << endl;
    if (gagFile != "gags.txt") {
        cout << "Using \"" << gagFile << "\" as the gag definitions file." << endl;
    }
    cout << rang::style::reset << endl;
    if (writeToFile) {
        ofstream configFile("conf.txt");
        configFile << "all_prestige: " << (presState ? 1 : 0) << '\n';
        configFile << "perfect_acc: " << (hitState ? 1 : 0) << '\n';
        configFile << "no_decay: " << (noDecayState ? 1 : 0) << "\n";
        configFile << "line_input: " << (lineInput ? 1 : 0) << '\n';
        configFile << "gag_file: " << gagFile << '\n';
    }
    // setup
    Battle b(gagFile);
    b.setPresState(presState);
    b.setHitState(hitState);
    b.setUpdate(!noDecayState);
    b.setInputState(lineInput);
    // determine cog set
    cout << CONFIG << "Use a randomly generated cog set? [Y/n] " << rang::style::reset;
    getline(cin, choice);
    bool generate = (choice != "n" && choice != "N");
    string again;
    do {
        if (again == "y" || again == "Y") {
            b.reset();
        }
        // build cogs
        if (generate) {
            b.generate();
        } else {
            cout << CONFIG << "Specify the custom cog set: " << rang::style::reset;
            vector<Cog> loader;
            string lvl;
            while (cin.peek() != '\n' && cin >> lvl) {
                loader.push_back(Cog(lvl));
            }
            cin.ignore();
            b.setCogset(loader);
        }

        b.battle();
        cout << CONFIG << "Play again? [y/N] " << rang::style::reset;
        getline(cin, again);
    } while (again == "y" || again == "Y");
    return 0;
}
