#include "setup.h"


// The initalize process
void initSetup()
{
  
  printf("Initialize game\n");

  // Calls for SDL to init everything from audio to event
  SDL_Init(SDL_INIT_EVERYTHING);

  // Creating a window with title "HELLO WORLD", positioned in the center of screen, with macrodefined size
  gameWindow = SDL_CreateWindow("HELLO WORLD", SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED,SCREEN_SIZE, SCREEN_SIZE,
				SDL_WINDOW_SHOWN);

  if (gameWindow == NULL) {

    printf("gameWindow couldn't be created\n");

  }else {
    // If creating of window is succesfull we create the render and set it to be hardwareaccelerated(using the dedicated graphics card)
    gameRender = SDL_CreateRenderer(gameWindow,
				    -1,
				    SDL_RENDERER_ACCELERATED);
    if (gameRender == NULL) {

      printf("gameRender couldn't be created\n");

    }else {
      
      SDL_SetRenderDrawColor(gameRender, 255,255,255,255);

      // Gamestate will be set to menu in the begining
      gamestate = MENU;

      //LOADING OF TEST TEXTURE
      testImage = loadTexture("DATA/test.png", gameRender);
      if (testImage == NULL) {
	printf("Image not created %s\n", SDL_GetError() );
      }
      
      
      gameloop();

    }
  }

  // quit the game 
  quit();
}

// Gamelooptroop rockers
void gameloop()
{
  int countedFrames = 0;
  // while gamestate is anything but QUIT the loop will be running with eventchecking, logical operations and rendering
  while (gamestate != QUIT) {
    if (countedFrames + FPS < SDL_GetTicks()) {
    
      updateLogic();
      eventCheck();
      drawScreen();

      countedFrames = SDL_GetTicks();
    }
    
  }
}

//
void updateLogic()
{

}

// SDL_PollEvent returns a buffer of events happening
void eventCheck()
{
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      // If event is the X window button being pressed gamestate is changed to QUIT 
      case SDL_QUIT:
	  gamestate = QUIT;
	break;
	
      // If a key is pressed down	
      case SDL_KEYDOWN:

	// we check wich key is pressed
	  switch (event.key.keysym.sym) {
	    
	    // if the key = ESC, gamestate = QUIT 
	     case SDLK_ESCAPE: 
	       gamestate = QUIT;
	     break;
	     
	     case SDLK_RETURN:
	       if (gamestate == MENU) {
		 gamestate = PLAY;
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

// Screen is drawn
void drawScreen()
{
  // Clear screan 
  SDL_RenderClear(gameRender);

  // draw menuspecific items if gamestate is MENU
  if (gamestate == MENU) {
    SDL_RenderCopy(gameRender, testImage, NULL, NULL);
  }
  
  // draw gameplay
  if (gamestate == PLAY) {
    
  }

  // Update screen to next position in buffer
  SDL_RenderPresent(gameRender);
}

// Quitting function that cleans up and destroys pointers
void quit()
{

  SDL_DestroyTexture(testImage);
  testImage = NULL;

  SDL_DestroyRenderer(gameRender);
  gameRender = NULL;

  SDL_DestroyWindow(gameWindow);
  gameWindow = NULL;
  
  SDL_Quit();
  
  printf("game executed fine\n");
}


