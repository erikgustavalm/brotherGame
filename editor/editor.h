#ifndef _EDITOR_H_
#define _EDITOR_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TILE_SIZE 32
#define TILE_CROP_SIZE 16

struct Tile
{
  SDL_Rect src;
  SDL_Rect crop;
  int isSolid;
};

struct Tile* tileArray;
int tileArrayUsed;
int tileArraySize;

SDL_Rect activeDestRect;
SDL_Rect activeCropRect;

SDL_Renderer* gRender;
SDL_Window* gWindow;

SDL_Texture* staticSprite;
SDL_Rect staticSpriteRect;

SDL_Texture* editorSprite;
SDL_Rect solidEditorRect;

extern int checkIfTaken(int x, int y);
extern void createTile(struct Tile* new, int x, int y, int cx, int cy, int solid);
extern void addTile(struct Tile new);
extern void render();
extern void update();
extern void event();
extern void mainloop();
extern void loadLevelType(char* levelType);
extern void loadLevelFile(char* levelFile);
extern void startEditor(char* levelFile, char* type);
extern void initEditor();
extern void quit();
extern void setActiveRect(int x, int y);
extern void saveToFile();
extern void movement(int x, int y);

int corr_x;
int corr_y;

int running;
int showStatic;
int makeSolid;

int mouse_x;
int mouse_y;

int camUp;
int camDown;
int camLeft;
int camRight;

char fpath[50];
#endif
