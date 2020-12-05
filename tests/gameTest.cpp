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
};

testing::AssertionResult
GagCollectionTest::verifyGag(const Gag& g, GagKind gk, string name, unsigned short damage, int accuracy) {
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

TEST_F(GagCollectionTest, IsGag) {
    EXPECT_TRUE(gc.contains("feather"));
    EXPECT_TRUE(gc.contains("highdive"));
    EXPECT_TRUE(gc.contains("spring"));
    EXPECT_TRUE(gc.contains("1dollar"));
    EXPECT_TRUE(gc.contains("trunk"));
    EXPECT_TRUE(gc.contains("seltzer"));
    EXPECT_TRUE(gc.contains("tv"));
    EXPECT_TRUE(gc.contains("fruitslice"));
    EXPECT_TRUE(gc.contains("piano"));

    EXPECT_TRUE(gc.contains("chuckle"));
    EXPECT_TRUE(gc.contains("sharky"));
    EXPECT_TRUE(gc.contains("bessie"));
    EXPECT_TRUE(gc.contains("rain"));
}

TEST_F(GagCollectionTest, IsNotGag) {
    EXPECT_FALSE(gc.contains("t"));
    EXPECT_FALSE(gc.contains("tvs"));
    EXPECT_FALSE(gc.contains("8"));
    EXPECT_FALSE(gc.contains(""));
}

TEST_F(GagCollectionTest, IsSOS) {
    EXPECT_TRUE(gc.isSOS("chuckle"));
    EXPECT_TRUE(gc.isSOS("sharky"));
    EXPECT_TRUE(gc.isSOS("bessie"));
    EXPECT_TRUE(gc.isSOS("rain"));
}

TEST_F(GagCollectionTest, IsNotSOS) {
    EXPECT_FALSE(gc.isSOS("feather"));
    EXPECT_FALSE(gc.isSOS("tv"));
    EXPECT_FALSE(gc.isSOS("piano"));
    EXPECT_FALSE(gc.isSOS("asdf"));
}

TEST_F(GagCollectionTest, GagInfo) {
    EXPECT_TRUE(verifyGag(gc.get("feather"), GagKind::TOONUP, "feather", 8, 70));
    EXPECT_TRUE(verifyGag(gc.get("bigmagnet"), GagKind::LURE, "bigmagnet", 0, 60));
    EXPECT_TRUE(verifyGag(gc.get("hose"), GagKind::SQUIRT, "hose", 56, 95));
    EXPECT_TRUE(verifyGag(gc.get("clara"), GagKind::TRAP, "clara", 240, 100));
    EXPECT_TRUE(verifyGag(gc.get("rain"), GagKind::DROP, "rain", 20, 100));
}

class CogTest : public testing::Test {
protected:
    CogTest() : cogs({Cog(), Cog(1), Cog(1, true), Cog(12), Cog(18, true)}) {}
    virtual ~CogTest() {}
    vector<Cog> cogs;
    testing::AssertionResult verifyCog(Cog& cog, string name, int hp);
};

testing::AssertionResult CogTest::verifyCog(Cog& cog, string name, int hp) {
    if (cog.getLevelName() != name) {
        return testing::AssertionFailure() << "Cog level name is incorrect!";
    }
    if (cog.getHP() != hp) {
        return testing::AssertionFailure() << "Cog hp is incorrect!";
    }
    return testing::AssertionSuccess();
}

TEST_F(CogTest, CogInit) {
    EXPECT_TRUE(verifyCog(cogs[0], "", 0));
    EXPECT_TRUE(verifyCog(cogs[1], "1", 6));
    EXPECT_TRUE(verifyCog(cogs[2], "1.exe", 9));
    EXPECT_TRUE(verifyCog(cogs[3], "12", 182));
    EXPECT_TRUE(verifyCog(cogs[4], "18.exe", 570));
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
}

TEST_F(CogTest, TrapUpdate) {
    int traps[2] = {20, 240};
    for (size_t i = 1; i < cogs.size(); i+=2) {
        cogs[i].setTrap(traps[i/2]);
    }
    for (size_t i = 0; i < cogs.size(); ++i) {
        if (i % 2 == 1) {
            EXPECT_EQ(cogs[i].getTrap(), traps[i/2]);
        } else {
            EXPECT_FALSE(cogs[i].getTrap());
        }
    }

    // -1 -2 3 4 5 (-2 and 4 --> 0 because of trap)
    for (size_t i = 0; i < cogs.size(); ++i) {
        cogs[i].setLured((i + 1) * (i < 2 ? -1 : 1));
    }
    int expectedLure[5] = {-1, 0, 3, 0, 5};
    for (size_t i = 0; i < cogs.size(); ++i) {
        EXPECT_EQ(cogs[i].getLured(), expectedLure[i]);
    }
}

TEST_F(CogTest, LureSoakUpdate) {
    // LURE: 0 -2 3 -4 5
    // SOAK: 0 1 2 3 4
    for (size_t i = 1; i < cogs.size(); ++i) {
        cogs[i].setLured((i + 1) * (i % 2 == 0 ? 1 : -1));
        cogs[i].setSoaked(i);
    }

    for (Cog& cog : cogs) {
        cog.update();
    }
    int expectedLure[5] = {0, -1, 2, -3, 4};
    int expectedSoak[5] = {0, 0, 1, 2, 3};
    for (size_t i = 0; i < cogs.size(); ++i) {
        EXPECT_EQ(cogs[i].getLured(), expectedLure[i]);
        EXPECT_EQ(cogs[i].getSoaked(), expectedSoak[i]);
    }

    for (Cog& cog : cogs) {
        for (char repeat = 0; repeat < 5; ++repeat) {
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
    vector<Gag> buildStrat(const vector<string>& s);
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

vector<Gag> ParseTest::buildStrat(const vector<string>& s) {
    vector<Gag> gags;
    for (const string& gag : s) {
        gags.push_back(battle.parseGag(gag));
    }
    return gags;
}

TEST_F(ParseTest, OneLineNoPres) {
    vector<Cog> set{Cog("18.exe"), Cog("17"), Cog("18"), Cog("18.exe")};
    battle.setCogset(set);
    vector<Gag> gags = battle.parseOneliner("trunk");
    EXPECT_EQ(gags.size(), 1);
    EXPECT_TRUE(contains(gags, Gag(GagKind::SOUND, 32, -1, false), 1));

    gags = battle.parseOneliner("marbles left hypno");
    EXPECT_EQ(gags.size(), 2);
    EXPECT_TRUE(contains(gags, Gag(GagKind::TRAP, 65, 0, false), 1));
    Gag hypno = Gag(GagKind::LURE, 0, -1, false);
    hypno.accuracy = 70;
    EXPECT_TRUE(contains(gags, hypno, 1));

    string commandtest[4]
            = {"wballoon left taser mid-left cream mid-right weight right",
               "wballoon !0 taser !1 cream !2 weight !3",
               "wballoon 18.exe taser 17 cream 18 weight right",
               "xxxx wballoon taser cream weight"};
    for (string command : commandtest) {
        gags = battle.parseOneliner(command);
        EXPECT_EQ(gags.size(), 4);
        EXPECT_TRUE(contains(gags, Gag(GagKind::SQUIRT, 21, 0, false), 1));
        EXPECT_TRUE(contains(gags, Gag(GagKind::ZAP, 24, 1, false), 1));
        EXPECT_TRUE(contains(gags, Gag(GagKind::THROW, 75, 2, false), 1));
        EXPECT_TRUE(contains(gags, Gag(GagKind::DROP, 80, 3, false), 1));
    }

    gags = battle.parseOneliner("-xx- tv tv -X-- seltzer seltzer");
    EXPECT_EQ(gags.size(), 4);
    EXPECT_TRUE(contains(gags, Gag(GagKind::SQUIRT, 30, 1, false), 2));
    EXPECT_TRUE(contains(gags, Gag(GagKind::ZAP, 40, 1, false), 1));
    EXPECT_TRUE(contains(gags, Gag(GagKind::ZAP, 40, 2, false), 1));
    EXPECT_TRUE(indexOf(gags, Gag(GagKind::ZAP, 40, 2, false)) < indexOf(gags, Gag(GagKind::ZAP, 40, 1, false)));

    gags = battle.parseOneliner("-xx- cross tv tv -X-- seltzer seltzer");
    EXPECT_EQ(gags.size(), 4);
    EXPECT_TRUE(contains(gags, Gag(GagKind::SQUIRT, 30, 1, false), 2));
    EXPECT_TRUE(contains(gags, Gag(GagKind::ZAP, 40, 1, false), 1));
    EXPECT_TRUE(contains(gags, Gag(GagKind::ZAP, 40, 2, false), 1));
    EXPECT_TRUE(indexOf(gags, Gag(GagKind::ZAP, 40, 1, false)) < indexOf(gags, Gag(GagKind::ZAP, 40, 2, false)));

    gags = battle.parseOneliner("sanjay rain");
    EXPECT_EQ(gags.size(), 2);
    EXPECT_TRUE(contains(gags, Gag(GagKind::SQUIRT, 80, -1, false), 1));
    EXPECT_TRUE(contains(gags, Gag(GagKind::DROP, 20, -1, false), 1));

    gags = battle.parseOneliner("FIRE left");
    EXPECT_EQ(gags.size(), 1);
    EXPECT_TRUE(contains(gags, Gag(GagKind::FIRE, 0, 0, false), 1));
}

TEST_F(ParseTest, OneLinePres) {
    vector<Cog> set{Cog("18.exe"), Cog("17"), Cog("18"), Cog("18.exe")};
    battle.setCogset(set);
    vector<Gag> gags = battle.parseOneliner("pres trunk");
    EXPECT_EQ(gags.size(), 1);
    EXPECT_TRUE(contains(gags, Gag(GagKind::SOUND, 32, -1, true), 1));

    gags = battle.parseOneliner("pres marbles left hypno");
    EXPECT_EQ(gags.size(), 2);
    EXPECT_TRUE(contains(gags, Gag(GagKind::TRAP, 65, 0, true), 1));
    Gag hypno = Gag(GagKind::LURE, 0, -1, false);
    hypno.accuracy = 70;
    EXPECT_TRUE(contains(gags, hypno, 1));

    string commandtest[4]
            = {"pres wballoon left taser mid-left pres cream mid-right weight right",
               "pres wballoon !0 taser !1 pres cream !2 weight !3",
               "pres wballoon 18.exe taser 17 pres cream 18 weight right",
               "xxxx pres wballoon taser pres cream weight"};
    for (string command : commandtest) {
        gags = battle.parseOneliner(command);
        EXPECT_EQ(gags.size(), 4);
        EXPECT_TRUE(contains(gags, Gag(GagKind::SQUIRT, 21, 0, true), 1));
        EXPECT_TRUE(contains(gags, Gag(GagKind::ZAP, 24, 1, false), 1));
        EXPECT_TRUE(contains(gags, Gag(GagKind::THROW, 75, 2, true), 1));
        EXPECT_TRUE(contains(gags, Gag(GagKind::DROP, 80, 3, false), 1));
    }

    gags = battle.parseOneliner("-xx- pres tv tv pres -X-- seltzer seltzer");
    EXPECT_EQ(gags.size(), 4);
    EXPECT_TRUE(contains(gags, Gag(GagKind::SQUIRT, 30, 1, true), 2));
    EXPECT_TRUE(contains(gags, Gag(GagKind::ZAP, 40, 1, true), 1));
    EXPECT_TRUE(contains(gags, Gag(GagKind::ZAP, 40, 2, false), 1));
    EXPECT_TRUE(indexOf(gags, Gag(GagKind::ZAP, 40, 2, false)) < indexOf(gags, Gag(GagKind::ZAP, 40, 1, true)));

    gags = battle.parseOneliner("-xx- cross pres tv tv pres -X-- seltzer seltzer");
    EXPECT_EQ(gags.size(), 4);
    EXPECT_TRUE(contains(gags, Gag(GagKind::SQUIRT, 30, 1, true), 2));
    EXPECT_TRUE(contains(gags, Gag(GagKind::ZAP, 40, 1, true), 1));
    EXPECT_TRUE(contains(gags, Gag(GagKind::ZAP, 40, 2, false), 1));
    EXPECT_TRUE(indexOf(gags, Gag(GagKind::ZAP, 40, 1, true)) < indexOf(gags, Gag(GagKind::ZAP, 40, 2, false)));
}

TEST_F(ParseTest, MultLineNoPres) {
    vector<Cog> set{Cog("18.exe"), Cog("17"), Cog("18"), Cog("18.exe")};
    battle.setCogset(set);
    vector<string> commands{"trunk", "trunk", "trunk", "trunk"};
    vector<Gag> gags = buildStrat(commands);
    EXPECT_EQ(gags.size(), 4);
    EXPECT_TRUE(contains(gags, Gag(GagKind::SOUND, 32, -1, false), 4));

    commands = {"marbles left", "hypno", "PASS", "PASS"};
    gags = buildStrat(commands);
    EXPECT_EQ(gags.size(), 4);
    EXPECT_TRUE(contains(gags, Gag(GagKind::TRAP, 65, 0, false), 1));
    Gag hypno = Gag(GagKind::LURE, 0, -1, false);
    hypno.accuracy = 70;
    EXPECT_TRUE(contains(gags, hypno, 1));

    vector<vector<string>> commandtest
            = {{"wballoon left", "taser mid-left", "cream mid-right", "weight right"},
               {"wballoon !0", "taser !1", "cream !2", "weight !3"},
               {"wballoon 18.exe", "taser 17", "cream 18", "weight right"}};
    for (auto command : commandtest) {
        gags = buildStrat(command);
        EXPECT_EQ(gags.size(), 4);
        EXPECT_TRUE(contains(gags, Gag(GagKind::SQUIRT, 21, 0, false), 1));
        EXPECT_TRUE(contains(gags, Gag(GagKind::ZAP, 24, 1, false), 1));
        EXPECT_TRUE(contains(gags, Gag(GagKind::THROW, 75, 2, false), 1));
        EXPECT_TRUE(contains(gags, Gag(GagKind::DROP, 80, 3, false), 1));
    }

    commands = {"tv mid-right", "tv mid-left", "seltzer mid-left", "seltzer mid-left"};
    gags = buildStrat(commands);
    EXPECT_EQ(gags.size(), 4);
    EXPECT_TRUE(contains(gags, Gag(GagKind::SQUIRT, 30, 1, false), 2));
    EXPECT_TRUE(contains(gags, Gag(GagKind::ZAP, 40, 1, false), 1));
    EXPECT_TRUE(contains(gags, Gag(GagKind::ZAP, 40, 2, false), 1));

    commands = {"sanjay", "rain", "PASS", "PASS"};
    gags = buildStrat(commands);
    EXPECT_EQ(gags.size(), 4);
    EXPECT_TRUE(contains(gags, Gag(GagKind::SQUIRT, 80, -1, false), 1));
    EXPECT_TRUE(contains(gags, Gag(GagKind::DROP, 20, -1, false), 1));

    commands = {"FIRE left", "PASS", "PASS", "PASS"};
    gags = buildStrat(commands);
    EXPECT_EQ(gags.size(), 4);
    EXPECT_TRUE(contains(gags, Gag(GagKind::FIRE, 0, 0, false), 1));
}

TEST_F(ParseTest, MultLinePres) {
    vector<Cog> set{Cog("18.exe"), Cog("17"), Cog("18"), Cog("18.exe")};
    battle.setCogset(set);
    vector<string> commands{"pres trunk", "trunk", "trunk", "pres trunk"};
    vector<Gag> gags = buildStrat(commands);
    EXPECT_EQ(gags.size(), 4);
    EXPECT_TRUE(contains(gags, Gag(GagKind::SOUND, 32, -1, true), 2));
    EXPECT_TRUE(contains(gags, Gag(GagKind::SOUND, 32, -1, false), 2));

    commands = {"pres marbles left", "pres hypno", "PASS", "PASS"};
    gags = buildStrat(commands);
    EXPECT_EQ(gags.size(), 4);
    EXPECT_TRUE(contains(gags, Gag(GagKind::TRAP, 65, 0, true), 1));
    Gag hypno = Gag(GagKind::LURE, 0, -1, true);
    hypno.accuracy = 70;
    EXPECT_TRUE(contains(gags, hypno, 1));

    vector<vector<string>> commandtest
            = {{"pres wballoon left", "taser mid-left", "pres cream mid-right", "weight right"},
               {"pres wballoon !0", "taser !1", "pres cream !2", "weight !3"},
               {"pres wballoon 18.exe", "taser 17", "pres cream 18", "weight right"}};
    for (auto command : commandtest) {
        gags = buildStrat(command);
        EXPECT_EQ(gags.size(), 4);
        EXPECT_TRUE(contains(gags, Gag(GagKind::SQUIRT, 21, 0, true), 1));
        EXPECT_TRUE(contains(gags, Gag(GagKind::ZAP, 24, 1, false), 1));
        EXPECT_TRUE(contains(gags, Gag(GagKind::THROW, 75, 2, true), 1));
        EXPECT_TRUE(contains(gags, Gag(GagKind::DROP, 80, 3, false), 1));
    }

    commands = {"tv mid-right", "pres tv mid-left", "pres seltzer mid-left", "pres seltzer mid-left"};
    gags = buildStrat(commands);
    EXPECT_EQ(gags.size(), 4);
    EXPECT_TRUE(contains(gags, Gag(GagKind::SQUIRT, 30, 1, true), 2));
    EXPECT_TRUE(contains(gags, Gag(GagKind::ZAP, 40, 1, true), 1));
    EXPECT_TRUE(contains(gags, Gag(GagKind::ZAP, 40, 2, false), 1));
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
    battle.turn(battle.parseOneliner("x-x- banana banana pres -x-x banana banana"));
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(battle.getCogset().getCog(i).getTrap(), expectedTrap[i]);
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), preHP[i]);
    }
    battle.turn(battle.parseOneliner("bigmagnet"));
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_FALSE(battle.getCogset().getCog(i).getTrap());
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), preHP[i] - expectedTrap[i]);
    }
}

