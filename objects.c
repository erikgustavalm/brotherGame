#include "objects.h"

SDL_Texture* loadTexture(char* filepath, SDL_Renderer* refRender)
{
  SDL_Texture* newText = NULL;

  SDL_Surface* loadSurf = IMG_Load(filepath);

  newText = SDL_CreateTextureFromSurface(refRender, loadSurf);
  
  if (newText == NULL) {
    printf("Unable to load text: %s\n", SDL_GetError());
  }else {
    printf("texture loaded: %s\n", filepath);
  }
  
  SDL_FreeSurface(loadSurf);

  return newText;
}
