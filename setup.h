#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <stdlib.h>

SDL_Renderer* gameRender;
SDL_Window* gameWindow;

int running;

extern void initSetup();
extern void gameloop();
extern void quit();

