#include "battle.h"
#include "cog.h"
#include "cogset.h"
#include "gag_collection.h"
#include "gtest/gtest.h"
#include <algorithm>
#include <iostream>
#include <string>

#define GAGDICT_PATH "gags.txt"

using namespace std;

class GagCollectionTest : public testing::Test {
protected:
    GagCollectionTest() {}
    virtual ~GagCollectionTest() {}
    GagCollection gc = GagCollection::read(GAGDICT_PATH);
    testing::AssertionResult verifyGag(const Gag& g, GagKind gk, string name, unsigned short damage, int accuracy);
    testing::AssertionResult verifySos(const Sos& g, SosKind gk, string name, int effect, int rounds);
};

testing::AssertionResult GagCollectionTest::verifyGag(const Gag& g, GagKind gk, string name, unsigned short damage, int accuracy) {
    if (g.kind != gk) {
        return testing::AssertionFailure() << "Gag kind is incorrect!";
    }
    if (g.name != name) {
        return testing::AssertionFailure() << "Gag name is incorrect!";
    }
    if (g.damage != damage) {
        return testing::AssertionFailure() << "Gag damage is incorrect!";
    }
    if (g.accuracy != accuracy) {
        return testing::AssertionFailure() << "Gag accuracy is incorrect!";
    }
    return testing::AssertionSuccess();
}

testing::AssertionResult GagCollectionTest::verifySos(const Sos& g, SosKind gk, string name, int effect, int rounds) {
    if (g.kind != gk) {
        return testing::AssertionFailure() << "Sos kind is incorrect!";
    }
    if (g.name != name) {
        return testing::AssertionFailure() << "Sos name is incorrect!";
    }
    if (g.passiveEffect != effect) {
        return testing::AssertionFailure() << "Sos effect is incorrect!";
    }
    if (g.duration != rounds) {
        return testing::AssertionFailure() << "Sos rounds is incorrect!";
    }
    return testing::AssertionSuccess();
}

TEST_F(GagCollectionTest, IsGag) {
    EXPECT_TRUE(gc.isGag("feather"));
    EXPECT_TRUE(gc.isGag("highdive"));
    EXPECT_TRUE(gc.isGag("spring"));
    EXPECT_TRUE(gc.isGag("1dollar"));
    EXPECT_TRUE(gc.isGag("trunk"));
    EXPECT_TRUE(gc.isGag("seltzer"));
    EXPECT_TRUE(gc.isGag("tv"));
    EXPECT_TRUE(gc.isGag("fruitslice"));
    EXPECT_TRUE(gc.isGag("piano"));
}

TEST_F(GagCollectionTest, IsNotGag) {
    EXPECT_FALSE(gc.isGag("t"));
    EXPECT_FALSE(gc.isGag("tvs"));
    EXPECT_FALSE(gc.isGag("8"));
    EXPECT_FALSE(gc.isGag(""));

    EXPECT_FALSE(gc.isGag("chuckle"));
    EXPECT_FALSE(gc.isGag("sharky"));
    EXPECT_FALSE(gc.isGag("bessie"));
    EXPECT_FALSE(gc.isGag("rain"));
}

TEST_F(GagCollectionTest, IsSOS) {
    EXPECT_TRUE(gc.isSos("chuckle"));
    EXPECT_TRUE(gc.isSos("sharky"));
    EXPECT_TRUE(gc.isSos("bessie"));
    EXPECT_TRUE(gc.isSos("rain"));
}

TEST_F(GagCollectionTest, IsNotSOS) {
    EXPECT_FALSE(gc.isSos("feather"));
    EXPECT_FALSE(gc.isSos("tv"));
    EXPECT_FALSE(gc.isSos("piano"));
    EXPECT_FALSE(gc.isSos("asdf"));
}

TEST_F(GagCollectionTest, GagInfo) {
    EXPECT_TRUE(verifyGag(gc.getGag("feather"), GagKind::TOONUP, "feather", 8, 95));
    EXPECT_TRUE(verifyGag(gc.getGag("bigmagnet"), GagKind::LURE, "bigmagnet", 0, 70));
    EXPECT_TRUE(verifyGag(gc.getGag("hose"), GagKind::SQUIRT, "hose", 56, 95));
    EXPECT_TRUE(verifySos(gc.getSos("clara"), SosKind::TRAP, "clara", 20, 2));
    EXPECT_TRUE(verifySos(gc.getSos("rain"), SosKind::ALL, "rain", 5, 1));
}

class CogTest : public testing::Test {
protected:
    CogTest() : cogs({Cog(), Cog(1), Cog("1.exe"), Cog(12), Cog("18.exe"), Cog("12v2"), Cog("18.exev2")}) {}
    virtual ~CogTest() {}
    vector<Cog> cogs;
    testing::AssertionResult verifyCog(Cog& cog, string name, int hp);
};

testing::AssertionResult CogTest::verifyCog(Cog& cog, string name, int hp) {
    if (cog.getLevelName() != name) {
        return testing::AssertionFailure() << "Cog level name is incorrect! Was " << cog.getLevelName() << ", expected " << name;
    }
    if (cog.getHP() != hp) {
        return testing::AssertionFailure() << "Cog hp is incorrect! Was " << cog.getHP() << ", expected " << hp;
    }
    return testing::AssertionSuccess();
}

TEST_F(CogTest, CogInit) {
    EXPECT_TRUE(verifyCog(cogs[0], "", 0));
    EXPECT_TRUE(verifyCog(cogs[1], "1", 6));
    EXPECT_TRUE(verifyCog(cogs[2], "1.exe", 9));
    EXPECT_TRUE(verifyCog(cogs[3], "12", 182));
    EXPECT_TRUE(verifyCog(cogs[4], "18.exe", 570));
    EXPECT_TRUE(verifyCog(cogs[5], "12v2", 182));
    EXPECT_TRUE(verifyCog(cogs[6], "18.exev2", 570));
    for (const Cog& cog : cogs) {
        EXPECT_FALSE(cog.getTrap());
        EXPECT_FALSE(cog.getLured());
        EXPECT_FALSE(cog.getSoaked());
    }
}

