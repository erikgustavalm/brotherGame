

CFILES="main.c setup.c objects.c levelhandler.c"
HFILES="setup.h objects.h levelhandler.h"

gcc -std=c99 $CFILES $HFILES -w -lSDL2 -lSDL2_image -o game
