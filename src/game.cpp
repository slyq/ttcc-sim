#include "battle.h"
#include "colors.h"
#include "rang.h"
#include <string>

using namespace std;

int main() {
    string again;
    string choice;
    // determine battle settings
    cout << CONFIG << "Assume prestige gags? [Y/n] " << rang::style::reset;
    getline(cin, choice);
    bool presState = (choice != "n" && choice != "N");
    cout << CONFIG << "Assume no miss? [Y/n] " << rang::style::reset;
    getline(cin, choice);
    bool hitState = (choice != "n" && choice != "N");
    // determine input method
    cout << CONFIG << "Input using one-liners? (will use individual commands otherwise) [Y/n] " << rang::style::reset;
    getline(cin, choice);
    bool lineInput = (choice != "n" && choice != "N");
    // determine cog set
    cout << CONFIG << "Use a randomly generated cog set? [Y/n] " << rang::style::reset;
    getline(cin, choice);
    bool generate = (choice != "n" && choice != "N");
    do {
        // setup
        Battle b;
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
            b = Battle(loader);
        }
        b.setPresState(presState);
        b.setHitState(hitState);
        b.setInputState(lineInput);
        // run game
        b.battle();
        // ask for loop
        cout << CONFIG << "Play again? [y/N] " << rang::style::reset;
        getline(cin, again);
    } while (again == "y" || again == "Y");
    return 0;
}
