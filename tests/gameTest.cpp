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
    CogTest() {}
    virtual ~CogTest() {}
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

TEST_F(CogTest, VerifyCog) {
    vector<Cog> cogs;
    cogs.push_back(Cog());
    cogs.push_back(Cog(1));
    cogs.push_back(Cog(1, true));
    cogs.push_back(Cog(12));
    cogs.push_back(Cog(18, true));
    EXPECT_TRUE(verifyCog(cogs[0], "", 0));
    EXPECT_TRUE(verifyCog(cogs[1], "1", 6));
    EXPECT_TRUE(verifyCog(cogs[2], "1.exe", 9));
    EXPECT_TRUE(verifyCog(cogs[3], "12", 182));
    EXPECT_TRUE(verifyCog(cogs[4], "18.exe", 570));

    for (Cog& c : cogs) {
        c.hit(50);
    }
    EXPECT_TRUE(verifyCog(cogs[0], "", 0));
    EXPECT_TRUE(verifyCog(cogs[1], "1", 0));
    EXPECT_TRUE(verifyCog(cogs[2], "1.exe", 0));
    EXPECT_TRUE(verifyCog(cogs[3], "12", 132));
    EXPECT_TRUE(verifyCog(cogs[4], "18.exe", 520));
}

class ParseTest : public testing::Test {
protected:
    ParseTest() {}
    virtual ~ParseTest() {}
    Battle battle;
    bool contains(Battle::Strategy s, const Gag& gag, size_t count);
    Battle::Strategy buildStrat(vector<string> s);
};

bool ParseTest::contains(Battle::Strategy s, const Gag& gag, size_t count) {
    size_t found = 0;
    for (const Gag& g : s.gags) {
        if (((gag.kind == GagKind::LURE && g.accuracy == gag.accuracy)
             || (gag.kind != GagKind::LURE && g.damage == gag.damage))
            && g.kind == gag.kind && g.damage == gag.damage && g.target == gag.target && g.prestiged == gag.prestiged) {
            ++found;
        }
    }
    return found == count;
}

Battle::Strategy ParseTest::buildStrat(vector<string> s) {
    vector<Gag> gags;
    for (string gag : s) {
        gags.push_back(battle.parseGag(gag));
    }
    return Battle::Strategy(gags, 2);
}

TEST_F(ParseTest, OneLineNoPres) {
    vector<Cog> set{Cog("18.exe"), Cog("17"), Cog("18"), Cog("18.exe")};
    battle.setCogset(set);
    Battle::Strategy s = battle.parseOneliner("trunk");
    EXPECT_EQ(s.gags.size(), 1);
    EXPECT_TRUE(contains(s, Gag(GagKind::SOUND, 32, -1, false), 1));

    s = battle.parseOneliner("marbles left hypno");
    EXPECT_EQ(s.gags.size(), 2);
    EXPECT_FALSE(contains(s, Gag(GagKind::SOUND, 32, -1, false), 1));
    EXPECT_TRUE(contains(s, Gag(GagKind::TRAP, 65, 0, false), 1));
    Gag hypno = Gag(GagKind::LURE, 0, -1, false);
    hypno.accuracy = 70;
    EXPECT_TRUE(contains(s, hypno, 1));

    string commandtest[4]
            = {"wballoon left taser mid-left cream mid-right weight right",
               "wballoon !0 taser !1 cream !2 weight !3",
               "wballoon 18.exe taser 17 cream 18 weight right",
               "xxxx wballoon taser cream weight"};
    for (string command : commandtest) {
        s = battle.parseOneliner(command);
        EXPECT_EQ(s.gags.size(), 4);
        EXPECT_TRUE(contains(s, Gag(GagKind::SQUIRT, 21, 0, false), 1));
        EXPECT_TRUE(contains(s, Gag(GagKind::ZAP, 24, 1, false), 1));
        EXPECT_TRUE(contains(s, Gag(GagKind::THROW, 75, 2, false), 1));
        EXPECT_TRUE(contains(s, Gag(GagKind::DROP, 80, 3, false), 1));
    }

    s = battle.parseOneliner("-xx- cross tv tv -X-- seltzer seltzer");
    EXPECT_EQ(s.gags.size(), 4);
    EXPECT_EQ(s.config, 1);
    EXPECT_TRUE(contains(s, Gag(GagKind::SQUIRT, 30, 1, false), 2));
    EXPECT_TRUE(contains(s, Gag(GagKind::ZAP, 40, 1, false), 1));
    EXPECT_TRUE(contains(s, Gag(GagKind::ZAP, 40, 2, false), 1));

    s = battle.parseOneliner("FIRE left");
    EXPECT_EQ(s.gags.size(), 1);
    EXPECT_TRUE(contains(s, Gag(GagKind::FIRE, 0, 0, false), 1));
}

