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
        int duration;

        for (size_t i = 0; i < 8; ++i) {
            for (size_t j = 0; j < 8; ++j) {
                if (i == 2 || i == 4) {
                    file >> name >> damage >> accuracy >> effect;
                    gc.gags[name] = Gag((GagKind)i, name, j + 1, damage, accuracy, effect);
                } else {
                    file >> name >> damage >> accuracy;
                    gc.gags[name] = Gag((GagKind)i, name, j + 1, damage, accuracy);
                }
            }
        }
        // 3 active sos per track
        for (size_t i = 0; i < 8; ++i) {
            for (size_t j = 0; j < 3; ++j) {
                file >> name >> effect >> duration;
                gc.soses[name] = Sos((SosKind)i, name, effect, duration);
            }
        }
        // rain
        file >> name >> effect >> duration;
        gc.soses[name] = Sos(SosKind::ALL, name, effect, duration);
    } else {
        if (file_path != "gags.txt") {
            std::cerr << "Could not find custom gag definitions file, using defaults..." << std::endl;
        }
        gc.gags["feather"] = Gag(GagKind::TOONUP, "feather", 1, 8, 95);
        gc.gags["megaphone"] = Gag(GagKind::TOONUP, "megaphone", 2, 15, 95);
        gc.gags["lipstick"] = Gag(GagKind::TOONUP, "lipstick", 3, 26, 95);
        gc.gags["bamboo"] = Gag(GagKind::TOONUP, "bamboo", 4, 39, 95);
        gc.gags["pixie"] = Gag(GagKind::TOONUP, "pixie", 5, 50, 95);
        gc.gags["juggling"] = Gag(GagKind::TOONUP, "juggling", 6, 78, 95);
        gc.gags["confetti"] = Gag(GagKind::TOONUP, "confetti", 7, 95, 95);
        gc.gags["highdive"] = Gag(GagKind::TOONUP, "highdive", 8, 135, 95);
        gc.gags["banana"] = Gag(GagKind::TRAP, "banana", 1, 20, 100);
        gc.gags["rake"] = Gag(GagKind::TRAP, "rake", 2, 30, 100);
        gc.gags["spring"] = Gag(GagKind::TRAP, "spring", 3, 45, 100);
        gc.gags["marbles"] = Gag(GagKind::TRAP, "marbles", 4, 65, 100);
        gc.gags["quicksand"] = Gag(GagKind::TRAP, "quicksand", 5, 90, 100);
        gc.gags["trapdoor"] = Gag(GagKind::TRAP, "trapdoor", 6, 140, 100);
        gc.gags["wreckingball"] = Gag(GagKind::TRAP, "wreckingball", 7, 200, 100);
        gc.gags["tnt"] = Gag(GagKind::TRAP, "tnt", 8, 240, 100);
        gc.gags["1dollar"] = Gag(GagKind::LURE, "1dollar", 1, 0, 65, 2);
        gc.gags["smallmagnet"] = Gag(GagKind::LURE, "smallmagnet", 2, 0, 65, 2);
        gc.gags["5dollar"] = Gag(GagKind::LURE, "5dollar", 3, 0, 70, 3);
        gc.gags["bigmagnet"] = Gag(GagKind::LURE, "bigmagnet", 4, 0, 70, 3);
        gc.gags["10dollar"] = Gag(GagKind::LURE, "10dollar", 5, 0, 75, 4);
        gc.gags["hypno"] = Gag(GagKind::LURE, "hypno", 6, 0, 75, 4);
        gc.gags["50dollar"] = Gag(GagKind::LURE, "50dollar", 7, 0, 80, 5);
        gc.gags["presentation"] = Gag(GagKind::LURE, "presentation", 8, 0, 80, 5);
        gc.gags["kazoo"] = Gag(GagKind::SOUND, "kazoo", 1, 4, 95);
        gc.gags["bikehorn"] = Gag(GagKind::SOUND, "bikehorn", 2, 7, 95);
        gc.gags["whistle"] = Gag(GagKind::SOUND, "whistle", 3, 11, 95);
        gc.gags["bugle"] = Gag(GagKind::SOUND, "bugle", 4, 16, 95);
        gc.gags["aoogah"] = Gag(GagKind::SOUND, "aoogah", 5, 21, 95);
        gc.gags["trunk"] = Gag(GagKind::SOUND, "trunk", 6, 32, 95);
        gc.gags["fog"] = Gag(GagKind::SOUND, "fog", 7, 50, 95);
        gc.gags["opera"] = Gag(GagKind::SOUND, "opera", 8, 65, 95);
        gc.gags["flower"] = Gag(GagKind::SQUIRT, "flower", 1, 4, 95, 2);
        gc.gags["glass"] = Gag(GagKind::SQUIRT, "glass", 2, 8, 95, 2);
        gc.gags["gun"] = Gag(GagKind::SQUIRT, "gun", 3, 12, 95, 3);
        gc.gags["wballoon"] = Gag(GagKind::SQUIRT, "wballoon", 4, 21, 95, 3);
        gc.gags["seltzer"] = Gag(GagKind::SQUIRT, "seltzer", 5, 30, 95, 4);
        gc.gags["hose"] = Gag(GagKind::SQUIRT, "hose", 6, 56, 95, 4);
        gc.gags["cloud"] = Gag(GagKind::SQUIRT, "cloud", 7, 80, 95, 5);
        gc.gags["geyser"] = Gag(GagKind::SQUIRT, "geyser", 8, 115, 95, 5);
        gc.gags["buzzer"] = Gag(GagKind::ZAP, "buzzer", 1, 4, 30);
        gc.gags["rug"] = Gag(GagKind::ZAP, "rug", 2, 6, 30);
        gc.gags["balloon"] = Gag(GagKind::ZAP, "balloon", 3, 10, 30);
        gc.gags["kart"] = Gag(GagKind::ZAP, "kart", 4, 16, 30);
        gc.gags["taser"] = Gag(GagKind::ZAP, "taser", 5, 24, 30);
        gc.gags["tv"] = Gag(GagKind::ZAP, "tv", 6, 40, 30);
        gc.gags["tesla"] = Gag(GagKind::ZAP, "tesla", 7, 66, 30);
        gc.gags["lightning"] = Gag(GagKind::ZAP, "lightning", 8, 80, 30);
        gc.gags["cupcake"] = Gag(GagKind::THROW, "cupcake", 1, 7, 75);
        gc.gags["fruitslice"] = Gag(GagKind::THROW, "fruitslice", 2, 11, 75);
        gc.gags["creamslice"] = Gag(GagKind::THROW, "creamslice", 3, 18, 75);
        gc.gags["bdayslice"] = Gag(GagKind::THROW, "bdayslice", 4, 30, 75);
        gc.gags["fruit"] = Gag(GagKind::THROW, "fruit", 5, 45, 75);
        gc.gags["cream"] = Gag(GagKind::THROW, "cream", 6, 75, 75);
        gc.gags["cake"] = Gag(GagKind::THROW, "cake", 7, 110, 75);
        gc.gags["wedding"] = Gag(GagKind::THROW, "wedding", 8, 145, 75);
        gc.gags["pot"] = Gag(GagKind::DROP, "pot", 1, 12, 50);
        gc.gags["sandbag"] = Gag(GagKind::DROP, "sandbag", 2, 20, 50);
        gc.gags["bowling"] = Gag(GagKind::DROP, "bowling", 3, 35, 50);
        gc.gags["anvil"] = Gag(GagKind::DROP, "anvil", 4, 55, 50);
        gc.gags["weight"] = Gag(GagKind::DROP, "weight", 5, 80, 50);
        gc.gags["safe"] = Gag(GagKind::DROP, "safe", 6, 125, 50);
        gc.gags["boulder"] = Gag(GagKind::DROP, "boulder", 7, 180, 50);
        gc.gags["piano"] = Gag(GagKind::DROP, "piano", 8, 220, 50);
        gc.soses["chuckle"] = Sos(SosKind::TOONUP, "chuckle", 20, 2);
        gc.soses["don"] = Sos(SosKind::TOONUP, "don", 30, 2);
        gc.soses["flippy"] = Sos(SosKind::TOONUP, "flippy", 40, 2);
        gc.soses["will"] = Sos(SosKind::TRAP, "will", 10, 2);
        gc.soses["penny"] = Sos(SosKind::TRAP, "penny", 15, 2);
        gc.soses["clara"] = Sos(SosKind::TRAP, "clara", 20, 2);
        gc.soses["stinkyned"] = Sos(SosKind::LURE, "stinkyned", 5, 2);
        gc.soses["nancy"] = Sos(SosKind::LURE, "nancy", 10, 2);
        gc.soses["oldman"] = Sos(SosKind::LURE, "oldman", 15, 2);
        gc.soses["barbara"] = Sos(SosKind::SOUND, "barbara", 5, 2);
        gc.soses["sonata"] = Sos(SosKind::SOUND, "sonata", 10, 2);
        gc.soses["moe"] = Sos(SosKind::SOUND, "moe", 15, 2);
        gc.soses["sid"] = Sos(SosKind::SQUIRT, "sid", 10, 2);
        gc.soses["sanjay"] = Sos(SosKind::SQUIRT, "sanjay", 15, 2);
        gc.soses["sharky"] = Sos(SosKind::SQUIRT, "sharky", 20, 2);
        gc.soses["dentist"] = Sos(SosKind::ZAP, "dentist", 5, 2);
        gc.soses["electra"] = Sos(SosKind::ZAP, "electra", 10, 2);
        gc.soses["nat"] = Sos(SosKind::ZAP, "nat", 15, 2);
        gc.soses["cleff"] = Sos(SosKind::THROW, "cleff", 10, 2);
        gc.soses["cindy"] = Sos(SosKind::THROW, "cindy", 15, 2);
        gc.soses["pierce"] = Sos(SosKind::THROW, "pierce", 20, 2);
        gc.soses["ned"] = Sos(SosKind::DROP, "ned", 10, 2);
        gc.soses["franz"] = Sos(SosKind::DROP, "franz", 15, 2);
        gc.soses["bessie"] = Sos(SosKind::DROP, "bessie", 20, 2);
        gc.soses["rain"] = Sos(SosKind::ALL, "rain", 5, 1);
    }
    // fire
    gc.gags["fire"] = Gag(GagKind::FIRE, "fire", 0, 0, 100);
    // gag type definitions
    gc.gagTypeMap[(GagKind)-1] = "PASS";
    gc.gagTypeMap[(GagKind)0] = "TOONUP";
    gc.gagTypeMap[(GagKind)1] = "TRAP";
    gc.gagTypeMap[(GagKind)2] = "LURE";
    gc.gagTypeMap[(GagKind)3] = "SOUND";
    gc.gagTypeMap[(GagKind)4] = "SQUIRT";
    gc.gagTypeMap[(GagKind)5] = "ZAP";
    gc.gagTypeMap[(GagKind)6] = "THROW";
    gc.gagTypeMap[(GagKind)7] = "DROP";
    gc.gagTypeMap[(GagKind)8] = "FIRE";
    // sos type definitions
    gc.sosTypeMap[(SosKind)0] = "TOONUP";
    gc.sosTypeMap[(SosKind)1] = "TRAP";
    gc.sosTypeMap[(SosKind)2] = "LURE";
    gc.sosTypeMap[(SosKind)3] = "SOUND";
    gc.sosTypeMap[(SosKind)4] = "SQUIRT";
    gc.sosTypeMap[(SosKind)5] = "ZAP";
    gc.sosTypeMap[(SosKind)6] = "THROW";
    gc.sosTypeMap[(SosKind)7] = "DROP";
    gc.sosTypeMap[(SosKind)8] = "ALL";
    gc.sosTypeMap[(SosKind)9] = "ACC";
    gc.sosTypeMap[(SosKind)10] = "ATTACK_DOWN";
    return gc;
}
