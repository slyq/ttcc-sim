# Global vars
CC       = g++
BIN_DIR  = bin
LIB_DIR  = lib
SRC_DIR  = src
CPPFLAGS = -Wall -g -I$(LIB_DIR)
# GTEST_LL = -I /usr/local/opt/gtest/include/ -l gtest -l gtest_main -pthread
TARGETS = $(BIN_DIR)/cog.o $(BIN_DIR)/cogset.o $(BIN_DIR)/gag_collection.o $(BIN_DIR)/battle.o

all: $(TARGETS) game
	-@echo "--- All Built! ---"

game: $(SRC_DIR)/game.cpp $(TARGETS) $(LIB_DIR)/colors.h $(LIB_DIR)/rang.h
	$(CC) $(CPPFLAGS) $^ -o $@

$(BIN_DIR)/cog.o: $(SRC_DIR)/cog.cpp $(LIB_DIR)/cog.h $(LIB_DIR)/rang.h $(BIN_DIR)/.dirstamp
	$(CC) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR)/cogset.o: $(SRC_DIR)/cogset.cpp $(LIB_DIR)/cog.h $(LIB_DIR)/cogset.h $(LIB_DIR)/rang.h $(BIN_DIR)/.dirstamp
	$(CC) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR)/gag_collection.o: $(SRC_DIR)/gag_collection.cpp $(LIB_DIR)/gag_collection.h $(BIN_DIR)/.dirstamp
	$(CC) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR)/battle.o: $(SRC_DIR)/battle.cpp $(LIB_DIR)/battle.h $(LIB_DIR)/cog.h $(LIB_DIR)/cogset.h $(LIB_DIR)/gag_collection.h $(LIB_DIR)/rang.h $(BIN_DIR)/.dirstamp
	$(CC) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR)/.dirstamp:
	-@mkdir -p $(BIN_DIR)
	-@touch $@

.PHONY: clean tests

tests: clean game
	-@echo "--- Now Testing ---"
	-@./game
	-@echo "--- All Tested! ---"

clean:
	-@rm -rf $(BIN_DIR)
	-@rm game
	-@echo "--- All Clean! ---"