TEST_F(ParseTest, OneLinePres) {
    vector<Cog> set{Cog("18.exe"), Cog("17"), Cog("18"), Cog("18.exe")};
    battle.setCogset(set);
    Battle::Strategy s = battle.parseOneliner("pres trunk");
    EXPECT_EQ(s.gags.size(), 1);
    EXPECT_TRUE(contains(s, Gag(GagKind::SOUND, 32, -1, true), 1));

    s = battle.parseOneliner("pres marbles left hypno");
    EXPECT_EQ(s.gags.size(), 2);
    EXPECT_FALSE(contains(s, Gag(GagKind::SOUND, 32, -1, false), 1));
    EXPECT_TRUE(contains(s, Gag(GagKind::TRAP, 65, 0, true), 1));
    Gag hypno = Gag(GagKind::LURE, 0, -1, false);
    hypno.accuracy = 70;
    EXPECT_TRUE(contains(s, hypno, 1));

    string commandtest[4]
            = {"pres wballoon left taser mid-left pres cream mid-right weight right",
               "pres wballoon !0 taser !1 pres cream !2 weight !3",
               "pres wballoon 18.exe taser 17 pres cream 18 weight right",
               "xxxx pres wballoon taser pres cream weight"};
    for (string command : commandtest) {
        s = battle.parseOneliner(command);
        EXPECT_EQ(s.gags.size(), 4);
        EXPECT_TRUE(contains(s, Gag(GagKind::SQUIRT, 21, 0, true), 1));
        EXPECT_TRUE(contains(s, Gag(GagKind::ZAP, 24, 1, false), 1));
        EXPECT_TRUE(contains(s, Gag(GagKind::THROW, 75, 2, true), 1));
        EXPECT_TRUE(contains(s, Gag(GagKind::DROP, 80, 3, false), 1));
    }

    s = battle.parseOneliner("-xx- cross pres tv tv pres -X-- seltzer seltzer");
    EXPECT_EQ(s.gags.size(), 4);
    EXPECT_EQ(s.config, 1);
    EXPECT_TRUE(contains(s, Gag(GagKind::SQUIRT, 30, 1, true), 2));
    EXPECT_TRUE(contains(s, Gag(GagKind::ZAP, 40, 1, true), 1));
    EXPECT_TRUE(contains(s, Gag(GagKind::ZAP, 40, 2, false), 1));
}

TEST_F(ParseTest, MultLineNoPres) {
    vector<Cog> set{Cog("18.exe"), Cog("17"), Cog("18"), Cog("18.exe")};
    battle.setCogset(set);
    vector<string> commands{"trunk", "trunk", "trunk", "trunk"};
    Battle::Strategy s = buildStrat(commands);
    EXPECT_EQ(s.gags.size(), 4);
    EXPECT_EQ(s.config, 2);
    EXPECT_TRUE(contains(s, Gag(GagKind::SOUND, 32, -1, false), 4));

    commands = {"marbles left", "hypno", "PASS", "PASS"};
    s = buildStrat(commands);
    EXPECT_EQ(s.gags.size(), 4);
    EXPECT_EQ(s.config, 2);
    EXPECT_FALSE(contains(s, Gag(GagKind::SOUND, 32, -1, false), 1));
    EXPECT_TRUE(contains(s, Gag(GagKind::TRAP, 65, 0, false), 1));
    Gag hypno = Gag(GagKind::LURE, 0, -1, false);
    hypno.accuracy = 70;
    EXPECT_TRUE(contains(s, hypno, 1));

    vector<vector<string>> commandtest
            = {{"wballoon left", "taser mid-left", "cream mid-right", "weight right"},
               {"wballoon !0", "taser !1", "cream !2", "weight !3"},
               {"wballoon 18.exe", "taser 17", "cream 18", "weight right"}};
    for (auto command : commandtest) {
        s = buildStrat(command);
        EXPECT_EQ(s.gags.size(), 4);
        EXPECT_EQ(s.config, 2);
        EXPECT_TRUE(contains(s, Gag(GagKind::SQUIRT, 21, 0, false), 1));
        EXPECT_TRUE(contains(s, Gag(GagKind::ZAP, 24, 1, false), 1));
        EXPECT_TRUE(contains(s, Gag(GagKind::THROW, 75, 2, false), 1));
        EXPECT_TRUE(contains(s, Gag(GagKind::DROP, 80, 3, false), 1));
    }

    commands = {"tv mid-right", "tv mid-left", "seltzer mid-left", "seltzer mid-left"};
    s = buildStrat(commands);
    EXPECT_EQ(s.gags.size(), 4);
    EXPECT_EQ(s.config, 2);
    EXPECT_TRUE(contains(s, Gag(GagKind::SQUIRT, 30, 1, false), 2));
    EXPECT_TRUE(contains(s, Gag(GagKind::ZAP, 40, 1, false), 1));
    EXPECT_TRUE(contains(s, Gag(GagKind::ZAP, 40, 2, false), 1));

    commands = {"FIRE left", "PASS", "PASS", "PASS"};
    s = buildStrat(commands);
    EXPECT_EQ(s.gags.size(), 4);
    EXPECT_EQ(s.config, 2);
    EXPECT_TRUE(contains(s, Gag(GagKind::FIRE, 0, 0, false), 1));
}

