CC = gcc
CFLAGS = -Wall -Wextra
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
EXEC = clash_royale

SDL_LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_gfx

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@ $(SDL_LIBS)

clean:
	del /Q src\\*.o $(EXEC).exe 2>nul || exit 0
