

CFILES="main.c setup.c objects.c"
HFILES="setup.h objects.h"

gcc -std=c99 $CFILES $HFILES -w -lSDL2 -lSDL2_image -o game
