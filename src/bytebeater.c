/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) Krystof Pesek, 2016
 */




#include "bytebeater.h"

#define TRUE 1
#define FALSE 0
#define BUFSIZE 256

#define WIDTH 640
#define HEIGHT 480
#define BPP 24
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
int time =0;
/*
// NOTE: Sound test
int SamplesPerSecond = 44100;
int ToneHz = 256;
Uint16 ToneVolume = 3000;
Uint32 RunningSampleIndex = 0;
int SquareWavePeriod = SamplesPerSecond / ToneHz;
int HalfSquareWavePeriod = SquareWavePeriod / 2;
int BytesPerSample = sizeof(int16) * 2;
*/

/*
  internal void
SDLAudioCallback(void *UserData, Uint8 *AudioData, int Length)
{
  sdl_audio_ring_buffer *RingBuffer = (sdl_audio_ring_buffer *)UserData;

  int Region1Size = Length;
  int Region2Size = 0;
  if (RingBuffer->PlayCursor + Length > RingBuffer->Size)
  {
    Region1Size = RingBuffer->Size - RingBuffer->PlayCursor;
    Region2Size = Length - Region1Size;
  }
  memcpy(AudioData, (uint8*)(RingBuffer->Data) + RingBuffer->PlayCursor, Region1Size);
  memcpy(&AudioData[Region1Size], RingBuffer->Data, Region2Size);
  RingBuffer->PlayCursor = (RingBuffer->PlayCursor + Length) % RingBuffer->Size;
  RingBuffer->WriteCursor = (RingBuffer->PlayCursor + 2048) % RingBuffer->Size;
}
*/

/////////////////////////////////////////////



struct sdl_audio_ring_buffer
{
  int Size;
  int WriteCursor;
  int PlayCursor;
  void *Data;
};




void render(){   


  frametime = SDL_GetTicks (); 
  time++;

  // Lock surface if needed
  if (SDL_MUSTLOCK(screen)) 
    if (SDL_LockSurface(screen) < 0) 
      return;



  // Draw to screen
  int offset = 0;
  float r,g,b;
  for (int y = 0; y < HEIGHT; y+=2){
    for (int x = 0 ;x < WIDTH; x+=2){
      t++;
      r = ((x^y+(int)(time/sqrt(((WIDTH/2-x)*(WIDTH/2-x)) + ((HEIGHT/2-y)*(HEIGHT/2-y)))*100) )+(time/sqrt(x*x+y*y)));
      g = ((x^y+(int)(time/sqrt(((WIDTH/2-x)*(WIDTH/2-x)) + ((HEIGHT/2-y)*(HEIGHT/2-y)))*101) )+(time/sqrt(x*x+y*y)));
      b = ((x^y+(int)(time/sqrt(((WIDTH/2-x)*(WIDTH/2-x)) + ((HEIGHT/2-y)*(HEIGHT/2-y)))*102) )+(time/sqrt(x*x+y*y)));
      int color = SDL_MapRGB(screen->format,r,g,b);
      PutPixel24(screen,x,y,color);
    }
  }

  // Unlock if needed
  if (SDL_MUSTLOCK(screen)) 
    SDL_UnlockSurface(screen);

  // Tell SDL to update the whole screen
}

int main(int argc,char *argv[]){


  /* Initialize defaults, Video and Audio */

  if( SDL_Init(SDL_INIT_VIDEO)==(-1) ) { 

    printf("Could not initialize SDL: %s.\n", SDL_GetError());

    exit(-1);

  }

  screen = SDL_SetVideoMode(WIDTH, HEIGHT, BPP, SDL_HWSURFACE );

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

  int running = 1;
  SDL_Event event;
  int t = 0;
  minframetime = (1000.0 / fps);


  while(running==1){


    render();

    SDL_UpdateRect(screen, 0, 0, WIDTH, HEIGHT);    
    SDL_BlitSurface( sText,NULL, screen,&rcDest );
    SDL_Flip(screen);
    SDL_Delay(16.667);

    while (SDL_PollEvent (&event) != 0){
      switch(event.type){
        case SDL_KEYDOWN: if (event.key.keysym.sym == SDLK_ESCAPE)
                            running = 0;
                          break;
      }
    }
  }


  TTF_CloseFont( font );
  SDL_FreeSurface( sText );

  atexit(TTF_Quit);

  SDL_FreeSurface (screen);
  SDL_Delay (200);


  return 0;

}


void PutPixel24(SDL_Surface * surface, int x, int y, Uint32 color)
{
  Uint8 * pixel = (Uint8*)surface->pixels;
  pixel += (y * surface->pitch) + (x * sizeof(Uint8) * 3);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  pixel[0] = (color >> 24) & 0xFF;
  pixel[1] = (color >> 16) & 0xFF;
  pixel[2] = (color >> 8) & 0xFF;
#else
  pixel[0] = color & 0xFF;
  pixel[1] = (color >> 8) & 0xFF;
  pixel[2] = (color >> 16) & 0xFF;
#endif
}