TEST_F(CogTest, HPUpdate) {
    for (Cog& cog : cogs) {
        cog.hit(50);
    }
    EXPECT_TRUE(verifyCog(cogs[0], "", 0));
    EXPECT_TRUE(verifyCog(cogs[1], "1", 0));
    EXPECT_TRUE(verifyCog(cogs[2], "1.exe", 0));
    EXPECT_TRUE(verifyCog(cogs[3], "12", 132));
    EXPECT_TRUE(verifyCog(cogs[4], "18.exe", 520));
    EXPECT_TRUE(verifyCog(cogs[5], "12v2", 132));
    EXPECT_TRUE(verifyCog(cogs[6], "18.exev2", 520));

    cogs[5].hit(1000);
    EXPECT_TRUE(verifyCog(cogs[5], "12v^", 91));
    cogs[5].hit(1000);
    EXPECT_TRUE(verifyCog(cogs[5], "12v^", 0));
    cogs[6].hit(520);
    cogs[6].hit(285);
    EXPECT_TRUE(verifyCog(cogs[6], "18.exev^", 0));
}

TEST_F(CogTest, TrapUpdate) {
    int traps[3] = {20, 240, 140};
    for (size_t i = 1; i < cogs.size(); i += 2) {
        cogs[i].setTrap(traps[i / 2]);
    }
    for (size_t i = 0; i < cogs.size(); ++i) {
        if (i % 2 == 1) {
            EXPECT_EQ(cogs[i].getTrap(), traps[i / 2]);
        } else {
            EXPECT_FALSE(cogs[i].getTrap());
        }
    }

    // 1 2 3* 4* 5* 6* 7* (2, 4*, and 6* --> 0 because of trap)
    for (size_t i = 0; i < cogs.size(); ++i) {
        cogs[i].setLured(i + 1, i >= 2);
    }
    int expectedLure[7] = {1, 0, 3, 0, 5, 0, 7};
    int expectedPresLure[7] = {false, false, true, false, true, false, true};
    for (size_t i = 0; i < cogs.size(); ++i) {
        EXPECT_EQ(cogs[i].getLured(), expectedLure[i]);
        EXPECT_EQ(cogs[i].getPresLured(), expectedPresLure[i]);
    }
}

TEST_F(CogTest, LureSoakUpdate) {
    // LURE: 0 -2 3 -4 5 -6 7
    // SOAK: 0 1 2 3 4 5 6
    for (size_t i = 1; i < cogs.size(); ++i) {
        cogs[i].setLured(i + 1, i % 2 == 0);
        cogs[i].setSoaked(i);
    }

    for (Cog& cog : cogs) {
        cog.update();
    }
    int expectedLure[7] = {0, 1, 2, 3, 4, 5, 6};
    int expectedPresLure[7] = {false, false, true, false, true, false, true};
    int expectedSoak[7] = {0, 0, 1, 2, 3, 4, 5};
    for (size_t i = 0; i < cogs.size(); ++i) {
        EXPECT_EQ(cogs[i].getLured(), expectedLure[i]);
        EXPECT_EQ(cogs[i].getPresLured(), expectedPresLure[i]);
        EXPECT_EQ(cogs[i].getSoaked(), expectedSoak[i]);
    }

    for (Cog& cog : cogs) {
        for (char repeat = 0; repeat < 6; ++repeat) {
            cog.update();
        }
    }
    for (size_t i = 0; i < cogs.size(); ++i) {
        EXPECT_FALSE(cogs[i].getLured());
        EXPECT_FALSE(cogs[i].getSoaked());
    }
}

class ParseTest : public testing::Test {
protected:
    ParseTest() : battle(GAGDICT_PATH) {}
    virtual ~ParseTest() {}
    Battle battle;
    bool contains(const vector<Gag>& gags, const Gag& gag, size_t count) const;
    int indexOf(const vector<Gag>& gags, const Gag& gag) const;
    Strategy buildStrat(const vector<string>& s);
    GagCollection gc = GagCollection::read(GAGDICT_PATH);
};

bool ParseTest::contains(const vector<Gag>& gags, const Gag& gag, size_t count) const {
    size_t found = 0;
    for (const Gag& g : gags) {
        if (((gag.kind == GagKind::LURE && g.accuracy == gag.accuracy)
             || (gag.kind != GagKind::LURE && g.damage == gag.damage))
            && g.kind == gag.kind && g.damage == gag.damage && g.target == gag.target && g.prestiged == gag.prestiged) {
            ++found;
        }
    }
    return found == count;
}

int ParseTest::indexOf(const vector<Gag>& gags, const Gag& gag) const {
    for (size_t i = 0; i < gags.size(); ++i) {
        const Gag& g = gags[i];
        if (((gag.kind == GagKind::LURE && g.accuracy == gag.accuracy)
             || (gag.kind != GagKind::LURE && g.damage == gag.damage))
            && g.kind == gag.kind && g.damage == gag.damage && g.target == gag.target && g.prestiged == gag.prestiged) {
            return i;
        }
    }
    return -1;
}

Strategy ParseTest::buildStrat(const vector<string>& s) {
    vector<Gag> gags;
    vector<Sos> soses;
    for (const string& gag : s) {
        if (gc.isSos(gag)) {
            soses.push_back(gc.getSos(gag));
        } else {
            gags.push_back(battle.parseGag(gag));
        }
    }
    sort(gags.begin(), gags.end(), GagComparator());
    reverse(gags.begin(), gags.end());
    return Strategy(gags, soses);
}

