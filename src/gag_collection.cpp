#include <gag_collection.h>
#include <fstream>
#include <iostream>

GagCollection GagCollection::read(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file) {
        throw std::runtime_error("cannot open gag definitions");
    }

    GagCollection gc;
    std::string name;
    unsigned short damage;
    float accuracy;

    for (size_t i = 0; i < 8; ++i) {
        for (size_t j = 0; j < 8; ++j) {
            file >> name >> damage >> accuracy;
            bool sos = j>7;
            switch (i) {
                case 0:
                    gc.gagmap[name] = Gag(GagKind::TOONUP, name, damage, accuracy);
                    break;
                case 1:
                    gc.gagmap[name] = Gag(GagKind::TRAP, name, damage, accuracy);
                    break;
                case 2:
                    gc.gagmap[name] = Gag(GagKind::LURE, name, damage, accuracy);
                    break;
                case 3:
                    gc.gagmap[name] = Gag(GagKind::SOUND, name, damage, accuracy);
                    break;
                case 4:
                    gc.gagmap[name] = Gag(GagKind::SQUIRT, name, damage, accuracy);
                    break;
                case 5:
                    gc.gagmap[name] = Gag(GagKind::ZAP, name, damage, accuracy);
                    break;
                case 6:
                    gc.gagmap[name] = Gag(GagKind::THROW, name, damage, accuracy);
                    break;
                case 7:
                    gc.gagmap[name] = Gag(GagKind::DROP, name, damage, accuracy);
                    break;
                default:
                    break;
            }
        }
    }
    return gc;
}
