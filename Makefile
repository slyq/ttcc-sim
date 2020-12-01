# Global vars
CC       = g++
BIN_DIR  = bin
LIB_DIR  = lib
SRC_DIR  = src
TEST_DIR = tests
CPPFLAGS = -Wall -g -I$(LIB_DIR)
GTEST_LL = -I /usr/include/gtest/ -l gtest -l gtest_main -pthread

all: $(BIN_DIR)/gameTest $(BIN_DIR)/game
	-@echo "--- All Built! ---"

tests: $(BIN_DIR)/.dirstamp $(BIN_DIR)/gameTest
	# valgrind --leak-check=yes --track-origins=yes ./$(BIN_DIR)/gameTest
	./$(BIN_DIR)/gameTest
	-@echo "--- All Passed! ---"

game: $(BIN_DIR)/.dirstamp $(BIN_DIR)/game
	-@./$(BIN_DIR)/game

$(BIN_DIR)/gameTest: $(BIN_DIR)/gameTest.o $(BIN_DIR)/cog.o $(BIN_DIR)/cogset.o $(BIN_DIR)/gag_collection.o $(BIN_DIR)/battle.o
	$(CC) $(CPPFLAGS) $^ $(GTEST_LL) -o $@

$(BIN_DIR)/gameTest.o: $(TEST_DIR)/gameTest.cpp $(LIB_DIR)/battle.h $(LIB_DIR)/cog.h $(LIB_DIR)/cogset.h $(LIB_DIR)/gag_collection.h $(LIB_DIR)/colors.h $(LIB_DIR)/rang.h $(BIN_DIR)/.dirstamp
	$(CC) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR)/game: $(BIN_DIR)/game.o $(BIN_DIR)/cog.o $(BIN_DIR)/cogset.o $(BIN_DIR)/gag_collection.o $(BIN_DIR)/battle.o
	$(CC) $(CPPFLAGS) $^ -o $@

$(BIN_DIR)/game.o: $(SRC_DIR)/game.cpp $(LIB_DIR)/battle.h $(LIB_DIR)/cog.h $(LIB_DIR)/cogset.h $(LIB_DIR)/gag_collection.h $(LIB_DIR)/colors.h $(LIB_DIR)/rang.h $(BIN_DIR)/.dirstamp
	$(CC) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR)/cog.o: $(SRC_DIR)/cog.cpp $(LIB_DIR)/cog.h $(LIB_DIR)/colors.h $(LIB_DIR)/rang.h $(BIN_DIR)/.dirstamp
	$(CC) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR)/cogset.o: $(SRC_DIR)/cogset.cpp $(LIB_DIR)/cog.h $(LIB_DIR)/cogset.h $(LIB_DIR)/colors.h $(LIB_DIR)/rang.h $(BIN_DIR)/.dirstamp
	$(CC) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR)/gag_collection.o: $(SRC_DIR)/gag_collection.cpp $(LIB_DIR)/gag_collection.h $(BIN_DIR)/.dirstamp
	$(CC) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR)/battle.o: $(SRC_DIR)/battle.cpp $(LIB_DIR)/battle.h $(LIB_DIR)/cog.h $(LIB_DIR)/cogset.h $(LIB_DIR)/gag_collection.h $(LIB_DIR)/colors.h $(LIB_DIR)/rang.h $(BIN_DIR)/.dirstamp
	$(CC) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR)/.dirstamp:
	-@mkdir -p $(BIN_DIR)
	-@touch $@

.PHONY: clean tests

clean:
	-@rm -rf $(BIN_DIR)
	-@echo "--- All Clean! ---"