TEST_F(ParseTest, OneLineNoPres) {
    vector<Cog> set{Cog("18.exe"), Cog("17"), Cog("18"), Cog("18.exe")};
    battle.setCogset(set);
    vector<Gag> gags = battle.parseOneliner("trunk").gags;
    EXPECT_EQ(gags.size(), 1);
    EXPECT_TRUE(contains(gags, Gag(GagKind::SOUND, 32, -1, false), 1));

    gags = battle.parseOneliner("marbles left hypno").gags;
    EXPECT_EQ(gags.size(), 2);
    EXPECT_TRUE(contains(gags, Gag(GagKind::TRAP, 65, 0, false), 1));
    Gag hypno = Gag(GagKind::LURE, 0, -1, false);
    hypno.accuracy = 75;
    EXPECT_TRUE(contains(gags, hypno, 1));

    string commandtest[4]
            = {"wballoon left taser mid-left cream mid-right weight right",
               "wballoon !0 taser !1 cream !2 weight !3",
               "wballoon 18.exe taser 17 cream 18 weight right",
               "xxxx wballoon taser cream weight"};
    for (string command : commandtest) {
        gags = battle.parseOneliner(command).gags;
        EXPECT_EQ(gags.size(), 4);
        EXPECT_TRUE(contains(gags, Gag(GagKind::SQUIRT, 21, 0, false), 1));
        EXPECT_TRUE(contains(gags, Gag(GagKind::ZAP, 24, 1, false), 1));
        EXPECT_TRUE(contains(gags, Gag(GagKind::THROW, 75, 2, false), 1));
        EXPECT_TRUE(contains(gags, Gag(GagKind::DROP, 80, 3, false), 1));
    }

    gags = battle.parseOneliner("-xx- tvs -X-- seltzer seltzer").gags;
    EXPECT_EQ(gags.size(), 4);
    EXPECT_TRUE(contains(gags, Gag(GagKind::SQUIRT, 30, 1, false), 2));
    EXPECT_TRUE(contains(gags, Gag(GagKind::ZAP, 40, 1, false), 1));
    EXPECT_TRUE(contains(gags, Gag(GagKind::ZAP, 40, 2, false), 1));
    EXPECT_TRUE(indexOf(gags, Gag(GagKind::ZAP, 40, 2, false)) < indexOf(gags, Gag(GagKind::ZAP, 40, 1, false)));

    gags = battle.parseOneliner("-xx- cross tv tv -X-- seltzer seltzer").gags;
    EXPECT_EQ(gags.size(), 4);
    EXPECT_TRUE(contains(gags, Gag(GagKind::SQUIRT, 30, 1, false), 2));
    EXPECT_TRUE(contains(gags, Gag(GagKind::ZAP, 40, 1, false), 1));
    EXPECT_TRUE(contains(gags, Gag(GagKind::ZAP, 40, 2, false), 1));
    EXPECT_TRUE(indexOf(gags, Gag(GagKind::ZAP, 40, 1, false)) < indexOf(gags, Gag(GagKind::ZAP, 40, 2, false)));

    vector<Sos> soses;
    Strategy s = battle.parseOneliner("sanjay rain");
    gags = s.gags;
    soses = s.soses;
    EXPECT_EQ(gags.size(), 0);
    EXPECT_EQ(soses.size(), 2);

    gags = battle.parseOneliner("fire left").gags;
    EXPECT_EQ(gags.size(), 1);
    EXPECT_TRUE(contains(gags, Gag(GagKind::FIRE, 0, 0, false), 1));
}

TEST_F(ParseTest, OneLinePres) {
    vector<Cog> set{Cog("18.exe"), Cog("17"), Cog("18"), Cog("18.exe")};
    battle.setCogset(set);
    vector<Gag> gags = battle.parseOneliner("pres trunk").gags;
    EXPECT_EQ(gags.size(), 1);
    EXPECT_TRUE(contains(gags, Gag(GagKind::SOUND, 32, -1, true), 1));

    gags = battle.parseOneliner("pres marbles left hypno").gags;
    EXPECT_EQ(gags.size(), 2);
    EXPECT_TRUE(contains(gags, Gag(GagKind::TRAP, 65, 0, true), 1));
    Gag hypno = Gag(GagKind::LURE, 0, -1, false);
    hypno.accuracy = 75;
    EXPECT_TRUE(contains(gags, hypno, 1));

    string commandtest[4]
            = {"pres wballoon left taser mid-left pres cream mid-right weight right",
               "pres wballoon !0 taser !1 pres cream !2 weight !3",
               "pres wballoon 18.exe taser 17 pres cream 18 weight right",
               "xxxx pres wballoon taser pres cream weight"};
    for (string command : commandtest) {
        gags = battle.parseOneliner(command).gags;
        EXPECT_EQ(gags.size(), 4);
        EXPECT_TRUE(contains(gags, Gag(GagKind::SQUIRT, 21, 0, true), 1));
        EXPECT_TRUE(contains(gags, Gag(GagKind::ZAP, 24, 1, false), 1));
        EXPECT_TRUE(contains(gags, Gag(GagKind::THROW, 75, 2, true), 1));
        EXPECT_TRUE(contains(gags, Gag(GagKind::DROP, 80, 3, false), 1));
    }

    gags = battle.parseOneliner("-xx- pres tv tv pres -X-- seltzer seltzer").gags;
    EXPECT_EQ(gags.size(), 4);
    EXPECT_TRUE(contains(gags, Gag(GagKind::SQUIRT, 30, 1, true), 2));
    EXPECT_TRUE(contains(gags, Gag(GagKind::ZAP, 40, 1, true), 1));
    EXPECT_TRUE(contains(gags, Gag(GagKind::ZAP, 40, 2, false), 1));
    EXPECT_TRUE(indexOf(gags, Gag(GagKind::ZAP, 40, 2, false)) < indexOf(gags, Gag(GagKind::ZAP, 40, 1, true)));

    gags = battle.parseOneliner("-xx- cross pres tv tv pres -X-- seltzers").gags;
    EXPECT_EQ(gags.size(), 4);
    EXPECT_TRUE(contains(gags, Gag(GagKind::SQUIRT, 30, 1, true), 2));
    EXPECT_TRUE(contains(gags, Gag(GagKind::ZAP, 40, 1, false), 1));
    EXPECT_TRUE(contains(gags, Gag(GagKind::ZAP, 40, 2, true), 1));
    EXPECT_TRUE(indexOf(gags, Gag(GagKind::ZAP, 40, 1, false)) < indexOf(gags, Gag(GagKind::ZAP, 40, 2, true)));
}