TEST_F(ParseTest, MultLinePres) {
    vector<Cog> set{Cog("18.exe"), Cog("17"), Cog("18"), Cog("18.exe")};
    battle.setCogset(set);
    vector<string> commands{"pres trunk", "trunk", "trunk", "pres trunk"};
    Battle::Strategy s = buildStrat(commands);
    EXPECT_EQ(s.gags.size(), 4);
    EXPECT_EQ(s.config, 2);
    EXPECT_TRUE(contains(s, Gag(GagKind::SOUND, 32, -1, true), 2));
    EXPECT_TRUE(contains(s, Gag(GagKind::SOUND, 32, -1, false), 2));

    commands = {"pres marbles left", "pres hypno", "PASS", "PASS"};
    s = buildStrat(commands);
    EXPECT_EQ(s.gags.size(), 4);
    EXPECT_EQ(s.config, 2);
    EXPECT_FALSE(contains(s, Gag(GagKind::SOUND, 32, -1, false), 1));
    EXPECT_TRUE(contains(s, Gag(GagKind::TRAP, 65, 0, true), 1));
    Gag hypno = Gag(GagKind::LURE, 0, -1, true);
    hypno.accuracy = 70;
    EXPECT_TRUE(contains(s, hypno, 1));

    vector<vector<string>> commandtest
            = {{"pres wballoon left", "taser mid-left", "pres cream mid-right", "weight right"},
               {"pres wballoon !0", "taser !1", "pres cream !2", "weight !3"},
               {"pres wballoon 18.exe", "taser 17", "pres cream 18", "weight right"}};
    for (auto command : commandtest) {
        s = buildStrat(command);
        EXPECT_EQ(s.gags.size(), 4);
        EXPECT_EQ(s.config, 2);
        EXPECT_TRUE(contains(s, Gag(GagKind::SQUIRT, 21, 0, true), 1));
        EXPECT_TRUE(contains(s, Gag(GagKind::ZAP, 24, 1, false), 1));
        EXPECT_TRUE(contains(s, Gag(GagKind::THROW, 75, 2, true), 1));
        EXPECT_TRUE(contains(s, Gag(GagKind::DROP, 80, 3, false), 1));
    }

    commands = {"tv mid-right", "pres tv mid-left", "pres seltzer mid-left", "pres seltzer mid-left"};
    s = buildStrat(commands);
    EXPECT_EQ(s.gags.size(), 4);
    EXPECT_EQ(s.config, 2);
    EXPECT_TRUE(contains(s, Gag(GagKind::SQUIRT, 30, 1, true), 2));
    EXPECT_TRUE(contains(s, Gag(GagKind::ZAP, 40, 1, true), 1));
    EXPECT_TRUE(contains(s, Gag(GagKind::ZAP, 40, 2, false), 1));
}

class BattleTest : public testing::Test {
protected:
    BattleTest() {
        battle.setHitState(true);
        battle.setPrint(false);
        battle.setUpdate(true);
    }
    virtual ~BattleTest() {}
    Battle battle;
};

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
            EXPECT_EQ(battle.getCogset().getCog(i).getLured(), 0);
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
        EXPECT_EQ(battle.getCogset().getCog(i).getLured(), 0);
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
        EXPECT_EQ(battle.getCogset().getCog(i).getLured(), 0);
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
}
