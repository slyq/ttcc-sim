#include <gag_collection.h>
#include <fstream>
#include <iostream>

using namespace std;

GagCollection::GagCollection() {
    ifstream file(file_path);
    if (!file) {
        throw runtime_error("cannot open gag definitions");
    }

    string name;
    unsigned short damage;

    for (size_t i = 0; i < 8; ++i) {
        vector<Gag> gagset;
        for (size_t j = 0; j < 8; ++j) {
            file >> name >> damage;
            switch (i) {
            case 0:
                gagset.push_back(Gag(GagKind::TOONUP, name, damage));
                gagmap[name] = Gag(GagKind::TOONUP, name, damage);
                break;
            case 1:
                gagset.push_back(Gag(GagKind::TRAP, name, damage));
                gagmap[name] = Gag(GagKind::TRAP, name, damage);
                break;
            case 2:
                gagset.push_back(Gag(GagKind::LURE, name, damage));
                gagmap[name] = Gag(GagKind::LURE, name, damage);
                break;
            case 3:
                gagset.push_back(Gag(GagKind::SOUND, name, damage));
                gagmap[name] = Gag(GagKind::SOUND, name, damage);
                break;
            case 4:
                gagset.push_back(Gag(GagKind::SQUIRT, name, damage));
                gagmap[name] = Gag(GagKind::SQUIRT, name, damage);
                break;
            case 5:
                gagset.push_back(Gag(GagKind::ZAP, name, damage));
                gagmap[name] = Gag(GagKind::ZAP, name, damage);
                break;
            case 6:
                gagset.push_back(Gag(GagKind::THROW, name, damage));
                gagmap[name] = Gag(GagKind::THROW, name, damage);
                break;
            case 7:
                gagset.push_back(Gag(GagKind::DROP, name, damage));
                gagmap[name] = Gag(GagKind::DROP, name, damage);
                break;
            default:
                break;
            }
        }
        gagsets.push_back(gagset);
    }
}

GagCollection& GagCollection::operator=(const GagCollection& other) {
    gagsets = other.gagsets;
    gagmap = other.gagmap;
    return *this;
}