TEST_F(BattleTest, TrapSOS) {
    battle.setCogset(vector<Cog>(4, 9));
    battle.setPresState(false);
    battle.turn(battle.parseOneliner("penny"));
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(battle.getCogset().getCog(i).getTrap(), 200);
    }

    battle.setCogset(vector<Cog>(4, 9));
    battle.turn(battle.parseOneliner("10dollar left banana mid-left"));
    battle.turn(battle.parseOneliner("penny"));
    EXPECT_FALSE(battle.getCogset().getCog(0).getTrap());
    for (size_t i = 1; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(battle.getCogset().getCog(i).getTrap(), i == 1 ? 20 : 200);
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), 110);
    }
}

TEST_F(BattleTest, TrapDrop) {
    vector<Cog> set{Cog("14.exe"), Cog("22"), Cog("18"), Cog("19.exe")};
    battle.setCogset(set);
    battle.setPresState(true);

    battle.turn(battle.parseOneliner("xx-- wreckingball wreckingball"));
    battle.turn(battle.parseOneliner("xx-- 50dollar 10dollar xX-- safe safe weight"));
    EXPECT_EQ(battle.getCogset().getSize(), 2);

    battle.setPresState(false);
    battle.turn(battle.parseOneliner("xx wreckingball tnt hypno xX boulder weight piano"));
    EXPECT_TRUE(battle.getCogset().allDead());
}

