#include "battle.h"
#include "colors.h"
#include "rang.h"
#include "game_config.h"
#include <string>

using namespace std;

int main() {
    string again;
    string choice;
    bool presState;
    bool hitState;
    bool updateState;
    bool lineInput;
    string gagFile;
    try {
        GameConfig conf = GameConfig::read("conf.txt");
        presState = conf.autoPres;
        hitState = conf.autoHit;
        updateState = conf.roundUpdate;
        lineInput = conf.lineInput;
        gagFile = conf.gagFilePath;
    } catch (const runtime_error& e) {
        // determine battle settings
        cout << CONFIG << "Assume prestige gags? [Y/n] " << rang::style::reset;
        getline(cin, choice);
        presState = (choice != "n" && choice != "N");
        cout << CONFIG << "Assume no miss? [Y/n] " << rang::style::reset;
        getline(cin, choice);
        hitState = (choice != "n" && choice != "N");
        cout << CONFIG << "Consider lure and soak decay? [y/N] " << rang::style::reset;
        getline(cin, choice);
        updateState = (choice != "y" && choice != "Y");
        // determine input method
        cout << CONFIG << "Input using one-liners? (will use individual commands otherwise) [Y/n] " << rang::style::reset;
        getline(cin, choice);
        lineInput = (choice != "n" && choice != "N");
        cout << CONFIG << "Specify the file with gag definitions [default: gags.txt] " << rang::style::reset;
        getline(cin, choice);
        if (choice == "") {
            gagFile = "gags.txt";
        } else {
            gagFile = choice;
        }
    }
    // determine cog set
    cout << CONFIG << "Use a randomly generated cog set? [Y/n] " << rang::style::reset;
    getline(cin, choice);
    bool generate = (choice != "n" && choice != "N");
    do {
        // setup
        Battle b(gagFile);
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
            b = Battle(gagFile, loader);
        }
        b.setPresState(presState);
        b.setHitState(hitState);
        b.setUpdate(updateState);
        b.setInputState(lineInput);
        // run game
        b.battle();
        // ask for loop
        cout << CONFIG << "Play again? [y/N] " << rang::style::reset;
        getline(cin, again);
    } while (again == "y" || again == "Y");
    return 0;
}
