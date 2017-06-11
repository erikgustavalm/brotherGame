#include "editor.h"

void event()
{
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
      case SDL_QUIT: 
        running = 0;
      break;
	
      case SDL_KEYDOWN:
        switch(e.key.keysym.sym){
	  case SDLK_ESCAPE:
	    running = 0;
	  break;
	  case SDLK_s:
	    if (showStatic == 1) {
	      showStatic = 0;
	    }else {
	      showStatic = 1;
	    }
	  break;
	  default:
	  break;
	}
      break;
	  
    default:
    break;
    }
  }
}
void update()
{

}
void render()
{
  SDL_RenderClear(gRender);


  for (int i = 0; i < tileArraySize; i++) {
    SDL_RenderCopy(gRender, staticSprite, &tileArray[i].crop, &tileArray[i].src);
  }
  if (showStatic == 1) {
    SDL_RenderCopy(gRender, staticSprite, NULL, &staticSpriteRect);
  }
  
  SDL_RenderPresent(gRender);
}
void mainloop()
{
  SDL_SetRenderDrawColor(gRender, 255,255,255,255);
  running = 1;
  while (running) {
    event();
    update();
    render();
  }
}

void initEditor()
{
  gWindow = SDL_CreateWindow("EDITOR",
			     SDL_WINDOWPOS_CENTERED,
			     SDL_WINDOWPOS_CENTERED,
			     420,420,SDL_WINDOW_SHOWN);
  if (gWindow == NULL) {
    printf("gWindow not created %s\n", SDL_GetError());
  } else {
    gRender = SDL_CreateRenderer(gWindow,-1, SDL_RENDERER_ACCELERATED);
    if (gRender == NULL) {
      printf("couldn't create render: %s\n", SDL_GetError());
    }else {
    }
  }
}

void createTile(struct Tile* new, int x, int y, int cx, int cy){
  new->src.x = x;
  new->src.y = y;
  new->src.w = TILE_SIZE;
  new->src.h = TILE_SIZE;

  new->crop.x = cx;
  new->crop.y = cy;
  new->crop.w = TILE_CROP_SIZE;
  new->crop.h = TILE_CROP_SIZE;
}

void loadLevelFile(char* levelFile)
{
  if (strlen(levelFile) > 50) {
    printf("Not a correct level\n");
    exit(1);
  }
  char pre[50];
  strcpy(pre, "levels/");
  
  strcat(pre, levelFile);
  
  FILE* f = fopen(pre, "a+");
  if (f == NULL) {
    printf("file not loaded: %s\n", pre);
  }else {
    printf("file loaded succesfully\n");

    int linesInFile = 2;

    /*
    while (!feof(f)) {
      char s[30];
      if (fscanf(f, "%s\n", &s)) {
	
      }
      linesInFile++;
      }*/
    
    //rewind(f);

    tileArraySize = linesInFile +1;
    
    tileArray = NULL;
    tileArray = realloc(tileArray, sizeof(struct Tile) * tileArraySize);
    
    if (tileArray == NULL) {
      printf("mem not allocated\n");
    }else {
      printf("mem allocated\n");
    }

    int index = 0;
    
    while (!feof(f)) {
      
      int srcx;
      int srcy;
      int crox;
      int croy;
      
      if (fscanf(f, "%d %d %d %d\n", &crox, &croy, &srcx, &srcy)) {
	struct Tile new;
	createTile(&new, srcx, srcy, crox, croy);
	tileArray[index] = new;
	index++;
      }else {
	printf("not working\n");
      }
    }
  }
  
  fclose(f);
}

void loadLevelType(char* levelType)
{
  char* ssFilepath;

  if (strcmp(ssFilepath, "wood")) {
    ssFilepath = "levels/wood.png";

  }

  printf("static spritesheet is loaded from: %s\n", ssFilepath);
  
  SDL_Surface* loadSurf = IMG_Load(ssFilepath);

  if (loadSurf == NULL) {
    printf("%s\n", SDL_GetError());
  }
  staticSprite = SDL_CreateTextureFromSurface(gRender, loadSurf);

  SDL_FreeSurface(loadSurf);

  if (staticSprite == NULL) {
    printf("%s\n", SDL_GetError());
  }

  staticSpriteRect.x = 0;
  staticSpriteRect.y = 0;
  staticSpriteRect.w = 64;
  staticSpriteRect.h = 64;
  
}

void startEditor(char* levelFile, char* type)
{
  SDL_Init(SDL_INIT_EVERYTHING);

  loadLevelFile(levelFile);
  
  initEditor();
  
  loadLevelType(type);
  
  mainloop();

  quit();
}

void quit()
{
  free(tileArray);
  tileArray = NULL;
  
  SDL_DestroyTexture(staticSprite);
  staticSprite = NULL;
  SDL_DestroyRenderer(gRender);
  gRender = NULL;
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;

  SDL_Quit();

  printf("byebye\n");
}
