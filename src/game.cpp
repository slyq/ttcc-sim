#include "battle.h"
#include "rang.h"
#include "colors.h"
#include <stack>
#include <string>

using namespace std;

int main() {
    string again = "N";
    do {
        string choice = "Y";
        // determine cog set
        cout << CONFIG << "Use a randomly generated cog set? [Y/n] " << rang::style::reset;
        Battle b;
        getline(cin, choice);
        if (choice == "n" || choice == "N") {
            cout << CONFIG << "Specify the custom cog set: " << rang::style::reset;
            vector<Cog> loader;
            string lvl;
            while (cin >> lvl) {
                loader.push_back(Cog(stoi(lvl), lvl.find(".exe") != string::npos));
            }
            b = Battle(loader);
        } else {
            b = Battle();
        }
        // determine battle settings
        cout << CONFIG << "Assume prestiged gags? [Y/n] " << rang::style::reset;
        getline(cin, choice);
        b.setPresState(choice != "n" && choice != "N");
        // determine input method
        cout << CONFIG << "Input using one-liners? (will use individual commands otherwise) [Y/n] " << rang::style::reset;
        getline(cin, choice);
        b.main(choice != "n" && choice != "N");
        cout << CONFIG << "Try again? [y/N] " << rang::style::reset;
        getline(cin, again);
    } while (again == "y" || again == "Y");
    return 0;
}
