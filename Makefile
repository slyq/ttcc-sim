# Global vars`
CC       = g++
BIN_DIR  = bin
LIB_DIR  = lib
SRC_DIR  = src
CPPFLAGS = -Wall -g -I$(LIB_DIR)
# GTEST_LL = -I /usr/local/opt/gtest/include/ -l gtest -l gtest_main -pthread
TARGETS = $(BIN_DIR)/cog.o $(BIN_DIR)/cogset.o

all: $(TARGETS) game
	-@echo "--- All Built! ---"

game: $(SRC_DIR)/game.cpp $(TARGETS)
	$(CC) $(CPPFLAGS) $^ -o $@

$(BIN_DIR)/cog.o: $(SRC_DIR)/cog.cpp $(LIB_DIR)/cog.h $(BIN_DIR)/.dirstamp
	$(CC) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR)/cogset.o: $(SRC_DIR)/cogset.cpp $(LIB_DIR)/cog.h $(LIB_DIR)/cogset.h $(BIN_DIR)/.dirstamp
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