TEST_F(BattleTest, BadTrap) {
    battle.setCogset(vector<Cog>(4, 9));
    battle.setPresState(false);

    vector<string> badcommands = {
        "banana left banana left",
        "clara banana left",
        "tnt left tnt right will",
    };
    // traps should cancel out
    for (string s : badcommands) {
        battle.turn(battle.parseOneliner(s));
        for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
            EXPECT_FALSE(battle.getCogset().getCog(i).getTrap());
        }
    }
    battle.turn(battle.parseOneliner("banana left banana right banana right"));
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
    battle.turn(battle.parseOneliner("10dollar left 50dollar mid-left"));
    EXPECT_EQ(battle.getCogset().getCog(0).getLured(), -4);
    EXPECT_EQ(battle.getCogset().getCog(1).getLured(), -5);
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        if (i >= 2) {
            EXPECT_FALSE(battle.getCogset().getCog(i).getLured());
        }
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), preHP[i]);
    }

    // xxoo (does not override previous lure)
    battle.turn(battle.parseOneliner("pres bigmagnet"));
    EXPECT_EQ(battle.getCogset().getCog(0).getLured(), -3);
    EXPECT_EQ(battle.getCogset().getCog(1).getLured(), -4);
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        if (i >= 2) {
            EXPECT_EQ(battle.getCogset().getCog(i).getLured(), 3);
        }
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), preHP[i]);
    }

    battle.setCogset(set);
    // xoxo (single prestige override on same turn)
    battle.turn(battle.parseOneliner("pres -x-x 10dollar 10dollar bigmagnet"));
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(battle.getCogset().getCog(i).getLured(), i % 2 == 0 ? -3 : 7);
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), preHP[i]);
    }

    battle.setCogset(set);
    // oooo (group prestige override on same turn)
    battle.turn(battle.parseOneliner("x-x- 10dollar 5dollar pres hypno"));
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(battle.getCogset().getCog(i).getLured(), i % 2 == 1 ? 4 : (i == 0 ? 8 : 7));
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), preHP[i]);
    }
}

