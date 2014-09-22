#include <cassert>
#include <iostream>
#include <libgen.h>
#include <vector>
#include <string>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "gpio.hpp"
#include "graphics.hpp"

#define DELAY  100
#define STEPS  100
#define HEIGHT 160
#define WIDTH  128
#define RADIUS 20

void forward(std::vector<int> gpios_out, std::vector<int> gpios_first, std::vector<int> gpios_second, int delay, int steps){

   std::vector<int>::iterator pin; 
   std::vector<int>::iterator val; 

   for (pin = gpios_out.begin(), val = gpios_first.begin(); pin != gpios_out.end(); ++pin, ++val)
   {
       	GPIOWrite(*pin, *val);
   }
   usleep(delay);
   for (pin = gpios_out.begin(), val = gpios_second.begin(); pin != gpios_out.end(); ++pin, ++val)
   {
       	GPIOWrite(*pin, *val);
   }
   usleep(delay);

}


void backwards(std::vector<int> gpios_out, std::vector<int> gpios_first, std::vector<int> gpios_second, int delay, int steps){

   std::vector<int>::iterator pin; 
   std::vector<int>::iterator val; 

   for (pin = gpios_out.begin(), val = gpios_second.begin(); pin != gpios_out.end(); ++pin, ++val)
   {
       	GPIOWrite(*pin, *val);
   }
   usleep(delay);
   for (pin = gpios_out.begin(), val = gpios_first.begin(); pin != gpios_out.end(); ++pin, ++val)
   {
       	GPIOWrite(*pin, *val);
   }
   usleep(delay);

}
 
void stop(std::vector<int> gpios_out, std::vector<int> gpios_stop){

  std::vector<int>::iterator pin; 
  std::vector<int>::iterator val; 

  for (pin = gpios_out.begin(), val = gpios_stop.begin(); pin != gpios_stop.end(); ++pin, ++val)
  {
      GPIOWrite(*pin, *val);
  } 
}

