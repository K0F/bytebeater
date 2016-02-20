#include "bytebeater.h"

#define TRUE 1
#define FALSE 0
#define BUFSIZE 256

#define WIDTH 640
#define HEIGHT 480
#define BPP 4
#define DEPTH 32

char buf[BUFSIZE];
char *pIn, *pOut, *pEnd;
char full;
int t;
unsigned char running = TRUE;


void buf_init(){
  pIn = pOut = buf;
  pEnd = &buf[BUFSIZE];
  full = 0;
}

int buf_put(char c){
  if(pIn == pOut && full)
    return 0;

  *pIn++ = c;
  if(pIn >= pEnd)
    pIn = buf;

  if(pIn == pOut)
    full = 1;

  return 1;
}

int buf_get(char *pc){
  if(pIn == pOut && !full)
    return 0;

  *pc = *pOut++;
  if(pOut >= pEnd)
    pOut = buf;

  full = 0;
  return 1;

}
void putpixel(SDL_Surface *surface, int x, int y, int pixel)
{
  int bpp = surface->format->BytesPerPixel;
  /* Here p is the address to the pixel we want to set */
  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

  switch(bpp) {
    case 1:
      *p = pixel;
      break;

    case 2:
      *(Uint16 *)p = pixel;
      break;

    case 3:
      if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
        p[0] = (pixel >> 16) & 0xff;
        p[1] = (pixel >> 8) & 0xff;
        p[2] = pixel & 0xff;
      } else {
        p[0] = pixel & 0xff;
        p[1] = (pixel >> 8) & 0xff;
        p[2] = (pixel >> 16) & 0xff;
      }
      break;

    case 4:
      *(Uint32 *)p = pixel;
      break;
  }
}


void DrawScreen(SDL_Surface* screen, int h)
{ 
  int x, y, ytimesw;

  //buf_put(t % BUFSIZE);
  //t++;
  //int pix = buf_get(t%BUFSIZE);


  if(SDL_MUSTLOCK(screen)) 
  {
    if(SDL_LockSurface(screen) < 0) return;
  }

  for(y = 0; y < screen->h; y++ ) 
  {
    ytimesw = y*screen->pitch/BPP;
    for( x = 0; x < screen->w; x++ ) 
    {
      putpixel(screen, x, ytimesw, h);
    }
  }

  if(SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);

  SDL_Flip(screen); 
}


int main(int argc,char *argv[]){

  buf_init();

  SDL_Surface *screen;
  SDL_Event event;

  int keypress = 0;
  int h = 0;


  if(SDL_Init(SDL_INIT_VIDEO) < 0) return 1;

  if (!(screen = SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH, SDL_HWSURFACE)))
  {
    SDL_Quit();
    return 1;
  }

  while(TRUE) 
  {
    DrawScreen(screen,h++);
    while(SDL_PollEvent(&event)) 
    {      
      switch (event.type) 
      {
        case SDL_QUIT:
          keypress = 1;
          break;
        case SDL_KEYDOWN:
          keypress = 1;
          break;
      }
    }
  }

  SDL_Quit();




  return 0;
}