TEST_F(ParseTest, MultLineNoPres) {
    vector<Cog> set{Cog("18.exe"), Cog("17"), Cog("18"), Cog("18.exe")};
    battle.setCogset(set);
    vector<string> commands{"trunk", "trunk", "trunk", "trunk"};
    vector<Gag> gags = buildStrat(commands).gags;
    EXPECT_EQ(gags.size(), 4);
    EXPECT_TRUE(contains(gags, Gag(GagKind::SOUND, 32, -1, false), 4));

    commands = {"marbles left", "hypno", "PASS", "PASS"};
    gags = buildStrat(commands).gags;
    EXPECT_EQ(gags.size(), 4);
    EXPECT_TRUE(contains(gags, Gag(GagKind::TRAP, 65, 0, false), 1));
    Gag hypno = Gag(GagKind::LURE, 0, -1, false);
    hypno.accuracy = 75;
    EXPECT_TRUE(contains(gags, hypno, 1));

    vector<vector<string>> commandtest
            = {{"wballoon left", "taser mid-left", "cream mid-right", "weight right"},
               {"wballoon !0", "taser !1", "cream !2", "weight !3"},
               {"wballoon 18.exe", "taser 17", "cream 18", "weight right"}};
    for (auto command : commandtest) {
        gags = buildStrat(command).gags;
        EXPECT_EQ(gags.size(), 4);
        EXPECT_TRUE(contains(gags, Gag(GagKind::SQUIRT, 21, 0, false), 1));
        EXPECT_TRUE(contains(gags, Gag(GagKind::ZAP, 24, 1, false), 1));
        EXPECT_TRUE(contains(gags, Gag(GagKind::THROW, 75, 2, false), 1));
        EXPECT_TRUE(contains(gags, Gag(GagKind::DROP, 80, 3, false), 1));
    }

    commands = {"tv mid-left", "tv mid-right", "seltzer mid-left", "seltzer mid-left"};
    gags = buildStrat(commands).gags;
    EXPECT_EQ(gags.size(), 4);
    EXPECT_TRUE(contains(gags, Gag(GagKind::SQUIRT, 30, 1, false), 2));
    EXPECT_TRUE(contains(gags, Gag(GagKind::ZAP, 40, 1, false), 1));
    EXPECT_TRUE(contains(gags, Gag(GagKind::ZAP, 40, 2, false), 1));
    EXPECT_TRUE(indexOf(gags, Gag(GagKind::ZAP, 40, 2, false)) < indexOf(gags, Gag(GagKind::ZAP, 40, 1, false)));

    commands = {"tv mid-right", "tv mid-left", "seltzer mid-left", "seltzer mid-left"};
    gags = buildStrat(commands).gags;
    EXPECT_EQ(gags.size(), 4);
    EXPECT_TRUE(contains(gags, Gag(GagKind::SQUIRT, 30, 1, false), 2));
    EXPECT_TRUE(contains(gags, Gag(GagKind::ZAP, 40, 1, false), 1));
    EXPECT_TRUE(contains(gags, Gag(GagKind::ZAP, 40, 2, false), 1));
    EXPECT_TRUE(indexOf(gags, Gag(GagKind::ZAP, 40, 1, false)) < indexOf(gags, Gag(GagKind::ZAP, 40, 2, false)));

    commands = {"sanjay", "rain", "PASS", "PASS"};
    gags = buildStrat(commands).gags;
    vector<Sos> soses = buildStrat(commands).soses;
    EXPECT_EQ(gags.size(), 2);
    EXPECT_EQ(soses.size(), 2);

    commands = {"fire left", "PASS", "PASS", "PASS"};
    gags = buildStrat(commands).gags;
    EXPECT_EQ(gags.size(), 4);
    EXPECT_TRUE(contains(gags, Gag(GagKind::FIRE, 0, 0, false), 1));
}

TEST_F(ParseTest, MultLinePres) {
    vector<Cog> set{Cog("18.exe"), Cog("17"), Cog("18"), Cog("18.exe")};
    battle.setCogset(set);
    vector<string> commands{"pres trunk", "trunk", "trunk", "pres trunk"};
    vector<Gag> gags = buildStrat(commands).gags;
    EXPECT_EQ(gags.size(), 4);
    EXPECT_TRUE(contains(gags, Gag(GagKind::SOUND, 32, -1, true), 2));
    EXPECT_TRUE(contains(gags, Gag(GagKind::SOUND, 32, -1, false), 2));

    commands = {"pres marbles left", "pres hypno", "PASS", "PASS"};
    gags = buildStrat(commands).gags;
    EXPECT_EQ(gags.size(), 4);
    EXPECT_TRUE(contains(gags, Gag(GagKind::TRAP, 65, 0, true), 1));
    Gag hypno = Gag(GagKind::LURE, 0, -1, true);
    hypno.accuracy = 75;
    EXPECT_TRUE(contains(gags, hypno, 1));

    vector<vector<string>> commandtest
            = {{"pres wballoon left", "taser mid-left", "pres cream mid-right", "weight right"},
               {"pres wballoon !0", "taser !1", "pres cream !2", "weight !3"},
               {"pres wballoon 18.exe", "taser 17", "pres cream 18", "weight right"}};
    for (auto command : commandtest) {
        gags = buildStrat(command).gags;
        EXPECT_EQ(gags.size(), 4);
        EXPECT_TRUE(contains(gags, Gag(GagKind::SQUIRT, 21, 0, true), 1));
        EXPECT_TRUE(contains(gags, Gag(GagKind::ZAP, 24, 1, false), 1));
        EXPECT_TRUE(contains(gags, Gag(GagKind::THROW, 75, 2, true), 1));
        EXPECT_TRUE(contains(gags, Gag(GagKind::DROP, 80, 3, false), 1));
    }

    commands = {"tv mid-right", "pres tv mid-left", "pres seltzer mid-left", "pres seltzer mid-left"};
    gags = buildStrat(commands).gags;
    EXPECT_EQ(gags.size(), 4);
    EXPECT_TRUE(contains(gags, Gag(GagKind::SQUIRT, 30, 1, true), 2));
    EXPECT_TRUE(contains(gags, Gag(GagKind::ZAP, 40, 1, true), 1));
    EXPECT_TRUE(contains(gags, Gag(GagKind::ZAP, 40, 2, false), 1));
    EXPECT_TRUE(indexOf(gags, Gag(GagKind::ZAP, 40, 1, true)) < indexOf(gags, Gag(GagKind::ZAP, 40, 2, false)));
}

class BattleTest : public testing::Test {
protected:
    BattleTest() : battle(GAGDICT_PATH) {
        battle.setHitState(true);
        battle.setPrint(false);
        battle.setUpdate(true);
    }
    virtual ~BattleTest() {}
    Battle battle;
};

TEST_F(BattleTest, Trap) {
    vector<Cog> set{Cog("9"), Cog("9"), Cog("10"), Cog("10")};
    battle.setCogset(set);
    battle.setPresState(false);
    int preHP[4] = {110, 110, 132, 132};
    int expectedTrap[4] = {20, 47, 20, 50};
    battle.turn(battle.parseOneliner("x-x- banana banana pres -x-x banana banana").gags);
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(battle.getCogset().getCog(i).getTrap(), expectedTrap[i]);
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), preHP[i]);
    }
    battle.turn(battle.parseOneliner("bigmagnet").gags);
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_FALSE(battle.getCogset().getCog(i).getTrap());
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), preHP[i] - expectedTrap[i]);
    }
}

TEST_F(BattleTest, TrapSOS) {
    vector<Cog> set{Cog("20"), Cog("19")};
    battle.setCogset(set);
    battle.setPresState(false);
    Strategy s = battle.parseOneliner("clara pres wreckingball left");
    battle.affect(s.soses);
    battle.turn(s.gags);
    battle.turn(battle.parseOneliner("wreckingball right hypno").gags);
    EXPECT_EQ(battle.getCogset().getCog(0).getHP(), 462 - 300);
    EXPECT_EQ(battle.getCogset().getCog(1).getHP(), 420 - 240);
    battle.turn(battle.parseOneliner("banana left hypno").gags);
    EXPECT_EQ(battle.getCogset().getCog(0).getHP(), 462 - 300 - 20);
}

