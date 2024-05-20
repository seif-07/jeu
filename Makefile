CC = gcc
CFLAGS = -Wall -Wextra -g -Wno-unused-parameter -Wno-missing-field-initializers
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

SRC = main.c run.c game.c perso.c enemy.c audio.c graphics.c score.c utils.c
OBJ = $(SRC:.c=.o)
EXECUTABLE = prog

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(EXECUTABLE) $(OBJ)

