#include <fstream>
#include <gag_collection.h>
#include <iostream>

GagCollection GagCollection::read(const std::string& file_path) {
    std::ifstream file(file_path);
    GagCollection gc;
    if (file) {
        std::string name;
        unsigned short damage;
        unsigned short accuracy;
        int effect;

        for (size_t i = 0; i < 8; ++i) {
            for (size_t j = 0; j < 8; ++j) {
                if (i == 2 || i == 4) {
                    file >> name >> damage >> accuracy >> effect;
                    gc.gagmap[name] = Gag((GagKind)i, name, j + 1, damage, accuracy, effect, false);
                } else {
                    file >> name >> damage >> accuracy;
                    gc.gagmap[name] = Gag((GagKind)i, name, j + 1, damage, accuracy, false);
                }
            }
        }
        // 3 active sos per track
        for (size_t i = 0; i < 8; ++i) {
            for (size_t j = 0; j < 3; ++j) {
                if (i == 2 || i == 4) {
                    file >> name >> damage >> effect;
                    gc.gagmap[name] = Gag((GagKind)i, name, j + 1, damage, 100, effect, true);
                } else {
                    file >> name >> damage;
                    gc.gagmap[name] = Gag((GagKind)i, name, j + 1, damage, 100, true);
                }
            }
        }
        // rain
        file >> name >> damage;
        gc.gagmap[name] = Gag(GagKind::DROP, name, 2, damage, 100, true);
    } else {
        if (file_path != "gags.txt") {
            std::cerr << "Could not find custom gag definitions file, using defaults..." << std::endl;
        }
        gc.gagmap["feather"] = Gag(GagKind::TOONUP, "feather", 1, 8, 70, false);
        gc.gagmap["megaphone"] = Gag(GagKind::TOONUP, "megaphone", 2, 15, 70, false);
        gc.gagmap["lipstick"] = Gag(GagKind::TOONUP, "lipstick", 3, 26, 70, false);
        gc.gagmap["bamboo"] = Gag(GagKind::TOONUP, "bamboo", 4, 39, 70, false);
        gc.gagmap["pixie"] = Gag(GagKind::TOONUP, "pixie", 5, 50, 70, false);
        gc.gagmap["juggling"] = Gag(GagKind::TOONUP, "juggling", 6, 78, 70, false);
        gc.gagmap["confetti"] = Gag(GagKind::TOONUP, "confetti", 7, 95, 70, false);
        gc.gagmap["highdive"] = Gag(GagKind::TOONUP, "highdive", 8, 135, 70, false);
        gc.gagmap["banana"] = Gag(GagKind::TRAP, "banana", 1, 20, 100, false);
        gc.gagmap["rake"] = Gag(GagKind::TRAP, "rake", 2, 30, 100, false);
        gc.gagmap["spring"] = Gag(GagKind::TRAP, "spring", 3, 45, 100, false);
        gc.gagmap["marbles"] = Gag(GagKind::TRAP, "marbles", 4, 65, 100, false);
        gc.gagmap["quicksand"] = Gag(GagKind::TRAP, "quicksand", 5, 90, 100, false);
        gc.gagmap["trapdoor"] = Gag(GagKind::TRAP, "trapdoor", 6, 140, 100, false);
        gc.gagmap["wreckingball"] = Gag(GagKind::TRAP, "wreckingball", 7, 200, 100, false);
        gc.gagmap["tnt"] = Gag(GagKind::TRAP, "tnt", 8, 240, 100, false);
        gc.gagmap["1dollar"] = Gag(GagKind::LURE, "1dollar", 1, 0, 50, 2, false);
        gc.gagmap["smallmagnet"] = Gag(GagKind::LURE, "smallmagnet", 2, 0, 50, 2, false);
        gc.gagmap["5dollar"] = Gag(GagKind::LURE, "5dollar", 3, 0, 60, 3, false);
        gc.gagmap["bigmagnet"] = Gag(GagKind::LURE, "bigmagnet", 4, 0, 60, 3, false);
        gc.gagmap["10dollar"] = Gag(GagKind::LURE, "10dollar", 5, 0, 70, 4, false);
        gc.gagmap["hypno"] = Gag(GagKind::LURE, "hypno", 6, 0, 70, 4, false);
        gc.gagmap["50dollar"] = Gag(GagKind::LURE, "50dollar", 7, 0, 80, 5, false);
        gc.gagmap["presentation"] = Gag(GagKind::LURE, "presentation", 8, 0, 80, 5, false);
        gc.gagmap["kazoo"] = Gag(GagKind::SOUND, "kazoo", 1, 4, 95, false);
        gc.gagmap["bikehorn"] = Gag(GagKind::SOUND, "bikehorn", 2, 7, 95, false);
        gc.gagmap["whistle"] = Gag(GagKind::SOUND, "whistle", 3, 11, 95, false);
        gc.gagmap["bugle"] = Gag(GagKind::SOUND, "bugle", 4, 16, 95, false);
        gc.gagmap["aoogah"] = Gag(GagKind::SOUND, "aoogah", 5, 21, 95, false);
        gc.gagmap["trunk"] = Gag(GagKind::SOUND, "trunk", 6, 32, 95, false);
        gc.gagmap["fog"] = Gag(GagKind::SOUND, "fog", 7, 50, 95, false);
        gc.gagmap["opera"] = Gag(GagKind::SOUND, "opera", 8, 65, 95, false);
        gc.gagmap["flower"] = Gag(GagKind::SQUIRT, "flower", 1, 4, 95, 1, false);
        gc.gagmap["glass"] = Gag(GagKind::SQUIRT, "glass", 2, 8, 95, 1, false);
        gc.gagmap["gun"] = Gag(GagKind::SQUIRT, "gun", 3, 12, 95, 2, false);
        gc.gagmap["wballoon"] = Gag(GagKind::SQUIRT, "wballoon", 4, 21, 95, 2, false);
        gc.gagmap["seltzer"] = Gag(GagKind::SQUIRT, "seltzer", 5, 30, 95, 3, false);
        gc.gagmap["hose"] = Gag(GagKind::SQUIRT, "hose", 6, 56, 95, 3, false);
        gc.gagmap["cloud"] = Gag(GagKind::SQUIRT, "cloud", 7, 80, 95, 4, false);
        gc.gagmap["geyser"] = Gag(GagKind::SQUIRT, "geyser", 8, 115, 95, 4, false);
        gc.gagmap["buzzer"] = Gag(GagKind::ZAP, "buzzer", 1, 4, 30, false);
        gc.gagmap["rug"] = Gag(GagKind::ZAP, "rug", 2, 6, 30, false);
        gc.gagmap["balloon"] = Gag(GagKind::ZAP, "balloon", 3, 10, 30, false);
        gc.gagmap["kart"] = Gag(GagKind::ZAP, "kart", 4, 16, 30, false);
        gc.gagmap["taser"] = Gag(GagKind::ZAP, "taser", 5, 24, 30, false);
        gc.gagmap["tv"] = Gag(GagKind::ZAP, "tv", 6, 40, 30, false);
        gc.gagmap["tesla"] = Gag(GagKind::ZAP, "tesla", 7, 66, 30, false);
        gc.gagmap["lightning"] = Gag(GagKind::ZAP, "lightning", 8, 80, 30, false);
        gc.gagmap["cupcake"] = Gag(GagKind::THROW, "cupcake", 1, 7, 75, false);
        gc.gagmap["fruitslice"] = Gag(GagKind::THROW, "fruitslice", 2, 11, 75, false);
        gc.gagmap["creamslice"] = Gag(GagKind::THROW, "creamslice", 3, 18, 75, false);
        gc.gagmap["bdayslice"] = Gag(GagKind::THROW, "bdayslice", 4, 30, 75, false);
        gc.gagmap["fruit"] = Gag(GagKind::THROW, "fruit", 5, 45, 75, false);
        gc.gagmap["cream"] = Gag(GagKind::THROW, "cream", 6, 75, 75, false);
        gc.gagmap["cake"] = Gag(GagKind::THROW, "cake", 7, 110, 75, false);
        gc.gagmap["wedding"] = Gag(GagKind::THROW, "wedding", 8, 145, 75, false);
        gc.gagmap["pot"] = Gag(GagKind::DROP, "pot", 1, 12, 50, false);
        gc.gagmap["sandbag"] = Gag(GagKind::DROP, "sandbag", 2, 20, 50, false);
        gc.gagmap["bowling"] = Gag(GagKind::DROP, "bowling", 3, 35, 50, false);
        gc.gagmap["anvil"] = Gag(GagKind::DROP, "anvil", 4, 55, 50, false);
        gc.gagmap["weight"] = Gag(GagKind::DROP, "weight", 5, 80, 50, false);
        gc.gagmap["safe"] = Gag(GagKind::DROP, "safe", 6, 125, 50, false);
        gc.gagmap["boulder"] = Gag(GagKind::DROP, "boulder", 7, 180, 50, false);
        gc.gagmap["piano"] = Gag(GagKind::DROP, "piano", 8, 220, 50, false);
        gc.gagmap["chuckle"] = Gag(GagKind::TOONUP, "chuckle", 6, 78, 100, true);
        gc.gagmap["don"] = Gag(GagKind::TOONUP, "don", 7, 95, 100, true);
        gc.gagmap["flippy"] = Gag(GagKind::TOONUP, "flippy", 8, 150, 100, true);
        gc.gagmap["will"] = Gag(GagKind::TRAP, "will", 6, 140, 100, true);
        gc.gagmap["penny"] = Gag(GagKind::TRAP, "penny", 7, 200, 100, true);
        gc.gagmap["clara"] = Gag(GagKind::TRAP, "clara", 8, 240, 100, true);
        gc.gagmap["stinkyned"] = Gag(GagKind::LURE, "stinkyned", 6, 0, 100, 3, true);
        gc.gagmap["nancy"] = Gag(GagKind::LURE, "nancy", 7, 0, 100, 4, true);
        gc.gagmap["oldman"] = Gag(GagKind::LURE, "oldman", 8, 0, 100, 5, true);
        gc.gagmap["barbara"] = Gag(GagKind::SOUND, "barbara", 6, 40, 100, true);
        gc.gagmap["sonata"] = Gag(GagKind::SOUND, "sonata", 7, 60, 100, true);
        gc.gagmap["moe"] = Gag(GagKind::SOUND, "moe", 8, 85, 100, true);
        gc.gagmap["sid"] = Gag(GagKind::SQUIRT, "sid", 6, 56, 100, 3, true);
        gc.gagmap["sanjay"] = Gag(GagKind::SQUIRT, "sanjay", 7, 80, 100, 4, true);
        gc.gagmap["sharky"] = Gag(GagKind::SQUIRT, "sharky", 8, 115, 100, 4, true);
        gc.gagmap["dentist"] = Gag(GagKind::ZAP, "dentist", 6, 45, 100, true);
        gc.gagmap["electra"] = Gag(GagKind::ZAP, "electra", 7, 70, 100, true);
        gc.gagmap["nat"] = Gag(GagKind::ZAP, "nat", 8, 85, 100, true);
        gc.gagmap["cleff"] = Gag(GagKind::THROW, "cleff", 6, 75, 100, true);
        gc.gagmap["cindy"] = Gag(GagKind::THROW, "cindy", 7, 110, 100, true);
        gc.gagmap["pierce"] = Gag(GagKind::THROW, "pierce", 8, 145, 100, true);
        gc.gagmap["ned"] = Gag(GagKind::DROP, "ned", 6, 125, 100, true);
        gc.gagmap["franz"] = Gag(GagKind::DROP, "franz", 7, 180, 100, true);
        gc.gagmap["bessie"] = Gag(GagKind::DROP, "bessie", 8, 220, 100, true);
        gc.gagmap["rain"] = Gag(GagKind::DROP, "rain", 2, 20, 100, true);
    }
    // fire
    gc.gagmap["fire"] = Gag(GagKind::FIRE, "fire", 0, 0, 100, false);
    return gc;
}
