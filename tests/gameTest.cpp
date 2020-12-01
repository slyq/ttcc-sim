#include "gtest/gtest.h"
#include <iostream>
#include <string>
#include <algorithm>

#include "battle.h"
#include "cog.h"
#include "cogset.h"
#include "gag_collection.h"

#define GAGDICT_PATH "gags.txt"

using namespace std;

class GagCollectionTest : public testing::Test {
protected:
	GagCollectionTest() {}
	virtual ~GagCollectionTest() {}
	GagCollection gc = GagCollection::read(GAGDICT_PATH);
	testing::AssertionResult verifyGag(const Gag& g, GagKind gk, string name, unsigned short damage, int accuracy);
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

class BattleTest : public testing::Test {
protected:
	BattleTest() {
		battle.setPresState(true);
		battle.setPrint(false);
	}
	virtual ~BattleTest() {}
	GagCollection gc = GagCollection::read(GAGDICT_PATH);
	Battle battle;
};

TEST_F(BattleTest, TrapDrop) {
	vector<Cog> set{Cog("14.exe"), Cog("22"), Cog("18"), Cog("19.exe")};
	battle.setCogset(set);
	battle.turn(battle.parseOneliner("xx-- wreckingball wreckingball 50dollar left 10dollar mid-left safe left safe mid-left weight mid-left"));
	EXPECT_EQ(battle.getCogset().getSize(), 2);
	battle.setPresState(false);
	battle.turn(battle.parseOneliner("xx wreckingball tnt hypno xX boulder weight piano"));
	EXPECT_TRUE(battle.cogsetDead());
}

TEST_F(BattleTest, Sound) {
	battle.setCogset(vector<Cog>(4, 12));
	battle.turn(battle.parseOneliner("3 trunk aoogah"));
	EXPECT_EQ(battle.getCogset().getSize(), 4);
	EXPECT_EQ(battle.getCogset().getCog(0).getHP(), 12);
	battle.setCogset(vector<Cog>(4, 12));
	battle.turn(battle.parseOneliner("4 trunk"));
	EXPECT_TRUE(battle.cogsetDead());

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
	EXPECT_TRUE(battle.cogsetDead());
	// sound + drop
	set = vector<Cog>(3, 11);
	set.push_back(Cog(14, true));
	battle.setCogset(set);
	battle.setPresState(true);
	battle.turn(battle.parseOneliner("fog 2 trunk piano right"));
	EXPECT_TRUE(battle.cogsetDead());
}
