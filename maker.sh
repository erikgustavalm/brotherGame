

CFILES="main.c setup.c"
HFILES="setup.h"

gcc -std=c99 $CFILES $HFILES -w -lSDL2 -lSDL2_image -o game
