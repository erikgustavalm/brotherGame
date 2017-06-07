#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <stdlib.h>

// Define a macro for screen size, both width and height
#define SCREEN_SIZE 640


// Gamestate is to be used as identifier for which mode the game will be in
// and what to do thereafter
typedef enum{ MENU, QUIT, PLAY} gamestate_t;
gamestate_t gamestate;

// gameRender is the rendering structure/engine that will be used whenever
//an object needs to be drawn to screen
SDL_Renderer* gameRender;

// gameWindow is simply the instance of a window, only used in init and exit
SDL_Window* gameWindow;


// First function to be called and only initalize everything else
extern void initSetup();

// The body of the whole, inside everything happends
extern void gameloop();

// A function for a nice cleanup of resources
extern void quit();

// Where logical operations is called from
extern void updateLogic();

// Checking for all events that might be happening
extern void eventCheck();

// The rendering of object to the screen
extern void drawScreen();
