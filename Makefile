CC := gcc

CFLAGS := -ggdb3 -O0 -std=c99 -Wall $(shell sdl2-config --cflags)
LDFLAGS := $(shell sdl2-config --libs) -lSDL2_image -lm

SRCS := src/main.c src/vec2.c src/app.c src/player.c src/map.c
OBJS := $(SRCS:.c=.o)
EXEC := exe

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) -o $(EXEC) $(OBJS) $(LDFLAGS)

clean:
	rm -f $(EXEC) $(OBJS)

.PHONY: all clean