TEST_F(BattleTest, TrapDrop) {
    vector<Cog> set{Cog("14.exe"), Cog("22"), Cog("18"), Cog("19.exe")};
    battle.setCogset(set);
    battle.setPresState(true);

    battle.turn(battle.parseOneliner("xx-- wreckingball wreckingball").gags);
    battle.turn(battle.parseOneliner("xx-- 50dollar 10dollar xX-- safe safe weight").gags);
    EXPECT_EQ(battle.getCogset().getSize(), 2);

    battle.setPresState(false);
    battle.turn(battle.parseOneliner("xx wreckingball tnt hypno xX boulder weight piano").gags);
    EXPECT_TRUE(battle.getCogset().allDead());
}

TEST_F(BattleTest, TrapCanceling) {
    battle.setCogset(vector<Cog>(4, 9));
    battle.setPresState(false);

    // traps should cancel out
    battle.turn(battle.parseOneliner("banana left banana left").gags);
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_FALSE(battle.getCogset().getCog(i).getTrap());
    }

    battle.turn(battle.parseOneliner("banana left banana right banana right").gags);
    EXPECT_EQ(battle.getCogset().getCog(0).getTrap(), 20);
    for (size_t i = 1; i < battle.getCogset().getSize(); ++i) {
        EXPECT_FALSE(battle.getCogset().getCog(i).getTrap());
    }
}

TEST_F(BattleTest, Lure) {
    vector<Cog> set{Cog("6.exe"), Cog("10"), Cog("9.exe"), Cog("12")};
    vector<int> preHP{84, 132, 165, 182};
    battle.setCogset(set);
    battle.setPresState(false);

    // xx--
    battle.turn(battle.parseOneliner("10dollar left 50dollar mid-left").gags);
    EXPECT_EQ(battle.getCogset().getCog(0).getLured(), 4);
    EXPECT_EQ(battle.getCogset().getCog(0).getPresLured(), false);
    EXPECT_EQ(battle.getCogset().getCog(0).getLuredKnockback(), 50);
    EXPECT_EQ(battle.getCogset().getCog(1).getLured(), 5);
    EXPECT_EQ(battle.getCogset().getCog(1).getPresLured(), false);
    EXPECT_EQ(battle.getCogset().getCog(1).getLuredKnockback(), 50);
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        if (i >= 2) {
            EXPECT_FALSE(battle.getCogset().getCog(i).getLured());
            EXPECT_EQ(battle.getCogset().getCog(i).getLuredKnockback(), 0);
        }
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), preHP[i]);
    }

    // xxoo (does not override previous lure)
    battle.turn(battle.parseOneliner("pres bigmagnet").gags);
    EXPECT_EQ(battle.getCogset().getCog(0).getLured(), 3);
    EXPECT_EQ(battle.getCogset().getCog(0).getPresLured(), false);
    EXPECT_EQ(battle.getCogset().getCog(0).getLuredKnockback(), 50);
    EXPECT_EQ(battle.getCogset().getCog(1).getLured(), 4);
    EXPECT_EQ(battle.getCogset().getCog(1).getPresLured(), false);
    EXPECT_EQ(battle.getCogset().getCog(1).getLuredKnockback(), 50);
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        if (i >= 2) {
            EXPECT_EQ(battle.getCogset().getCog(i).getLured(), 3);
            EXPECT_EQ(battle.getCogset().getCog(i).getPresLured(), true);
            EXPECT_EQ(battle.getCogset().getCog(i).getLuredKnockback(), 65);
        }
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), preHP[i]);
    }

    battle.setCogset(set);
    // xoxo (single prestige override on same turn)
    battle.turn(battle.parseOneliner("pres -x-x 10dollar 10dollar bigmagnet").gags);
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(battle.getCogset().getCog(i).getLured(), i % 2 == 0 ? 3 : 4);
        EXPECT_EQ(battle.getCogset().getCog(i).getPresLured(), i % 2 == 0 ? false : true);
        EXPECT_EQ(battle.getCogset().getCog(i).getLuredKnockback(), i % 2 == 0 ? 50 : 65);
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), preHP[i]);
    }

    battle.setCogset(set);
    // oooo (group prestige override on same turn)
    battle.turn(battle.parseOneliner("x-x- 50dollar 5dollar pres hypno").gags);
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(battle.getCogset().getCog(i).getLured(), i == 0 ? 5 : 4);
        EXPECT_EQ(battle.getCogset().getCog(i).getPresLured(), true);
        EXPECT_EQ(battle.getCogset().getCog(i).getLuredKnockback(), 65);
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), preHP[i]);
    }

    battle.setCogset(set);
    // bonus knockback
    Strategy s = battle.parseOneliner("xx-- 10dollar pres 5dollar lom");
    battle.affect(s.soses);
    battle.turn(s.gags);
    EXPECT_TRUE(battle.getCogset().getCog(0).getLured());
    EXPECT_EQ(battle.getCogset().getCog(0).getPresLured(), false);
    EXPECT_EQ(battle.getCogset().getCog(0).getLuredKnockback(), 65);
    EXPECT_TRUE(battle.getCogset().getCog(1).getLured());
    EXPECT_EQ(battle.getCogset().getCog(1).getPresLured(), true);
    EXPECT_EQ(battle.getCogset().getCog(1).getLuredKnockback(), 80);
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        if (i >= 2) {
            EXPECT_FALSE(battle.getCogset().getCog(i).getLured());
        }
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), preHP[i]);
    }
}

TEST_F(BattleTest, LureKnockbackDamage) {
    vector<Cog> set{Cog("6.exe"), Cog("10"), Cog("9.exe"), Cog("12")};
    battle.setCogset(set);
    battle.setPresState(false);
    vector<int> postHP{39, 39, 52, 58};

    battle.turn(battle.parseOneliner("pres -x-x 10dollar 10dollar bigmagnet").gags);
    battle.turn(battle.parseOneliner("xxxx seltzer hose cream cream").gags);
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_FALSE(battle.getCogset().getCog(i).getLured());
        EXPECT_FALSE(battle.getCogset().getCog(i).getPresLured());
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), postHP[i]);
    }

    battle.setCogset(set);
    battle.turn(battle.parseOneliner("pres -x-x 10dollar 10dollar bigmagnet").gags);
    battle.turn(battle.parseOneliner("xxxx hose cloud cake cake").gags);
    EXPECT_TRUE(battle.getCogset().allDead());
}