int main(int argc, char* argv[])
{
  std::vector<int> gpios_in = {22};
  std::vector<int> gpios_out = {4, 17, 18, 23};
  std::vector<int> gpios_first = {1, 1, 0, 1};
  std::vector<int> gpios_second = {1, 0, 1, 1};
  std::vector<int> gpios_stop = {0, 0, 0, 0};

  std::vector<int>::iterator pin; 

  int button =  1;

  SDL_putenv("SDL_FBDEV=/dev/fb1");

  std::string images_path (dirname(argv[0]));
  images_path += "/images";

  /* Init */
  if(SDL_Init(SDL_INIT_VIDEO) != 0){
	fprintf(stderr,"Could not initialize SDL: %s\n",SDL_GetError());
        exit(1);
  }

  SDL_ShowCursor(0);
  	
  // load support for the JPG and PNG image formats
  int flags=IMG_INIT_JPG|IMG_INIT_PNG;
  int initted=IMG_Init(flags);
  if(initted&flags != flags) {
    printf("IMG_Init: Failed to init required jpg and png support!\n");
    printf("IMG_Init: %s\n", IMG_GetError());
    exit(1);
  }
  
  printf("SDL Initialized\n");
  SDL_Surface *screen;
  SDL_Rect dstRect;
  SDL_Surface *webcam, *inter;
  SDL_RWops *rwop;


  if( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
      fprintf(stderr,
              "Couldn't initialize SDL: %s\n", SDL_GetError());
      exit(1);
  }

  /* Clean up on exit */
  atexit(SDL_Quit);
    
  /*
   * Initialize the display in a 128x160 8-bit palettized mode,
   * requesting a software surface
   */
  screen = SDL_SetVideoMode(WIDTH, HEIGHT, 8, SDL_DOUBLEBUF|SDL_SWSURFACE);
  if ( screen == NULL ) {
      fprintf(stderr, "Couldn't set 128x160 video mode: %s\n",
                       SDL_GetError());
      exit(1);
  }

  //load the SDL logo bitmap to a temporary surface
  SDL_Surface* temp = SDL_LoadBMP((images_path + "/background.bmp").c_str());  //PROBLEM LINE
  if (temp == NULL)   //TEMP KEEPS RETURNING NULL
  {
      printf("Unable to load bitmap: %s\n", SDL_GetError());
      exit(1);
  };
  
  //create the working SDL_Surface which matches the display format
  //of the temporary surface
  SDL_Surface* bg = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);

  // load  webcam image
  rwop=SDL_RWFromFile((images_path + "/webcam.jpg").c_str(), "rb");
  webcam=IMG_LoadJPG_RW(rwop);
  if(!webcam) {
      printf("IMG_LoadJPG_RW: %s\n", IMG_GetError());
  } 

  SDL_Event event;
  bool quit = false;
  int dir=-1, y=80;

  for (pin = gpios_out.begin(); pin != gpios_out.end(); ++pin)
  {
      GPIOExport(*pin);
  }	 

  for (pin = gpios_out.begin(); pin != gpios_out.end(); ++pin)
  {
      GPIODirection(*pin,OUT);
  }	 

  for (pin = gpios_in.begin(); pin != gpios_in.end(); ++pin)
  {
      GPIOExport(*pin);
  }	 

  for (pin = gpios_in.begin(); pin != gpios_in.end(); ++pin)
  {
      GPIODirection(*pin,IN);
  }
  
  //This is the main message loop of the game
  while(!quit)
  {
     // load interruptor image
     //rwop=SDL_RWFromFile(images_path + "interruptor.jpg"), "rb");
     //inter=IMG_LoadJPG_RW(rwop);
     //if(!inter) {
     //   printf("IMG_LoadJPG_RW: %s\n", IMG_GetError());
     //	  quit(1);
     //}  

     //check the message queue for an event
     if(SDL_PollEvent(&event))
     {
  	switch(event.type)
  	{
  	   case SDL_QUIT:
  	   	quit = true;
             	break;

           case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT){
		}
		break;

  	   case SDL_KEYDOWN:
		
  		switch(event.key.keysym.sym)
  		{
  			case SDLK_ESCAPE:
  			    quit = true;
  			    break;
  			case SDLK_UP:
  			    break;
  			case SDLK_DOWN:
  			    break;
  			case SDLK_LEFT:
  			    break;
  			case SDLK_RIGHT:
  			    break;
  		}
                
  		break;
  	};

     };

     SDL_Delay(DELAY);

     y += dir;

     if (y == 80){
	dir = -1;
     }
     else if (y==-80){
	dir = 1;
     }
     
     //button = GPIORead(22); 
     if (button) {
        forward(gpios_out, gpios_first, gpios_second, DELAY, STEPS);
     	// load  webcam image
     	//rwop=SDL_RWFromFile((images_path + "/webcam.jpg").c_str(), "rb");
     	//webcam=IMG_LoadJPG_RW(rwop);
     	//if(!webcam) {
       	//     printf("IMG_LoadJPG_RW: %s\n", IMG_GetError());
	//     exit(1);
     	//}  
     }
     else {
	stop(gpios_out, gpios_stop);
     }
  
     dstRect.x = 0;
     dstRect.y = y;

     //draw the background sprite
     SDL_BlitSurface(bg, NULL, screen, &dstRect);

     int x = rand() % (WIDTH - 4) + 2;
     int y = rand() % (HEIGHT - 4) + 2;
     int r = rand() % (RADIUS - 10) + 10;
     int c = ((rand() % 0xff) << 16) +
             ((rand() % 0xff) << 8) +
             (rand() % 0xff);
 
     if (r >= 4)
     {
        if (x < r + 2)
            x = r + 2;
        else if (x > WIDTH - r - 2)
            x = WIDTH - r - 2;
 
        if (y < r + 2)
            y = r + 2;
        else if (y > HEIGHT - r - 2)
            y = HEIGHT - r - 2;
     }

     //SDL_LockSurface(screen);
     //draw_circle(screen, x, y, r, (Uint8) 0x00);
     //SDL_FreeSurface(screen);

     //update the current window
     //SDL_UpdateRect(screen, 0, 0, WIDTH, HEIGHT);

     SDL_Flip(screen);
  };

  for (pin = gpios_out.begin(); pin != gpios_out.end(); ++pin)
  {
	GPIOUnexport(*pin);
  }	 

  for (pin = gpios_in.begin(); pin != gpios_in.end(); ++pin)
  {
	GPIOUnexport(*pin);
  }
  
  //free the allocated memory for the background surface
  SDL_FreeSurface(bg);

  /* Exit */
  SDL_Quit();
  printf("SDL Shutdown\n");
  /* Done */
  return 0;
}
