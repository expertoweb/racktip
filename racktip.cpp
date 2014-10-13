#include "racktip.hpp"

int diry, dirx, x=0, y=0;
int button = 0;
bool dropping = false;
bool quit = false;

void read_params(){
	std::ifstream infile("constants.hpp");

	std::string a, b, c;

	while (infile >> a >> b >> c)
	{
		if (a == "//#define")
			continue;
		if (b == "HEIGHT")
				HEIGHT = stoi(c);
		else if (b == "WIDTH")
				WIDTH = stoi(c);
		else if (b == "DELAY")
				DELAY = stoi(c);
		else if (b == "STEPS")
				STEPS = stoi(c);
		else if (b == "TOTAL_STEPS")
				TOTAL_STEPS = stoi(c);
		else if (b == "PLATES_X_BEGIN")
				PLATES_X_BEGIN = stoi(c);
		else if (b == "PLATES_Y_BEGIN")
				PLATES_Y_BEGIN = stoi(c);
		else if (b == "PLATE_Y_MOVE")
				PLATE_Y_MOVE = stoi(c);
		else if (b == "DROPPER_X_BEGIN")
				DROPPER_X_BEGIN = stoi(c);
		else if (b == "DROPPER_X_END")
				DROPPER_X_END = stoi(c);
		else if (b == "DROPPER_Y_BEGIN")
				DROPPER_Y_BEGIN = stoi(c);
		else if (b == "DROPPER_X_MOVE")
				DROPPER_X_MOVE = stoi(c);
		else if (b == "RADIUS")
				RADIUS = stoi(c);
		else if (b == "PIXELS_PER_STEP")
				PIXELS_PER_STEP = stoi(c);
		else if (b == "START_POSITION")
				START_POSITION = stoi(c);
		else if (b == "FIRST_LINE")
				FIRST_LINE = stoi(c);
		else if (b == "NEXT_LINE")
				NEXT_LINE = stoi(c);
		else if (b == "FIRST_WELL")
				FIRST_WELL = stoi(c);
		else if (b == "NEXT_WELL")
				NEXT_WELL = stoi(c);
		else if (b == "TEXT_X_BEGIN")
				TEXT_X_BEGIN = stoi(c);
		else if (b == "TEXT_Y_BEGIN")
				TEXT_Y_BEGIN = stoi(c);
		else if (b.substr (0,5) == "WELLS"){
				int i = stoi(b.substr (6,8));
				std::string d;
				infile >> d;
				wells.push_back(std::make_pair(stoi(c),stoi(d)));
		}
	}
}

void draw(SDL_Surface *screen, SDL_Surface *bg, SDL_Surface *dropper, TTF_Font *font){
	SDL_Surface *textSurface;
	SDL_Rect dstRect;
	SDL_Rect textLocation;
	SDL_Color foregroundColor = { 255, 255, 255 };
	SDL_Color backgroundColor = { 0, 0, 255 };
	char subtitle[] = "                                       ";

	// Initialize parameters
	textLocation = {(Sint16)TEXT_X_BEGIN, (Sint16)TEXT_Y_BEGIN, 0, 0 };

	dstRect.x = PLATES_X_BEGIN;dstRect.y = y;

	//draw the background sprite
	SDL_BlitSurface(bg, NULL, screen, &dstRect);

	//update the current window
	SDL_UpdateRect(screen, 0, 0, WIDTH, HEIGHT);

	dstRect.x = x;dstRect.y = DROPPER_Y_BEGIN;

	//draw the pin dropper sprite
	SDL_BlitSurface(dropper, NULL, screen, &dstRect);

	sprintf(subtitle, "x: %d , y: %d                   ", x, y);
	textSurface = TTF_RenderText_Shaded(font, subtitle, foregroundColor, backgroundColor);
	SDL_BlitSurface(textSurface, NULL, screen, &textLocation);
	SDL_FreeSurface(textSurface);

	//update the current window
	SDL_UpdateRect(screen, 0, 0, 0, 0);
}

int main(int argc, char* argv[])
{
  SDL_Surface *screen, *blue_dropper, *red_dropper;
  SDL_RWops *rwop;
  SDL_Event event;
  TTF_Font* font;

  //SDL_putenv("SDL_FBDEV=/dev/fb1");
  // Read parameter file
  read_params();

  x = DROPPER_X_BEGIN;
  y = PLATES_Y_BEGIN;

  diry=1;
  dirx=1;

  std::string images_path (dirname(argv[0]));
  images_path += "/images";
  std::string fonts_path (dirname(argv[0]));
  fonts_path += "/fonts";

  // init
  if(SDL_Init(SDL_INIT_VIDEO) != 0){
	fprintf(stderr,"Could not initialize SDL: %s\n",SDL_GetError());
        exit(1);
  }
  SDL_ShowCursor(0);

  TTF_Init();
  font = TTF_OpenFont((fonts_path+"/arial.ttf").c_str(), 12);

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

  // load  dropper images
  rwop=SDL_RWFromFile((images_path + "/blue_dropper.png").c_str(), "rb");
  blue_dropper=IMG_LoadPNG_RW(rwop);
  if(!blue_dropper) {
      printf("IMG_LoadPNG_RW: %s\n", IMG_GetError());
  }

  rwop=SDL_RWFromFile((images_path + "/red_dropper.png").c_str(), "rb");
  red_dropper=IMG_LoadPNG_RW(rwop);
  if(!red_dropper) {
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
		   //if (event.button.button == SDL_BUTTON_LEFT){
		   //}
		   break;

	   case SDL_KEYDOWN:
		   switch(event.key.keysym.sym)
		   {
				case SDLK_ESCAPE:
					quit = true;
					break;
		   }
		   break;
	};
     };
     

     //std::cout << "nums contains " << wells.size() << " elements.\n";
     dropping = false;

     button = read_input();
     if (button || wells.empty()) {
    	 wells.clear();
    	 diry = -1;
         dirx = -1;
    	 backwards();

    	 if (x > DROPPER_X_BEGIN)
    		 x += dirx;

    	 if (y > PLATES_Y_BEGIN)
    	     y += diry;

    	 if (y == PLATES_Y_BEGIN && x == DROPPER_X_BEGIN){
     		read_params();
    	 	diry = 1;
         	dirx = 1;
    	 }
     }
     else if (y < TOTAL_STEPS){
     	//std::cout << "next " << wells.front().first << " " << wells.front().second << "\n";

        if (y == wells.front().second) { 
			if (wells.front().first == x){
            	//std::cout << "y == wells.front().second && wells.front().first == x" << std::endl;
           		wells.erase(wells.begin());
           		dropping = true;
        		drop();
			}
			else {
				if (dirx == 1)
          			right();
        		else
          			left();

       			x += dirx;
			}
        }
		else {
            //std::cout << "forward" << std::endl;
       		forward();
       		y += diry;
		}

       	if (x >= wells.front().first) {
           	//std::cout << "x >= wells.front().first" << std::endl;
           	dirx = -1;
       	}

       	if (x <= wells.front().first){
           	//std::cout << "x <= wells.front().first" << std::endl;
           	dirx = 1;
       	}
     }

     if (dropping)
    	 draw(screen, bg, red_dropper, font);
     else
    	 draw(screen, bg, blue_dropper, font);
  };

  unexport_pins(); 
 
  //free the allocated memory for the background surface
  SDL_FreeSurface(bg);
  SDL_FreeSurface(red_dropper);
  SDL_FreeSurface(blue_dropper);
  TTF_CloseFont(font);

  /* Exit */
  SDL_Quit();
  printf("SDL Shutdown\n");
  /* Done */
  return 0;
}
