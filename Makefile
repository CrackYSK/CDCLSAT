CFLAGS = -Wall -std=c++11
SRC_DIR = src
DEFINES =

ifeq ($(debug), 1)
	CFLAGS += -g
	DEFINES = -DDEBUG
	OUT_DIR = out/debug
else
	OUT_DIR = out/release
endif

OBJ_DIR = $(OUT_DIR)/obj

.PHONY: all clean directories clean_debug clean_release

all: directories CDCL

CDCL: main.o Parser.o representation.o Valuation.o
	g++ -o $(OUT_DIR)/CDCL $(addprefix $(OBJ_DIR)/, $^)

main.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/*.h
	g++ $(CFLAGS) $(DEFINES) -o $(OBJ_DIR)/$@ -c $(SRC_DIR)/main.cpp

Valuation.o: $(SRC_DIR)/Valuation.* $(SRC_DIR)/representation.h
	g++ $(CFLAGS) $(DEFINES) -o $(OBJ_DIR)/$@ -c $(SRC_DIR)/Valuation.cpp

representation.o: $(SRC_DIR)/representation.*
	g++ $(CFLAGS) $(DEFINES) -o $(OBJ_DIR)/$@ -c $(SRC_DIR)/representation.cpp

Parser.o: $(SRC_DIR)/Parser.* $(SRC_DIR)/representation.h
	g++ $(CFLAGS) $(DEFINES) -o $(OBJ_DIR)/$@ -c $(SRC_DIR)/Parser.cpp

clean:
	rm -rf *~ out

directories:
	mkdir -p $(OBJ_DIR)

clean_debug:
	rm -rf out/debug

clean_release:
	rm -rf out/release