TEST_F(BattleTest, LureKnockbackDamage) {
    vector<Cog> set{Cog("6.exe"), Cog("10"), Cog("9.exe"), Cog("12")};
    battle.setCogset(set);
    battle.setPresState(false);
    vector<int> postHP{39, 39, 52, 58};

    battle.turn(battle.parseOneliner("pres -x-x 10dollar 10dollar bigmagnet"));
    battle.turn(battle.parseOneliner("xxxx seltzer hose cream cream"));
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_FALSE(battle.getCogset().getCog(i).getLured());
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), postHP[i]);
    }
}

TEST_F(BattleTest, LureNoKnockbackDamage) {
    vector<Cog> set{Cog("6.exe"), Cog("10"), Cog("9.exe"), Cog("12")};
    battle.setCogset(set);
    battle.setPresState(false);
    vector<int> postHP{84, 132, 135, 158};

    battle.turn(battle.parseOneliner("x-xx anvil rake taser hypno"));
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(battle.getCogset().getCog(i).getLured(), i < 2 ? -4 : 0);
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), postHP[i]);
    }

    battle.turn(battle.parseOneliner("kazoo"));
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_FALSE(battle.getCogset().getCog(i).getLured());
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), postHP[i] - 4);
    }
}

TEST_F(BattleTest, LureKnockbackKill) {
    vector<Cog> set{Cog("6.exe"), Cog("10"), Cog("9.exe"), Cog("12")};
    battle.setCogset(set);
    battle.setPresState(false);

    battle.turn(battle.parseOneliner("pres -x-x 10dollar 10dollar bigmagnet"));
    battle.turn(battle.parseOneliner("xxxx hose cloud cake cake"));
    EXPECT_TRUE(battle.getCogset().allDead());
}

