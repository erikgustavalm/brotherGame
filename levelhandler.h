#ifndef _LEVELHANDLER_H_
#define _LEVELHANDLER_H_

#include "objects.h"

#include <SDL2/SDL.h>

SDL_Texture* level_image;


extern void loadLevel(int level);

char* filepathToLevelTexture;

#endif