TEST_F(BattleTest, LureNoKnockbackDamage) {
    vector<Cog> set{Cog("6.exe"), Cog("10"), Cog("9.exe"), Cog("12")};
    battle.setCogset(set);
    battle.setPresState(false);
    vector<int> postHP{84, 132, 135, 158};

    battle.turn(battle.parseOneliner("x-xx anvil rake taser hypno").gags);
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(battle.getCogset().getCog(i).getLured(), i < 2 ? 4 : 0);
        EXPECT_FALSE(battle.getCogset().getCog(i).getPresLured());
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), postHP[i]);
    }

    battle.turn(battle.parseOneliner("kazoo").gags);
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_FALSE(battle.getCogset().getCog(i).getLured());
        EXPECT_FALSE(battle.getCogset().getCog(i).getPresLured());
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), postHP[i] - 4);
    }
}

TEST_F(BattleTest, Sound) {
    battle.setCogset(vector<Cog>(4, 12));
    battle.setPresState(true);

    // damage check
    battle.turn(battle.parseOneliner("3 trunk aoogah").gags);
    EXPECT_EQ(battle.getCogset().getSize(), 4);
    EXPECT_EQ(battle.getCogset().getCog(0).getHP(), 12);
    // kill check
    battle.setCogset(vector<Cog>(4, 12));
    battle.turn(battle.parseOneliner("4 trunk").gags);
    EXPECT_TRUE(battle.getCogset().allDead());

    // high-low tests
    // 9.exes still live because not all pres
    vector<Cog> set(3, {"9.exe"});
    set.push_back(Cog("15.exe"));
    battle.setCogset(set);
    battle.setPresState(false);
    battle.turn(battle.parseOneliner("fog 2 pres trunk").gags);
    EXPECT_EQ(battle.getCogset().getSize(), 4);
    EXPECT_EQ(battle.getCogset().getCog(0).getHP(), 9);
    // 9.exes still live because max lvl is not high enough
    set[3] = Cog("14.exe");
    battle.setCogset(set);
    battle.setPresState(true);
    battle.turn(battle.parseOneliner("fog 2 trunk").gags);
    EXPECT_EQ(battle.getCogset().getSize(), 4);
    EXPECT_EQ(battle.getCogset().getCog(0).getHP(), 3);
    // 9.exes are destroyed
    set[3] = Cog("15.exe");
    battle.setCogset(set);
    battle.turn(battle.parseOneliner("fog 2 trunk").gags);
    EXPECT_EQ(battle.getCogset().getSize(), 1);
    EXPECT_EQ(battle.getCogset().getCog(0).getHP(), 242);
}

TEST_F(BattleTest, SoundOther) {
    // sound + throw
    vector<Cog> set(3, 8);
    set.push_back(Cog("8.exe"));
    battle.setCogset(set);
    battle.setPresState(false);
    battle.turn(battle.parseOneliner("3 pres aoogah fruit right").gags);
    EXPECT_TRUE(battle.getCogset().allDead());
    // sound + drop
    set = vector<Cog>(3, 11);
    set.push_back(Cog("14.exe"));
    battle.setCogset(set);
    battle.setPresState(true);
    battle.turn(battle.parseOneliner("fog 2 trunk piano right").gags);
    EXPECT_TRUE(battle.getCogset().allDead());
}

TEST_F(BattleTest, Squirt) {
    battle.setCogset(vector<Cog>(4, 11));
    battle.setPresState(false);

    // x---
    // 5000
    battle.turn(battle.parseOneliner("cloud left").gags);
    EXPECT_EQ(battle.getCogset().getCog(0).getSoaked(), 5);
    EXPECT_EQ(battle.getCogset().getCog(0).getHP(), 76);
    for (size_t i = 1; i < battle.getCogset().getSize(); ++i) {
        EXPECT_FALSE(battle.getCogset().getCog(i).getSoaked());
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), 156);
    }

    // -,o,
    // 4222
    battle.turn(battle.parseOneliner("pres flower mid-right").gags);
    EXPECT_EQ(battle.getCogset().getCog(0).getSoaked(), 4);
    EXPECT_EQ(battle.getCogset().getCog(0).getHP(), 76);
    for (size_t i = 1; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(battle.getCogset().getCog(i).getSoaked(), 2);
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), 156 - (i == 2 ? 4 : 0));
    }

    // -,o,
    // 0444
    battle.setCogset(vector<Cog>(4, 11));
    battle.turn(battle.parseOneliner("pres hose mid-right").gags);
    EXPECT_FALSE(battle.getCogset().getCog(0).getSoaked());
    EXPECT_EQ(battle.getCogset().getCog(0).getHP(), 156);
    for (size_t i = 1; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(battle.getCogset().getCog(i).getSoaked(), 4);
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), 156 - (i == 2 ? 56 : 0));
    }

    // o,--
    // 2200
    battle.setCogset(vector<Cog>(4, 11));
    battle.turn(battle.parseOneliner("pres flower left").gags);
    EXPECT_TRUE(battle.getCogset().getCog(0).getSoaked());
    EXPECT_EQ(battle.getCogset().getCog(0).getHP(), 152);
    for (size_t i = 1; i < battle.getCogset().getSize(); ++i) {
        if (i != 1) {
            EXPECT_FALSE(battle.getCogset().getCog(i).getSoaked());
        } else {
            EXPECT_EQ(battle.getCogset().getCog(i).getSoaked(), 2);
        }
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), 156);
    }

    // --,o
    // 1122
    battle.turn(battle.parseOneliner("pres flower right").gags);
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(battle.getCogset().getCog(i).getSoaked(), i < 2 ? 1 : 2);
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), 156 - (i % 3 == 0 ? 4 : 0));
    }

    // --x-
    // 0021
    battle.turn(battle.parseOneliner("flower mid-right").gags);
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(battle.getCogset().getCog(i).getSoaked(), i < 2 ? 0 : (i == 2 ? 2 : 1));
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), 156 - (i != 1 ? 4 : 0));
    }

    // ,o,-
    // 3330
    battle.turn(battle.parseOneliner("pres gun mid-left").gags);
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(battle.getCogset().getCog(i).getSoaked(), i == 3 ? 0 : 3);
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), 156 - (i != 1 ? 4 : 12));
    }

    // -x--
    // 2220
    battle.turn(battle.parseOneliner("flower mid-left").gags);
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(battle.getCogset().getCog(i).getSoaked(), i == 3 ? 0 : 2);
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), 156 - (i != 1 ? 4 : 16));
    }

    // -x--
    // 4440
    battle.turn(battle.parseOneliner("flower mid-left pres seltzer mid-left").gags);
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(battle.getCogset().getCog(i).getSoaked(), i == 3 ? 0 : 4);
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), 156 - (i != 1 ? 4 : 57));
    }
}