TEST_F(BattleTest, Sound) {
    battle.setCogset(vector<Cog>(4, 12));
    battle.setPresState(true);

    // damage check
    battle.turn(battle.parseOneliner("3 trunk aoogah"));
    EXPECT_EQ(battle.getCogset().getSize(), 4);
    EXPECT_EQ(battle.getCogset().getCog(0).getHP(), 12);
    // kill check
    battle.setCogset(vector<Cog>(4, 12));
    battle.turn(battle.parseOneliner("4 trunk"));
    EXPECT_TRUE(battle.getCogset().allDead());

    // high-low tests
    // 9.exes still live because not all pres
    vector<Cog> set(3, {9, true});
    set.push_back(Cog(15, true));
    battle.setCogset(set);
    battle.setPresState(false);
    battle.turn(battle.parseOneliner("fog 2 pres trunk"));
    EXPECT_EQ(battle.getCogset().getSize(), 4);
    EXPECT_EQ(battle.getCogset().getCog(0).getHP(), 9);
    // 9.exes still live because max lvl is not high enough
    set[3] = Cog(14, true);
    battle.setCogset(set);
    battle.setPresState(true);
    battle.turn(battle.parseOneliner("fog 2 trunk"));
    EXPECT_EQ(battle.getCogset().getSize(), 4);
    EXPECT_EQ(battle.getCogset().getCog(0).getHP(), 3);
    // 9.exes are destroyed
    set[3] = Cog(15, true);
    battle.setCogset(set);
    battle.turn(battle.parseOneliner("fog 2 trunk"));
    EXPECT_EQ(battle.getCogset().getSize(), 1);
    EXPECT_EQ(battle.getCogset().getCog(0).getHP(), 242);
}

