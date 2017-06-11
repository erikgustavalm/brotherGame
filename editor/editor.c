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

	  case SDLK_SPACE:
	    if (showStatic == 1) {
	      showStatic = 0;
	    }else {
	      showStatic = 1;
	    }
	  break;

	  case SDLK_RETURN:
	    saveToFile();
	    running = 0;
	  break;

 	  case SDLK_w:
	    camUp = 1;
	  break;

	  case SDLK_s:
	    camDown = 1;
	  break;

	  case SDLK_a:
	    camLeft = 1;
	  break;

	  case SDLK_d:
	    camRight = 1;
	  break;

	  default:
	  break;
	}
    break;

    case SDL_KEYUP:
      switch (e.key.keysym.sym) {

	case SDLK_w:
	  camUp = 0;
	break;

        case SDLK_s:
          camDown = 0;
	break;

	case SDLK_a:
	  camLeft = 0;
	break;

	case SDLK_d:
	  camRight = 0;
        break;

	default:
	break;
      }
    break;
    
    case SDL_MOUSEBUTTONDOWN:
      if (showStatic) {
	int diffx = mouse_x % 16;
	int diffy = mouse_y % 16;
	int newx = mouse_x - diffx;
	int newy = mouse_y - diffy;
	setActiveRect(newx, newy);
	showStatic = 0;
      } else {
	int diffx = mouse_x % 32;
	int diffy = mouse_y % 32;
	int newx = mouse_x - diffx;
	int newy = mouse_y - diffy;
	if (checkIfTaken(newx, newy)) {
	  
	} else {
	  struct Tile new;
	  createTile(&new, newx, newy, activeCropRect.x, activeCropRect.y);
	  addTile(new);
	}
      }
    break;
    
    default:
    break;
    }
  }
}

void movement(int x, int y)
{
  for (int i = 0; i < tileArrayUsed; i++) {
    tileArray[i].src.x += x;
    tileArray[i].src.y += y;
  }
}

void update()
{
  SDL_GetMouseState(&mouse_x, &mouse_y);
  activeDestRect.x = mouse_x - (TILE_SIZE/2);
  activeDestRect.y = mouse_y - (TILE_SIZE/2);

  if (showStatic == 0) {
    SDL_ShowCursor(SDL_DISABLE);
  }else {
    SDL_ShowCursor(SDL_ENABLE);
  }

  if (camLeft) {
    movement(TILE_SIZE,0);
    corr_x += TILE_SIZE;
  }else if (camRight) {
    movement(-TILE_SIZE,0);
    corr_x -= TILE_SIZE;
  }
  if (camUp) {
    movement(0,TILE_SIZE);
    corr_y += TILE_SIZE;
  }else if (camDown) {
    movement(0,-TILE_SIZE);
    corr_y -= TILE_SIZE;
  }
}

void render()
{
  SDL_RenderClear(gRender);


  for (int i = 0; i < tileArraySize; i++) {
    SDL_RenderCopy(gRender, staticSprite, &tileArray[i].crop, &tileArray[i].src);
  }
  if (showStatic == 1) {
    SDL_RenderCopy(gRender, staticSprite, NULL, &staticSpriteRect);
  }else {
    SDL_RenderCopy(gRender, staticSprite, &activeCropRect, &activeDestRect);
  }
  
  SDL_RenderPresent(gRender);
}

void mainloop()
{
  SDL_SetRenderDrawColor(gRender, 255,255,255,255);

  setActiveRect(0,0);

  corr_x = 0;
  corr_y = 0;
  
  camLeft = 0;
  camRight = 0;
  camDown = 0;
  camUp = 0;

  int FPS = 0;
  
  running = 1;
  while (running) {
    if (FPS + 30 < SDL_GetTicks()) {
      event();
      update();
      render();
      
      FPS = SDL_GetTicks();
    }
  }
}

int checkIfTaken(int x, int y)
{
  int contractIndex = 0;
  int occupied = 0;
  
  for (int i = 0; i < tileArrayUsed; i++) {
    if (tileArray[i].src.x == x && tileArray[i].src.y == y) {
      contractIndex = i;
      occupied = 1;
    }
  }
  if (occupied == 1) {
    for (int i = contractIndex; i <= tileArrayUsed; i++) {
      tileArray[i] = tileArray[i+1];
    }
    tileArrayUsed--;
    return 1;
  }
  return 0;
}

void addTile(struct Tile new)
{  
    if (tileArraySize <= tileArrayUsed) {
      tileArraySize *= 2;
      tileArray = realloc(tileArray, sizeof(struct Tile) * tileArraySize);
      if (tileArray == NULL) {
	printf("mem allocation failed\n");
      }
      printf("tileArray expanded\n");
    }
  
    tileArray[tileArrayUsed] = new;
    tileArrayUsed++;
  
}
void setActiveRect(int x, int y)
{
  activeDestRect.w = TILE_SIZE;
  activeDestRect.h = TILE_SIZE;

  activeCropRect.x = x;
  activeCropRect.y = y;
  activeCropRect.w = 16;
  activeCropRect.h = 16;
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
  strcpy(fpath, "levels/");
  
  strcat(fpath, levelFile);
  
  FILE* f = fopen(fpath, "a+");
  if (f == NULL) {
    printf("file not loaded: %s\n", fpath);
  }else {
    printf("file loaded succesfully\n");

    int linesInFile = 0;
    
    while (!feof(f)) {
      char s[30];
      if (fscanf(f, "%s\n", &s)) {
	
      }
      linesInFile++;
    }
    
    rewind(f);

    tileArraySize = linesInFile * 2;
    tileArrayUsed = 0;
    tileArray = NULL;
    tileArray = realloc(tileArray, sizeof(struct Tile) * tileArraySize);
    
    if (tileArray == NULL) {
      printf("mem not allocated\n");
    }else {
      printf("mem allocated\n");
    }

    
    while (!feof(f)) {
      
      int srcx;
      int srcy;
      int crox;
      int croy;
      
      if (fscanf(f, "%d %d %d %d\n", &srcx, &srcy, &crox, &croy)) {
	struct Tile new;
	createTile(&new, srcx, srcy, crox, croy);
        addTile(new);
      }else {
	printf("not working\n");
      }
    }
  }
  printf("file opened and closed\n");
  fclose(f);
}

void saveToFile()
{
  FILE* fp = fopen(fpath, "w");
  if (fp == NULL) {
    printf("file for saving failed to open\n");
  }
  for (int i = 0; i < tileArrayUsed; i++) {
    fprintf(fp,"%d %d %d %d\n",
	  tileArray[i].src.x - corr_x,
	  tileArray[i].src.y - corr_y,
	  tileArray[i].crop.x,
	  tileArray[i].crop.y);
  }
  
  fclose(fp);
  printf("saved to file\n");
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

  printf("editor exited...\n");
}