TEST_F(BattleTest, Zap) {
    battle.setCogset(vector<Cog>(4, 14));
    battle.setPresState(false);
    battle.turn(battle.parseOneliner("pres seltzer mid-left taser left").gags);
    int postSoakHP[4] = {240, 210, 240, 240};
    int taser[3] = {72, 54, 36};
    int taserP[3] = {72, 60, 48};
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(battle.getCogset().getCog(i).getSoaked(), i == 3 ? 0 : 3);
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), i == 3 ? 240 : postSoakHP[i] - taser[i]);
    }
    battle.turn(battle.parseOneliner("pres taser mid-right").gags);
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(battle.getCogset().getCog(i).getSoaked(), i == 3 ? 0 : 1);
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), i == 3 ? 240 : postSoakHP[i] - taser[i] - taserP[2-i]);
    }
    battle.turn(battle.parseOneliner("taser mid-left").gags);
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_FALSE(battle.getCogset().getCog(i).getSoaked());
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), i == 3 ? 240 : postSoakHP[i] - taser[i] - taserP[2-i] - taser[(4-i)%3]);
    }
    battle.turn(battle.parseOneliner("pres taser right").gags);
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_FALSE(battle.getCogset().getCog(i).getSoaked());
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), i == 3 ? 216 : postSoakHP[i] - taser[i] - taserP[2-i] - taser[(4-i)%3]);
    }
}

TEST_F(BattleTest, ZapStandardCombosPreSoaked) {
    battle.setPresState(true);
    map<string,vector<int>> expectedResults = {
        {"x-x- tv tv", {200, 100, 220, 80}},
        {"x-x- tesla tv", {278, 100, 285, 132}},
        {"xx-- tesla tesla", {363, 363, 132, 132}},
        {"-xx- tv taser", {48, 180, 172, 80}},
        {"-xx- cross tv tv", {100, 220, 200, 80}},
        {"-xx- cross lightning tesla", {165, 398, 372, 160}},
    };
    for (auto& pair : expectedResults) {
        battle.setCogset(vector<Cog>(4, 23));
        // soak all
        for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
            battle.getCogset().getCog(i).setSoaked(4);
        }
        battle.turn(battle.parseOneliner(pair.first).gags);
        for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
            EXPECT_TRUE(battle.getCogset().getCog(i).getSoaked());
            EXPECT_EQ(600 - battle.getCogset().getCog(i).getHP(), pair.second[i]);
        }
    }
}

TEST_F(BattleTest, ZapStandardCombosWithSquirt) {
    battle.setPresState(true);
    map<string,vector<int>> expectedResults = {
        {"x-x- tv tv -x-x hose cloud", {200, 156, 220, 160}},
        {"x-x- tesla tv -x-x geyser hose", {278, 215, 285, 188}},
        {"xx-- tesla tesla x-x- hose cloud", {419, 363, 212, 132}},
        {"-xx- tv taser x--x hose seltzer", {104, 180, 172, 110}},
        {"-xx- cross tv tv x--x hose hose", {156, 220, 200, 136}},
        {"-xx- cross lightning tesla x--x geyser cloud", {280, 398, 372, 240}},
    };
    for (auto& pair : expectedResults) {
        battle.setCogset(vector<Cog>(4, 23));
        battle.turn(battle.parseOneliner(pair.first).gags);
        for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
            EXPECT_TRUE(battle.getCogset().getCog(i).getSoaked());
            EXPECT_EQ(600 - battle.getCogset().getCog(i).getHP(), pair.second[i]);
        }
    }
}

TEST_F(BattleTest, ZapComplexCombos) {
    battle.setPresState(true);
    map<string,vector<int>> expectedResults = {
        {"-x-x tvs -x-x gun hose", {100, 212, 100, 176}},
        {"-x-x lightning tesla x-x- geyser wballoon", {315, 372, 186, 198}},
        {"-X-- lightnings x--x cloud gun", {280, 480, 160, 212}},
        {"--X- tv tesla x-x- seltzer hose", {110, 100, 374, 165}},
    };
    for (auto& pair : expectedResults) {
        battle.setCogset(vector<Cog>(4, 23));
        battle.turn(battle.parseOneliner(pair.first).gags);
        for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
            EXPECT_TRUE(battle.getCogset().getCog(i).getSoaked());
            EXPECT_EQ(600 - battle.getCogset().getCog(i).getHP(), pair.second[i]);
        }
    }

    // below combos require first zap to kill two, or squirt to kill

    battle.setCogset(vector<Cog>{Cog(12), Cog("12.exe"), Cog(11), Cog(11)});
    // 200 298 160 165
    battle.turn(battle.parseOneliner("xx-- cross tesla tv x-x- clouds").gags);
    EXPECT_TRUE(battle.getCogset().allDead());

    battle.setCogset(vector<Cog>{Cog(15), Cog("14.exe"), Cog(13), Cog(11)});
    // 278 363 212 165
    battle.turn(battle.parseOneliner("xx-- cross teslas x-x- clouds").gags);
    EXPECT_TRUE(battle.getCogset().allDead());

    battle.setCogset(vector<Cog>{Cog(13), Cog("14.exe"), Cog(7), Cog("14.exe")});
    // 212 363 80 363
    battle.turn(battle.parseOneliner("-x-x teslas x-x- clouds").gags);
    EXPECT_TRUE(battle.getCogset().allDead());

    battle.setCogset(vector<Cog>{Cog("14.exe"), Cog(7), Cog("14.exe"), Cog(13)});
    // 363 80 363 212
    battle.turn(battle.parseOneliner("x-x- cross teslas -x-x clouds").gags);
    EXPECT_TRUE(battle.getCogset().allDead());

    battle.setCogset(vector<Cog>{Cog(14), Cog("14.exe"), Cog(13), Cog("10.exe")});
    // 245 363 212 198
    battle.turn(battle.parseOneliner("-x-x cross teslas x-x- clouds").gags);
    EXPECT_TRUE(battle.getCogset().allDead());
}

TEST_F(BattleTest, ZapMoreComplexCombos) {
    battle.setPresState(true);
    map<string,vector<int>> tripleZaps = {
        {"xX-- tvs", {220, 340, 80, 100}},
        {"xX-- cross tvs", {220, 340, 80, 100}},
        {"xX-- tesla tvs", {298, 405, 80, 100}},
        {"xX-- tv teslas", {285, 496, 80, 165}},
        {"tv mid-left tesla left tesla mid-left", {298, 483, 80, 165}},
        {"tv mid-left tesla mid-right tesla mid-left", {100, 483, 278, 165}},
        {"-X-x 2 teslas lightning", {165, 596, 132, 405}},
        {"x-X- taser taser taser", {120, 60, 204, 60}},
    };
    for (auto& pair : tripleZaps) {
        battle.setCogset(vector<Cog>(4, {"23.exe"}));
        // soak all
        for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
            battle.getCogset().getCog(i).setSoaked(5);
        }
        battle.turn(battle.parseOneliner(pair.first).gags);
        for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
            EXPECT_TRUE(battle.getCogset().getCog(i).getSoaked());
            EXPECT_EQ(900 - battle.getCogset().getCog(i).getHP(), pair.second[i]);
        }
    }
}

