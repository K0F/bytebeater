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

typedef struct{
  Uint8 r;
  Uint8 g;
  Uint8 b;
  Uint8 alpha;
} pixel;





int main(int argc,char *argv[]){


  /* Initialize defaults, Video and Audio */

  if( SDL_Init(SDL_INIT_VIDEO)==(-1) ) { 

    printf("Could not initialize SDL: %s.\n", SDL_GetError());

    exit(-1);

  }



  /* Set up the video */

  SDL_Surface *screen;


  screen = SDL_SetVideoMode( WIDTH, HEIGHT, BPP, SDL_HWSURFACE );

  if( screen == NULL ) {

    fprintf(stderr, "Couldn't set %ix%ix%i video mode: %s\n", WIDTH,HEIGHT,BPP, SDL_GetError());

    exit(1);

  }


  //SDL_ShowCursor( SDL_DISABLE ); // The cursor is ugly :)



  /* Set up the SDL_TTF */

  TTF_Init();
  TTF_Font *font = TTF_OpenFont( "assets/SempliceRegular.ttf", 8 );

  SDL_Color clrFg = {255,255,255,0};  // Blue ("Fg" is foreground)
  SDL_Surface *sText = TTF_RenderText_Solid( font, "Some random text", clrFg );
  SDL_Rect rcDest = {0,0,0,0};
  SDL_BlitSurface( sText,NULL, screen,&rcDest );
  SDL_FreeSurface( sText );

  float minframetime = (1000.0 / fps);
  int running = 1;
  SDL_Event event;
  Uint32 frametime;

  while(running==1){
    frametime = SDL_GetTicks (); 

    while (SDL_PollEvent (&event) != 0){
      switch(event.type){
        case SDL_KEYDOWN: if (event.key.keysym.sym == SDLK_ESCAPE)
                            running = 0;
                          break;
      }
    }


    if (SDL_GetTicks () - frametime < minframetime)
      SDL_Delay (minframetime - (SDL_GetTicks () - frametime));
      
    
    SDL_Flip (screen);

  }


  TTF_CloseFont( font );

  atexit(TTF_Quit);

  SDL_Flip (screen);
  SDL_FreeSurface (screen);
  SDL_Delay (200);


  return 0;

}
