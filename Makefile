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

CDCL: $(OBJ_DIR)/main.o $(OBJ_DIR)/Parser.o $(OBJ_DIR)/representation.o
	g++ $(CFLAGS) $(DEFINES) -o $(OUT_DIR)/CDCL $^

$(OBJ_DIR)/main.o: $(SRC_DIR)/*.cpp
	g++ $(CFLAGS) $(DEFINES) -o $@ -c $(SRC_DIR)/main.cpp

$(OBJ_DIR)/Parser.o: $(SRC_DIR)/Parser.cpp $(SRC_DIR)/representation.cpp
	g++ $(CFLAGS) $(DEFINES) -o $@ -c $(SRC_DIR)/Parser.cpp

$(OBJ_DIR)/representation.o: $(SRC_DIR)/representation.cpp
	g++ $(CFLAGS) $(DEFINES) -o $@ -c $(SRC_DIR)/representation.cpp

clean:
	rm -rf *~ out

directories:
	mkdir -p $(OBJ_DIR)

clean_debug:
	rm -rf out/debug

clean_release:
	rm -rf out/release