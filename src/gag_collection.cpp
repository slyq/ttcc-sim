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
            // bool sos = j > 7;
            gc.gagmap[name] = Gag((GagKind)i, name, damage, accuracy);
        }
    }
    return gc;
}
