SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=obj/%.o)
SHARED = $(SRC:src/%.c=shared/%.dll)
INC = -Iinclude

CC := x86_64-w64-mingw32-gcc ## Modify this line to match your compiler if necessary

$(OBJ): obj/%.o: src/%.c
	$(CC) -c $< -o $@ $(INC)

$(SHARED): shared/%.dll: obj/%.o
	$(CC) -shared -Os $< -o $@ $(INC)

build: $(OBJ) 

all: build $(SHARED)

setup: 
	@sudo apt-get update
	@sudo apt-get upgrade
	@sudo apt-get install -y mingw-w64

clean:
	rm -rf obj /* shared/*

.PHONY: clean setup all build
