#include "setup.h"

void initSetup()
{
  
  printf("Initialize game\n");

  SDL_Init(SDL_INIT_EVERYTHING);
  
  gameWindow = SDL_CreateWindow("HELLO WORLD", SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED,SCREEN_SIZE, SCREEN_SIZE,
				SDL_WINDOW_SHOWN);

  if (gameWindow == NULL) {

    printf("gameWindow couldn't be created\n");

  }else {
 
    gameRender = SDL_CreateRenderer(gameWindow,
				    -1,
				    SDL_RENDERER_ACCELERATED);
    if (gameRender == NULL) {

      printf("gameRender couldn't be created\n");

    }else {

      SDL_SetRenderDrawColor(gameRender, 255,255,255,255);
      
      gamestate = MENU;
      gameloop();

    }
  }
  quit();
}

void gameloop()
{
  while (gamestate == MENU){
    eventCheck();
    drawScreen();
  }
}

void eventCheck()
{
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
	  gamestate = QUIT;
	break;
      case SDL_KEYDOWN: 
	  switch (event.key.keysym.sym) {

	     case SDLK_ESCAPE: 
	       gamestate = QUIT;
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

void drawScreen()
{
  SDL_RenderClear(gameRender);
  
  SDL_RenderPresent(gameRender);
}
void quit()
{
  SDL_DestroyWindow(gameWindow);
  SDL_DestroyRenderer(gameRender);

  SDL_Quit();
  
  printf("game executed fine\n");
}
