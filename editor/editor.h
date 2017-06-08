#ifndef _EDITOR_H_
#define _EDITOR_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SDL_Renderer* gRender;
SDL_Window* gWindow;

SDL_Texture* staticSprite;

extern void render();
extern void update();
extern void event();
extern void mainloop();
extern void loadLevelType(char* levelType);
extern void loadLevelFile(char* levelFile);
extern void startEditor(char* levelFile, char* type);
extern void initEditor();
extern void quit();

int running;
#endif