TEST_F(BattleTest, SoundOther) {
    // sound + throw
    vector<Cog> set(3, 8);
    set.push_back(Cog(8, true));
    battle.setCogset(set);
    battle.setPresState(false);
    battle.turn(battle.parseOneliner("3 pres aoogah fruit right"));
    EXPECT_TRUE(battle.getCogset().allDead());
    // sound + drop
    set = vector<Cog>(3, 11);
    set.push_back(Cog(14, true));
    battle.setCogset(set);
    battle.setPresState(true);
    battle.turn(battle.parseOneliner("fog 2 trunk piano right"));
    EXPECT_TRUE(battle.getCogset().allDead());
}

TEST_F(BattleTest, Squirt) {
    battle.setCogset(vector<Cog>(4, 11));
    battle.setPresState(false);

    // x---
    // 4000
    battle.turn(battle.parseOneliner("cloud left"));
    EXPECT_EQ(battle.getCogset().getCog(0).getSoaked(), 4);
    EXPECT_EQ(battle.getCogset().getCog(0).getHP(), 76);
    for (size_t i = 1; i < battle.getCogset().getSize(); ++i) {
        EXPECT_FALSE(battle.getCogset().getCog(i).getSoaked());
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), 156);
    }

    // -,o,
    // 3222
    battle.turn(battle.parseOneliner("pres flower mid-right"));
    EXPECT_EQ(battle.getCogset().getCog(0).getSoaked(), 3);
    EXPECT_EQ(battle.getCogset().getCog(0).getHP(), 76);
    for (size_t i = 1; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(battle.getCogset().getCog(i).getSoaked(), 2);
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), 156 - (i == 2 ? 4 : 0));
    }

    // -,o,
    // 0444
    battle.setCogset(vector<Cog>(4, 11));
    battle.turn(battle.parseOneliner("pres hose mid-right"));
    EXPECT_FALSE(battle.getCogset().getCog(0).getSoaked());
    EXPECT_EQ(battle.getCogset().getCog(0).getHP(), 156);
    for (size_t i = 1; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(battle.getCogset().getCog(i).getSoaked(), 4);
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), 156 - (i == 2 ? 56 : 0));
    }

    // o,--
    // 2200
    battle.setCogset(vector<Cog>(4, 11));
    battle.turn(battle.parseOneliner("pres flower left"));
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
    battle.turn(battle.parseOneliner("pres flower right"));
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(battle.getCogset().getCog(i).getSoaked(), i < 2 ? 1 : 2);
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), 156 - (i % 3 == 0 ? 4 : 0));
    }

    // --x-
    // 0011
    battle.turn(battle.parseOneliner("flower mid-right"));
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(battle.getCogset().getCog(i).getSoaked(), i < 2 ? 0 : 1);
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), 156 - (i != 1 ? 4 : 0));
    }

    // ,o,-
    // 3330
    battle.turn(battle.parseOneliner("pres gun mid-left"));
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(battle.getCogset().getCog(i).getSoaked(), i == 3 ? 0 : 3);
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), 156 - (i != 1 ? 4 : 12));
    }

    // -x--
    // 2220
    battle.turn(battle.parseOneliner("flower mid-left"));
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(battle.getCogset().getCog(i).getSoaked(), i == 3 ? 0 : 2);
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), 156 - (i != 1 ? 4 : 16));
    }

    // -x--
    // 4440
    battle.turn(battle.parseOneliner("flower mid-left pres seltzer mid-left"));
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(battle.getCogset().getCog(i).getSoaked(), i == 3 ? 0 : 4);
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), 156 - (i != 1 ? 4 : 57));
    }
}

