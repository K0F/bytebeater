#include "bytebeater.h"

#define TRUE 1
#define FALSE 0
#define BUFSIZE 256

#define WIDTH 640
#define HEIGHT 480
#define BPP 8
#define DEPTH 32
#define sec 1000;


float fps = 60;

char buf[BUFSIZE];
char *pIn, *pOut, *pEnd;
char full;
int t;
unsigned char running = TRUE;

float minframetime;
SDL_Surface *screen;
Uint32 frametime;



void putpixel(int x, int y, int color){
  unsigned int *ptr = (unsigned int*)screen->pixels;
  int lineoffset = y * (screen->pitch / 4);
  ptr[lineoffset + x] = color;
}






void render(){   

  t++;

  frametime = SDL_GetTicks (); 


  // Lock surface if needed
  if (SDL_MUSTLOCK(screen)) 
    if (SDL_LockSurface(screen) < 0) 
      return;



  // Draw to screen
  int offset = 0;
  Uint32 *pixels = (Uint32*) screen->pixels;
  for (int y = 0; y < HEIGHT; y++){
    for (int x = 0 ;x < WIDTH; x++){
      offset = (screen->pitch / sizeof(Uint32)) * y + x;
      int br = (x*x+y*y)+t ;
      *(pixels + offset) = br;
    }
  }


  /*
     putpixel(10, 10, 0xff0000);
     putpixel(11, 10, 0xff0000);
     putpixel(10, 11, 0xff0000);
     putpixel(11, 11, 0xff0000);
     */
  // Unlock if needed
  if (SDL_MUSTLOCK(screen)) 
    SDL_UnlockSurface(screen);

  // Tell SDL to update the whole screen
  SDL_UpdateRect(screen, 0, 0, WIDTH, HEIGHT);    
}

int main(int argc,char *argv[]){


  /* Initialize defaults, Video and Audio */

  if( SDL_Init(SDL_INIT_VIDEO)==(-1) ) { 

    printf("Could not initialize SDL: %s.\n", SDL_GetError());

    exit(-1);

  }



  screen = SDL_SetVideoMode( WIDTH, HEIGHT, BPP, SDL_HWSURFACE );

  if( screen == NULL ) {

    fprintf(stderr, "Couldn't set %ix%ix%i video mode: %s\n", WIDTH,HEIGHT,BPP, SDL_GetError());

    exit(1);

  }


  //SDL_ShowCursor( SDL_DISABLE ); // The cursor is ugly :)

  TTF_Init();
  TTF_Font *font = TTF_OpenFont( "assets/SempliceRegular.ttf", 8 );

  SDL_Color clrFg = {255,255,255,0};  // Blue ("Fg" is foreground)
  SDL_Surface *sText = TTF_RenderText_Solid( font, "Some random text", clrFg );
  SDL_Rect rcDest = {10,10,0,0};
  SDL_BlitSurface( sText,NULL, screen,&rcDest );
  SDL_FreeSurface( sText );

  int running = 1;
  SDL_Event event;
  int t = 0;
  minframetime = (1000.0 / fps);


  while(running==1){

    render();


    while (SDL_PollEvent (&event) != 0){
      switch(event.type){
	case SDL_KEYDOWN: if (event.key.keysym.sym == SDLK_ESCAPE)
			    running = 0;
			  break;
      }
    }
  }


  TTF_CloseFont( font );

  atexit(TTF_Quit);

  SDL_FreeSurface (screen);
  SDL_Delay (200);


  return 0;

}
