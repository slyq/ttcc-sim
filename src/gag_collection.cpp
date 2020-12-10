#include <fstream>
#include <gag_collection.h>
#include <iostream>

GagCollection GagCollection::read(const std::string& file_path) {
    std::ifstream file(file_path);
    GagCollection gc;
    if (file) {
        std::string name;
        unsigned short damage;
        float accuracy;

        for (size_t i = 0; i < 8; ++i) {
            for (size_t j = 0; j < 8; ++j) {
                file >> name >> damage >> accuracy;
                gc.gagmap[name] = Gag((GagKind)i, name, damage, accuracy);
                if (i == 2 || i == 4) {
                    int effect;
                    file >> effect;
                    gc.gagmap[name].passiveEffect = effect;
                }
            }
        }
        // 3 active sos per track
        for (size_t i = 0; i < 8; ++i) {
            for (size_t j = 0; j < 3; ++j) {
                file >> name >> damage;
                gc.gagmap[name] = Gag((GagKind)i, name, damage, 100);
                gc.sosset.insert(name);
                if (i == 2 || i == 4) {
                    int effect;
                    file >> effect;
                    gc.gagmap[name].passiveEffect = effect;
                }
            }
        }
        // rain
        file >> name >> damage;
        gc.gagmap[name] = Gag(GagKind::DROP, name, damage, 100);
        gc.sosset.insert(name);
    } else {
        if (file_path != "gags.txt") {
            std::cerr << "Could not find custom gag definitions file, using defaults..." << std::endl;
        }
        gc.gagmap["feather"] = Gag(GagKind::TOONUP, "feather", 8, 70);
        gc.gagmap["megaphone"] = Gag(GagKind::TOONUP, "megaphone", 15, 70);
        gc.gagmap["lipstick"] = Gag(GagKind::TOONUP, "lipstick", 26, 70);
        gc.gagmap["bamboo"] = Gag(GagKind::TOONUP, "bamboo", 39, 70);
        gc.gagmap["pixie"] = Gag(GagKind::TOONUP, "pixie", 50, 70);
        gc.gagmap["juggling"] = Gag(GagKind::TOONUP, "juggling", 78, 70);
        gc.gagmap["confetti"] = Gag(GagKind::TOONUP, "confetti", 95, 70);
        gc.gagmap["highdive"] = Gag(GagKind::TOONUP, "highdive", 135, 70);
        gc.gagmap["banana"] = Gag(GagKind::TRAP, "banana", 20, 100);
        gc.gagmap["rake"] = Gag(GagKind::TRAP, "rake", 30, 100);
        gc.gagmap["spring"] = Gag(GagKind::TRAP, "spring", 45, 100);
        gc.gagmap["marbles"] = Gag(GagKind::TRAP, "marbles", 65, 100);
        gc.gagmap["quicksand"] = Gag(GagKind::TRAP, "quicksand", 90, 100);
        gc.gagmap["trapdoor"] = Gag(GagKind::TRAP, "trapdoor", 140, 100);
        gc.gagmap["wreckingball"] = Gag(GagKind::TRAP, "wreckingball", 200, 100);
        gc.gagmap["tnt"] = Gag(GagKind::TRAP, "tnt", 240, 100);
        gc.gagmap["1dollar"] = Gag(GagKind::LURE, "1dollar", 0, 50);
        gc.gagmap["1dollar"].passiveEffect = 2;
        gc.gagmap["smallmagnet"] = Gag(GagKind::LURE, "smallmagnet", 0, 50);
        gc.gagmap["smallmagnet"].passiveEffect = 2;
        gc.gagmap["5dollar"] = Gag(GagKind::LURE, "5dollar", 0, 60);
        gc.gagmap["5dollar"].passiveEffect = 3;
        gc.gagmap["bigmagnet"] = Gag(GagKind::LURE, "bigmagnet", 0, 60);
        gc.gagmap["bigmagnet"].passiveEffect = 3;
        gc.gagmap["10dollar"] = Gag(GagKind::LURE, "10dollar", 0, 70);
        gc.gagmap["10dollar"].passiveEffect = 4;
        gc.gagmap["hypno"] = Gag(GagKind::LURE, "hypno", 0, 70);
        gc.gagmap["hypno"].passiveEffect = 4;
        gc.gagmap["50dollar"] = Gag(GagKind::LURE, "50dollar", 0, 80);
        gc.gagmap["50dollar"].passiveEffect = 5;
        gc.gagmap["presentation"] = Gag(GagKind::LURE, "presentation", 0, 80);
        gc.gagmap["presentation"].passiveEffect = 5;
        gc.gagmap["kazoo"] = Gag(GagKind::SOUND, "kazoo", 4, 95);
        gc.gagmap["bikehorn"] = Gag(GagKind::SOUND, "bikehorn", 7, 95);
        gc.gagmap["whistle"] = Gag(GagKind::SOUND, "whistle", 11, 95);
        gc.gagmap["bugle"] = Gag(GagKind::SOUND, "bugle", 16, 95);
        gc.gagmap["aoogah"] = Gag(GagKind::SOUND, "aoogah", 21, 95);
        gc.gagmap["trunk"] = Gag(GagKind::SOUND, "trunk", 32, 95);
        gc.gagmap["fog"] = Gag(GagKind::SOUND, "fog", 50, 95);
        gc.gagmap["opera"] = Gag(GagKind::SOUND, "opera", 65, 95);
        gc.gagmap["flower"] = Gag(GagKind::SQUIRT, "flower", 4, 95);
        gc.gagmap["flower"].passiveEffect = 1;
        gc.gagmap["glass"] = Gag(GagKind::SQUIRT, "glass", 8, 95);
        gc.gagmap["glass"].passiveEffect = 1;
        gc.gagmap["gun"] = Gag(GagKind::SQUIRT, "gun", 12, 95);
        gc.gagmap["gun"].passiveEffect = 2;
        gc.gagmap["wballoon"] = Gag(GagKind::SQUIRT, "wballoon", 21, 95);
        gc.gagmap["wballoon"].passiveEffect = 2;
        gc.gagmap["seltzer"] = Gag(GagKind::SQUIRT, "seltzer", 30, 95);
        gc.gagmap["seltzer"].passiveEffect = 3;
        gc.gagmap["hose"] = Gag(GagKind::SQUIRT, "hose", 56, 95);
        gc.gagmap["hose"].passiveEffect = 3;
        gc.gagmap["cloud"] = Gag(GagKind::SQUIRT, "cloud", 80, 95);
        gc.gagmap["cloud"].passiveEffect = 4;
        gc.gagmap["geyser"] = Gag(GagKind::SQUIRT, "geyser", 115, 95);
        gc.gagmap["geyser"].passiveEffect = 4;
        gc.gagmap["buzzer"] = Gag(GagKind::ZAP, "buzzer", 4, 30);
        gc.gagmap["rug"] = Gag(GagKind::ZAP, "rug", 6, 30);
        gc.gagmap["balloon"] = Gag(GagKind::ZAP, "balloon", 10, 30);
        gc.gagmap["kart"] = Gag(GagKind::ZAP, "kart", 16, 30);
        gc.gagmap["taser"] = Gag(GagKind::ZAP, "taser", 24, 30);
        gc.gagmap["tv"] = Gag(GagKind::ZAP, "tv", 40, 30);
        gc.gagmap["tesla"] = Gag(GagKind::ZAP, "tesla", 66, 30);
        gc.gagmap["lightning"] = Gag(GagKind::ZAP, "lightning", 80, 30);
        gc.gagmap["cupcake"] = Gag(GagKind::THROW, "cupcake", 7, 30);
        gc.gagmap["fruitslice"] = Gag(GagKind::THROW, "fruitslice", 11, 75);
        gc.gagmap["creamslice"] = Gag(GagKind::THROW, "creamslice", 18, 75);
        gc.gagmap["bdayslice"] = Gag(GagKind::THROW, "bdayslice", 30, 75);
        gc.gagmap["fruit"] = Gag(GagKind::THROW, "fruit", 45, 75);
        gc.gagmap["cream"] = Gag(GagKind::THROW, "cream", 75, 75);
        gc.gagmap["cake"] = Gag(GagKind::THROW, "cake", 110, 75);
        gc.gagmap["wedding"] = Gag(GagKind::THROW, "wedding", 145, 75);
        gc.gagmap["pot"] = Gag(GagKind::DROP, "pot", 12, 50);
        gc.gagmap["sandbag"] = Gag(GagKind::DROP, "sandbag", 20, 50);
        gc.gagmap["bowling"] = Gag(GagKind::DROP, "bowling", 35, 50);
        gc.gagmap["anvil"] = Gag(GagKind::DROP, "anvil", 55, 50);
        gc.gagmap["weight"] = Gag(GagKind::DROP, "weight", 80, 50);
        gc.gagmap["safe"] = Gag(GagKind::DROP, "safe", 125, 50);
        gc.gagmap["boulder"] = Gag(GagKind::DROP, "boulder", 180, 50);
        gc.gagmap["piano"] = Gag(GagKind::DROP, "piano", 220, 50);
        gc.gagmap["chuckle"] = Gag(GagKind::TOONUP, "chuckle", 78, 100);
        gc.sosset.insert("chuckle");
        gc.gagmap["don"] = Gag(GagKind::TOONUP, "don", 95, 100);
        gc.sosset.insert("don");
        gc.gagmap["flippy"] = Gag(GagKind::TOONUP, "flippy", 150, 100);
        gc.sosset.insert("flippy");
        gc.gagmap["will"] = Gag(GagKind::TRAP, "will", 140, 100);
        gc.sosset.insert("will");
        gc.gagmap["penny"] = Gag(GagKind::TRAP, "penny", 200, 100);
        gc.sosset.insert("penny");
        gc.gagmap["clara"] = Gag(GagKind::TRAP, "clara", 240, 100);
        gc.sosset.insert("clara");
        gc.gagmap["stinkyned"] = Gag(GagKind::LURE, "stinkyned", 0, 100);
        gc.sosset.insert("stinkyned");
        gc.gagmap["stinkyned"].passiveEffect = 4;
        gc.sosset.insert("stinkyned");
        gc.gagmap["nancy"] = Gag(GagKind::LURE, "nancy", 0, 100);
        gc.sosset.insert("nancy");
        gc.gagmap["nancy"].passiveEffect = 5;
        gc.sosset.insert("nancy");
        gc.gagmap["oldman"] = Gag(GagKind::LURE, "oldman", 0, 100);
        gc.sosset.insert("oldman");
        gc.gagmap["oldman"].passiveEffect = 6;
        gc.sosset.insert("oldman");
        gc.gagmap["barbara"] = Gag(GagKind::SOUND, "barbara", 40, 100);
        gc.sosset.insert("barbara");
        gc.gagmap["sonata"] = Gag(GagKind::SOUND, "sonata", 60, 100);
        gc.sosset.insert("sonata");
        gc.gagmap["moe"] = Gag(GagKind::SOUND, "moe", 85, 100);
        gc.sosset.insert("moe");
        gc.gagmap["sid"] = Gag(GagKind::SQUIRT, "sid", 56, 100);
        gc.sosset.insert("sid");
        gc.gagmap["sid"].passiveEffect = 3;
        gc.sosset.insert("sid");
        gc.gagmap["sanjay"] = Gag(GagKind::SQUIRT, "sanjay", 80, 100);
        gc.sosset.insert("sanjay");
        gc.gagmap["sanjay"].passiveEffect = 4;
        gc.sosset.insert("sanjay");
        gc.gagmap["sharky"] = Gag(GagKind::SQUIRT, "sharky", 115, 100);
        gc.sosset.insert("sharky");
        gc.gagmap["sharky"].passiveEffect = 4;
        gc.sosset.insert("sharky");
        gc.gagmap["dentist"] = Gag(GagKind::ZAP, "dentist", 45, 100);
        gc.sosset.insert("dentist");
        gc.gagmap["electra"] = Gag(GagKind::ZAP, "electra", 70, 100);
        gc.sosset.insert("electra");
        gc.gagmap["nat"] = Gag(GagKind::ZAP, "nat", 85, 100);
        gc.sosset.insert("nat");
        gc.gagmap["cleff"] = Gag(GagKind::THROW, "cleff", 75, 100);
        gc.sosset.insert("cleff");
        gc.gagmap["cindy"] = Gag(GagKind::THROW, "cindy", 110, 100);
        gc.sosset.insert("cindy");
        gc.gagmap["pierce"] = Gag(GagKind::THROW, "pierce", 145, 100);
        gc.sosset.insert("pierce");
        gc.gagmap["ned"] = Gag(GagKind::DROP, "ned", 125, 100);
        gc.sosset.insert("ned");
        gc.gagmap["franz"] = Gag(GagKind::DROP, "franz", 180, 100);
        gc.sosset.insert("franz");
        gc.gagmap["bessie"] = Gag(GagKind::DROP, "bessie", 220, 100);
        gc.sosset.insert("bessie");
        gc.gagmap["rain"] = Gag(GagKind::DROP, "rain", 20, 100);
        gc.sosset.insert("rain");
    }
    // fire
    gc.gagmap["fire"] = Gag(GagKind::FIRE, "fire", 0, 100);
    return gc;
}