TEST_F(BattleTest, ZapQuestionableCombos) {
    battle.setPresState(true);
    map<string,vector<int>> expectedResults = {
        {"x-x- cross tvs", {220, 100, 200, 0}},
        {"x--x teslas", {198, 132, 165, 198}},
        {"x--x cross teslas", {198, 165, 132, 198}},
        {"X--- taser tv", {192, 60, 48, 0}},
        {"---X lightning tv", {0, 80, 100, 360}},
        {"3 tv mid-left", {100, 360, 80, 100}},
    };
    for (auto& pair : expectedResults) {
        battle.setCogset(vector<Cog>(4, 23));
        // soak all
        for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
            battle.getCogset().getCog(i).setSoaked(5);
        }
        battle.turn(battle.parseOneliner(pair.first).gags);
        for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
            EXPECT_TRUE(battle.getCogset().getCog(i).getSoaked());
            EXPECT_EQ(600 - battle.getCogset().getCog(i).getHP(), pair.second[i]);
        }
    }
}

TEST_F(BattleTest, Throw) {
    battle.setCogset(vector<Cog>(4, 18));
    battle.setPresState(false);
    int expectedResults[4] = {11, 87, 72, 327};
    battle.turn(battle.parseOneliner("xxXX fruitslice pres cream 2 bdayslices pres cake wedding").gags);
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(380 - battle.getCogset().getCog(i).getHP(), expectedResults[i]);
    }

    battle.setCogset(vector<Cog>(2, 18));
    battle.turn(battle.parseOneliner("4 fruit left 4 pres fruit right").gags);
    EXPECT_EQ(380 - battle.getCogset().getCog(0).getHP(), 216);
    EXPECT_EQ(380 - battle.getCogset().getCog(1).getHP(), 250);
}

TEST_F(BattleTest, Drop) {
    battle.setCogset(vector<Cog>(4, 18));
    battle.setPresState(false);
    int expectedResults[4] = {35, 55, 250, 183};
    battle.turn(battle.parseOneliner("xxXX bowling pres anvil pot boulder weight pres anvil").gags);
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(380 - battle.getCogset().getCog(i).getHP(), expectedResults[i]);
    }
    // non pres combo damages
    int comboDamages[4] = {55, 143, 231, 330};
    battle.setCogset(vector<Cog>(4, 18));
    battle.turn(battle.parseOneliner("anvil left 2 anvil mid-left 3 anvil mid-right 4 anvil right").gags);
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(380 - battle.getCogset().getCog(i).getHP(), comboDamages[i]);
    }
    // mix pres combo damages
    int mixComboDamages1[3] = {149, 240, 248};
    battle.setCogset(vector<Cog>(3, 18));
    battle.turn(battle.parseOneliner("anvil left pres anvil left 2 anvil mid pres anvil mid anvil right 2 pres anvil right").gags);
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(380 - battle.getCogset().getCog(i).getHP(), mixComboDamages1[i]);
    }
    int mixComboDamages2[3] = {341, 352, 363};
    battle.setCogset(vector<Cog>(3, 18));
    battle.turn(battle.parseOneliner("3 anvil left pres anvil left 2 anvil mid 2 pres anvil mid anvil right 3 pres anvil right").gags);
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(380 - battle.getCogset().getCog(i).getHP(), mixComboDamages2[i]);
    }
    // pres combo damages
    int presComboDamages[4] = {55, 154, 256, 374};
    battle.setCogset(vector<Cog>(4, 18));
    battle.setPresState(true);
    battle.turn(battle.parseOneliner("anvil left 2 anvil mid-left 3 anvil mid-right 4 anvil right").gags);
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(380 - battle.getCogset().getCog(i).getHP(), presComboDamages[i]);
    }
}

TEST_F(BattleTest, DropSOS) {
    battle.setCogset(vector<Cog>(2, {"24.exe"}));
    battle.setPresState(false);
    int expectedResults[4] = {96, 185};
    // 20% boost
    Strategy s = battle.parseOneliner("bessie xX weight pres anvils");
    battle.affect(s.soses);
    battle.turn(s.gags);
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(975 - battle.getCogset().getCog(i).getHP(), expectedResults[i]);
    }
    // 20% + 5% boost
    s = battle.parseOneliner("rain xx bowlings");
    battle.affect(s.soses);
    battle.turn(s.gags);
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(975 - 44 - battle.getCogset().getCog(i).getHP(), expectedResults[i]);
    }
    // 5% boost
    s = battle.parseOneliner("rain xx bowlings");
    battle.affect(s.soses);
    battle.turn(s.gags);
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(975 - 44 - 37 - battle.getCogset().getCog(i).getHP(), expectedResults[i]);
    }
    // no boost
    battle.turn(battle.parseOneliner("xx bowlings").gags);
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(975 - 44 - 37 - 35 - battle.getCogset().getCog(i).getHP(), expectedResults[i]);
    }
}

TEST_F(BattleTest, Rain) {
    battle.setCogset(vector<Cog>(2, 29));
    battle.setPresState(false);
    Strategy s = battle.parseOneliner("rain pierce lom pres hypno");
    battle.affect(s.soses);
    battle.turn(s.gags);
    s = battle.parseOneliner("2 cake right");
    battle.turn(s.gags);
    EXPECT_EQ(930 - battle.getCogset().getCog(1).getHP(), 542);
}

TEST_F(BattleTest, InvalidMove) {
    vector<Cog> set{Cog("14"), Cog("14"), Cog("14"), Cog("14")};
    battle.setCogset(set);
    battle.setPresState(false);

    battle.turn(battle.parseOneliner("10dollar left -xxx banana banana banana").gags);
    vector<string> badcommands = {
        "banana left", // trap on lured
        "banana mid-left", // trap on trapped
        "10dollar left" // lure on lured
    };
    for (string s : badcommands) {
        EXPECT_THROW(battle.parseOneliner(s).gags, invalid_argument);
    }
    battle.turn(battle.parseOneliner("kazoo").gags);
    battle.turn(battle.parseOneliner("banana left").gags);
    battle.turn(battle.parseOneliner("hypno").gags);
    battle.turn(battle.parseOneliner("hypno").gags);
    // group lure when all are lured
    EXPECT_THROW(battle.parseOneliner("hypno").gags, invalid_argument);
}
