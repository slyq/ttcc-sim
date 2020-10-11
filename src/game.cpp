#include "battle.h"
#include "rang.h"
#include "colors.h"
#include <stack>
#include <string>

using namespace std;

int main() {
    string again = "Y";
    while (again == "Y") {
        cout << CONFIG << "Input mode? (0 for tested generation, 1 for custom): " << rang::style::reset;
        size_t i_config = 0;
        Battle b;
        if (cin >> i_config && i_config == 1) {
            vector<Cog> loader;
            string lvl;
            for (int i = 0; i < 4; ++i) {
                cin >> lvl;
                loader.push_back(Cog(stoi(lvl), lvl.find(".exe") != string::npos));
            }
            b = Battle(loader);
        } else {
            b = Battle();
        }
        cout << CONFIG << "Strategy mode? (0 for one liners, 1 for individual commands): " << rang::style::reset;
        size_t s_config;
        if (cin >> s_config) {
            cin.ignore();
            b.main(s_config);
            cout << CONFIG << "Try again? Y / (n): " << rang::style::reset;
            cin >> again;
        } else {
            break;
        }
    }
    return 0;
}