TEST_F(BattleTest, Zap) {
    battle.setCogset(vector<Cog>(4, 13));
    battle.setPresState(false);
    battle.turn(battle.parseOneliner("pres flower mid-left taser left"));
    int postSoakHP[4] = {210, 206, 210, 210};
    int taser[3] = {72, 54, 36};
    int taserP[3] = {72, 60, 48};
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(battle.getCogset().getCog(i).getSoaked(), i == 3 ? 0 : 2);
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), i == 3 ? 210 : postSoakHP[i] - taser[i]);
    }
    battle.turn(battle.parseOneliner("pres taser mid-right"));
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_EQ(battle.getCogset().getCog(i).getSoaked(), i == 3 ? 0 : 1);
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), i == 3 ? 210 : postSoakHP[i] - taser[i] - taserP[2-i]);
    }
    battle.turn(battle.parseOneliner("taser mid-left"));
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_FALSE(battle.getCogset().getCog(i).getSoaked());
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), i == 3 ? 210 : postSoakHP[i] - taser[i] - taserP[2-i] - taser[(4-i)%3]);
    }
    battle.turn(battle.parseOneliner("pres taser right"));
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_FALSE(battle.getCogset().getCog(i).getSoaked());
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), i == 3 ? 186 : postSoakHP[i] - taser[i] - taserP[2-i] - taser[(4-i)%3]);
    }
    battle.turn(battle.parseOneliner("electra"));
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_FALSE(battle.getCogset().getCog(i).getSoaked());
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), (i == 3 ? 186 : postSoakHP[i] - taser[i] - taserP[2-i] - taser[(4-i)%3]) - 70);
    }

    battle.setCogset(vector<Cog>(4, {13, true}));
    battle.turn(battle.parseOneliner("sid nat"));
    for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
        EXPECT_TRUE(battle.getCogset().getCog(i).getSoaked());
        EXPECT_EQ(battle.getCogset().getCog(i).getHP(), 315 - 56 - (85*3));
    }
}

