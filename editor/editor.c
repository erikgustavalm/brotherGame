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

  SDL_RenderPresent(gRender);
}
void mainloop()
{
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
    
  }
}

void loadLevelFile(char* levelFile)
{
  char pre[50];
  strcpy(pre, "levels/");
  
  strcat(pre, levelFile);
  
  FILE* f = fopen(pre, "a+");
  if (f == NULL) {
    printf("file not loaded: %s\n", pre);
  }else {
    printf("file loaded succesfully\n");
    
  }
  
  fclose(f);
}
void startEditor(char* levelFile)
{
  SDL_Init(SDL_INIT_EVERYTHING);

  loadLevelFile(levelFile);
  
  initEditor();

  mainloop();

  quit();
}

void quit()
{
  SDL_DestroyRenderer(gRender);
  gRender = NULL;
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;

  SDL_Quit();

  printf("byebye\n");
}
