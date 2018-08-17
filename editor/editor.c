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

		    case SDLK_b:
				if (makeSolid) {
					makeSolid = 0;
				} else {
					makeSolid = 1;
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

		    case SDLK_e:
				printf("x on first: %d\n", tileArray[0].src.x);
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
		  
		  int diffx = mouse_x % TILE_CROP_SIZE;
		  int diffy = mouse_y % TILE_CROP_SIZE;
		  showStatic = 0;
		  
      } else {
		  
		  int diffx = mouse_x % TILE_SIZE;
		  int diffy = mouse_y % TILE_SIZE;
		  int newx = mouse_x - diffx;
		  int newy = mouse_y - diffy;
		  
		  if (checkIfTaken(newx, newy)) {
	  
		  } else {
			  struct Tile new;

			  if (makeSolid) {
				  createTile(&new, newx, newy, activeCropRect.x, activeCropRect.y, 1);  
			  }else {
				  createTile(&new, newx, newy, activeCropRect.x, activeCropRect.y, 0);
			  }
	  
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

  animateEnemy();
}

void render()
{
  SDL_RenderClear(gRender);

  SDL_RenderCopy(gRender, backgroundSprite, NULL, NULL);

  for (int i = 0; i < tileArraySize; i++) {
    SDL_RenderCopy(gRender, staticSprite, &tileArray[i].crop, &tileArray[i].src);
    if (tileArray[i].isSolid == 1) {
      SDL_RenderCopy(gRender, editorSprite, &solidEditorRect, &tileArray[i].src);
    }
  }
  
  if (showStatic == 1) {
    SDL_RenderCopy(gRender, staticSprite, NULL, &staticSpriteRect);
  }else {
    SDL_RenderCopy(gRender, staticSprite, &activeCropRect, &activeDestRect);
    if (makeSolid ) {
      SDL_RenderCopy(gRender, editorSprite, &solidEditorRect, &activeDestRect);
    }
  }

  SDL_RenderCopy(gRender, enemySprite, &enemyCrop, &enemySource);
  
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

void animateEnemy()
{
	if (enemyAnimationClock == 3) {
		enemyCrop.y += TILE_CROP_SIZE;
		if (enemyCrop.y == (TILE_CROP_SIZE*4)) {
			enemyCrop.y = 0;
		}
		enemyAnimationClock = 0;
	}
	enemyAnimationClock++;
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
  activeCropRect.w = TILE_CROP_SIZE;
  activeCropRect.h = TILE_CROP_SIZE;
}

void initEditor()
{
  gWindow = SDL_CreateWindow("EDITOR",
			     SDL_WINDOWPOS_CENTERED,
			     SDL_WINDOWPOS_CENTERED,
			     640,420, SDL_WINDOW_SHOWN);
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

void createTile(struct Tile* new, int x, int y, int cx, int cy, int solid){
  new->src.x = x;
  new->src.y = y;
  new->src.w = TILE_SIZE;
  new->src.h = TILE_SIZE;

  new->crop.x = cx;
  new->crop.y = cy;
  new->crop.w = TILE_CROP_SIZE;
  new->crop.h = TILE_CROP_SIZE;

  new->isSolid = solid;
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
    
    printf("lines in empty file : %d\n", linesInFile);
    
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

    
    while (!feof(f) && linesInFile > 1) {
      
      int srcx = 0;
      int srcy = 0;
      int crox = 0;
      int croy = 0;
      int solid = 0;
      
      if (fscanf(f, "%d %d %d %d %d\n", &srcx, &srcy, &crox, &croy, &solid)) {
		  struct Tile new;
		  createTile(&new, srcx, srcy, crox, croy, solid);
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
    fprintf(fp,"%d %d %d %d %d\n",
	  tileArray[i].src.x - corr_x,
	  tileArray[i].src.y - corr_y,
	  tileArray[i].crop.x,
	  tileArray[i].crop.y,
	  tileArray[i].isSolid);
  }
  
  fclose(fp);
  printf("saved to file\n");
}

void loadLevelType(char* levelType)
{
  char ssFilepath[40];

  strcpy(ssFilepath, "levels/");
  strcat(ssFilepath, levelType);
  strcat(ssFilepath, ".png");

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
  
//------------------------------------------------------------

  char bgFP[40];
  strcpy(bgFP, "levels/");
  strcat(bgFP, levelType);
  strcat(bgFP, "_bg.png");

  printf("Background sprite is loaded from: %s\n", bgFP );

  SDL_Surface* loadSurf3 = IMG_Load(bgFP);

  if (loadSurf3 == NULL) {
    printf("%s\n", SDL_GetError() );
  }
  
  backgroundSprite = SDL_CreateTextureFromSurface(gRender, loadSurf3);

  SDL_FreeSurface(loadSurf3);

  if (backgroundSprite == NULL) {
    printf("%s\n", SDL_GetError() );
  }

//------------------------------------------------------------

  SDL_Surface* loadSurf2 = IMG_Load("always.png");

  if (loadSurf2 == NULL) {
    printf("%s\n", SDL_GetError());
  }
  
  editorSprite = SDL_CreateTextureFromSurface(gRender, loadSurf2);

  SDL_FreeSurface(loadSurf2);

  if (editorSprite == NULL) {
    printf("%s\n", SDL_GetError());
  }

//------------------------------------------------------------

  SDL_Surface* loadSurf4 = IMG_Load("enemy.png");

  if (loadSurf4 == NULL) {
	  printf("%s\n", SDL_GetError() );
  }
  
  enemySprite = SDL_CreateTextureFromSurface(gRender, loadSurf4);

  SDL_FreeSurface(loadSurf4);

  if (enemySprite == NULL) {
	  printf("%s\n", SDL_GetError() );
  }

  enemySource.x = 64;
  enemySource.y = 64;
  enemySource.w = TILE_SIZE;
  enemySource.h = TILE_SIZE;

  enemyCrop.x = 0;
  enemyCrop.y = 0;
  enemyCrop.w = TILE_CROP_SIZE;
  enemyCrop.h = TILE_CROP_SIZE;

  enemyAnimationClock = 0;
  
//------------------------------------------------------------

  solidEditorRect.x = 0;
  solidEditorRect.y = 0;
  solidEditorRect.w = 16;
  solidEditorRect.h = 16;
  
  staticSpriteRect.x = 0;
  staticSpriteRect.y = 0;

  SDL_QueryTexture(staticSprite, NULL, NULL, &staticSpriteRect.w, &staticSpriteRect.h);
  
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

  SDL_DestroyTexture(backgroundSprite);
  backgroundSprite = NULL;
  SDL_DestroyTexture(editorSprite);
  editorSprite = NULL;
  SDL_DestroyTexture(staticSprite);
  staticSprite = NULL;
  SDL_DestroyRenderer(gRender);
  gRender = NULL;
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;

  SDL_Quit();

  printf("editor exited...\n");
}
