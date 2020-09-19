#include "cog.h"
#include "cogset.h"
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <string.h>
#include <memory>
#include <queue>

using namespace std;

int main() {
    queue<int> battlequeue;
    // TODO: make queue generator
    for (int i = 6; i < 12; i++) {
        battlequeue.push(i);
    }
    Cogset battles = Cogset(battlequeue);
    while (battles.getSize() != 0) {
        for (int i = 0; i < 4; i++) {
            cout << battles.getCog(i) << "\t\t";
        }
        cout << endl;
        // TODO: add functionality for gags, up to 4 per turn
        string gagtype;
        cin >> gagtype;
        if (gagtype == "sound") {
            int s;
            cin >> s;
            for (int i = 0; i < 4; i++) {
                battles.getCog(i).hit(s);
            }
        } else {
            int s1;
            int s1t;
            int s2;
            int s2t;
            cin >> s1;
            cin >> s1t;
            cin >> s2;
            cin >> s2t;
            battles.getCog(s1t).hit(s1);
            battles.getCog(s2t).hit(s2);
        }
        battles.load();
    }
    return 0;
}