TEST_F(BattleTest, ZapStandardCombos) {
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
        battle.turn(battle.parseOneliner(pair.first));
        for (size_t i = 0; i < battle.getCogset().getSize(); ++i) {
            EXPECT_TRUE(battle.getCogset().getCog(i).getSoaked());
            EXPECT_EQ(600 - battle.getCogset().getCog(i).getHP(), pair.second[i]);
        }
    }
}

TEST_F(BattleTest, InvalidMove) {
    vector<Cog> set{Cog("14"), Cog("14"), Cog("14"), Cog("14")};
    battle.setCogset(set);
    battle.setPresState(false);

    battle.turn(battle.parseOneliner("10dollar left -xxx banana banana banana"));
    vector<string> badcommands = {
        "banana left", // trap on lured
        "banana mid-left", // trap on trapped
        "10dollar left", // lure on lured
        "penny" // SOS trap when all are trapped/lured
    };
    for (string s : badcommands) {
        EXPECT_THROW(battle.parseOneliner(s), invalid_argument);
    }
    battle.turn(battle.parseOneliner("kazoo"));
    battle.turn(battle.parseOneliner("banana left"));
    // SOS trap when all are trapped
    EXPECT_THROW(battle.parseOneliner("penny"), invalid_argument);
    battle.turn(battle.parseOneliner("hypno"));
    battle.turn(battle.parseOneliner("hypno"));
    // group lure when all are lured
    EXPECT_THROW(battle.parseOneliner("hypno"), invalid_argument);
    // SOS trap when all are trapped
    EXPECT_THROW(battle.parseOneliner("penny"), invalid_argument);
}
