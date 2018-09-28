.PHONY: install clean

NAME       := ikea-tradfri-gateway
BIN_DIR    := bin
BINARY     := $(BIN_DIR)/$(NAME)
INST_DIR   := /usr/bin/$(NAME)

SRC_DIR    := src
SRC_FILES  := $(wildcard $(SRC_DIR)/*.cpp)
CXXFLAGS   := --std=c++17 -DWITH_POSIX
CXXINC     := 

OBJ_DIR    := obj
OBJ_FILES  := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
LDFLAGS    := 
LDLIBS     := -pthread -lrt /usr/lib/libcoap-1.a
	
$(BINARY): $(OBJ_FILES) $(BIN_DIR)
	g++ $(LDFLAGS) -o $@ $(OBJ_FILES) $(LDLIBS)
	
$(BIN_DIR):
	mkdir -p $@

install: $(BINARY)
	cp -v $(BINARY) $(INST_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(OBJ_DIR)
	g++ $(CXXFLAGS) $(CXXINC) -c -o $@ $<

$(OBJ_DIR):
	mkdir -p $@
	
clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)