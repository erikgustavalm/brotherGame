#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <stdlib.h>

#define SCREEN_SIZE 640


typedef enum{ MENU, QUIT, PLAY} gamestate_t;

SDL_Renderer* gameRender;
SDL_Window* gameWindow;

gamestate_t gamestate;

extern void initSetup();
extern void gameloop();
extern void quit();
extern void eventCheck();
extern void drawScreen();
