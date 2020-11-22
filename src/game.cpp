#include "battle.h"
#include "rang.h"
#include "colors.h"
#include <stack>
#include <string>

using namespace std;

int main() {
    string again;
    string choice;
    // determine battle settings
    cout << CONFIG << "Assume prestiged gags? [Y/n] " << rang::style::reset;
    getline(cin, choice);
    bool auto_pres = (choice != "n" && choice != "N");
    // determine input method
    cout << CONFIG << "Input using one-liners? (will use individual commands otherwise) [Y/n] " << rang::style::reset;
    getline(cin, choice);
    bool oneline = (choice != "n" && choice != "N");
    // determine cog set
    cout << CONFIG << "Use a randomly generated cog set? [Y/n] " << rang::style::reset;
    getline(cin, choice);
    bool generate = (choice != "n" && choice != "N");
    do {
        // setup
        Battle b;
        if (!generate) {
            cout << CONFIG << "Specify the custom cog set: " << rang::style::reset;
            vector<Cog> loader;
            string lvl;
            while (cin.peek() != '\n' && cin >> lvl) {
                loader.push_back(Cog(stoi(lvl.substr(0, lvl.find("."))), lvl.find(".exe") != string::npos));
            }
            cin.ignore();
            b = Battle(loader);
        }
        b.setPresState(auto_pres);
        // run game
        b.main(oneline);
        // ask for loop
        cout << CONFIG << "Play again? [y/N] " << rang::style::reset;
        getline(cin, again);
    } while (again == "y" || again == "Y");
    return 0;
}
