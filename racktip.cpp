#include "racktip.hpp"

int diry=1, dirx=1, x=DROPPER_X_BEGIN, y=PLATES_Y_BEGIN;
int button = 0;
bool quit = false;

int main(int argc, char* argv[])
{
  SDL_Surface *screen;
  SDL_Surface *textSurface;
  SDL_Rect dstRect;
  SDL_Surface *dropper;
  SDL_RWops *rwop;
  SDL_Event event;
  SDL_Rect textLocation;


  SDL_putenv("SDL_FBDEV=/dev/fb1");

  std::string images_path (dirname(argv[0]));
  images_path += "/images";

  // init
  if(SDL_Init(SDL_INIT_VIDEO) != 0){
	fprintf(stderr,"Could not initialize SDL: %s\n",SDL_GetError());
        exit(1);
  }
  SDL_ShowCursor(0);

  TTF_Init();
  TTF_Font* font = TTF_OpenFont("ARIAL.TTF", 12);

  // load support for the JPG and PNG image formats
  int flags=IMG_INIT_JPG|IMG_INIT_PNG;
  int initted=IMG_Init(flags);
  if(initted&flags != flags) {
    printf("IMG_Init: %s\n", IMG_GetError());
    exit(1);
  }
  
  printf("SDL Initialized\n");

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
  screen = SDL_SetVideoMode(WIDTH, HEIGHT, 8, SDL_SWSURFACE);
  if ( screen == NULL ) {
      fprintf(stderr, "Couldn't set 128x160 video mode: %s\n",
                       SDL_GetError());
      exit(1);
  }

  //load the SDL logo bitmap to a temporary surface
  SDL_Surface* temp = SDL_LoadBMP((images_path + "/background.bmp").c_str());
  if (temp == NULL)   //TEMP KEEPS RETURNING NULL
  {
      printf("Unable to load bitmap: %s\n", SDL_GetError());
      exit(1);
  };
  
  //create the working SDL_Surface which matches the display format
  //of the temporary surface
  SDL_Surface* bg = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);

  // load  dropper image
  rwop=SDL_RWFromFile((images_path + "/dropper.png").c_str(), "rb");
  dropper=IMG_LoadPNG_RW(rwop);
  if(!dropper) {
      printf("IMG_LoadPNG_RW: %s\n", IMG_GetError());
  } 

  export_pins();
  
  //This is the main message loop of the game
  while(!quit)
  {
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
     
     button = read_input();
     if (button) {
        rewind();
     	y = PLATES_Y_BEGIN;
     }
     else if (y < TOTAL_STEPS){
        if (y > FIRST_LINE && y % NEXT_LINE == 0){
            stop();
            x += dirx * DROPPER_X_MOVE;
            if (x >= LINE_END) {
            	dirx = -1;
            	y += PLATE_Y_MOVE;
            }
            else if (x <= DROPPER_X_BEGIN){
            	dirx = 1;
            	y += diry;
            }
        }
        else {
        	forward();
        	y += diry;
        }
     }
  
     dstRect.x = PLATES_X_BEGIN;dstRect.y = y;

     //draw the background sprite
     SDL_BlitSurface(bg, NULL, screen, &dstRect);

     //update the current window
     SDL_UpdateRect(screen, 0, 0, WIDTH, HEIGHT);

     dstRect.x = x;dstRect.y = DROPPER_Y_BEGIN;

     //draw the pin dropper sprite
     SDL_BlitSurface(dropper, NULL, screen, &dstRect);

     //update the current window
     SDL_UpdateRect(screen, 0, 0, 0, 0);

     textLocation = { 100, 100, 0, 0 };
     SDL_Color foregroundColor = { 255, 255, 255 };
     SDL_Color backgroundColor = { 0, 0, 255 };
     textSurface = TTF_RenderText_Shaded(font, "This is my text.",foregroundColor, backgroundColor);
     SDL_BlitSurface(textSurface, NULL, screen, &textLocation);
     //SDL_Flip(screen);
  };

  unexport_pins(); 
 
  //free the allocated memory for the background surface
  SDL_FreeSurface(bg);
  SDL_FreeSurface(textSurface);
  TTF_CloseFont(font);

  /* Exit */
  SDL_Quit();
  printf("SDL Shutdown\n");
  /* Done */
  return 0;
}
