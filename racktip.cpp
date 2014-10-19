#include "racktip.hpp"

int diry, dirx, x=DROPPER_X_BEGIN, y=PLATES_Y_BEGIN;
int button = 0;
bool dropping = false;
bool quit = false;

bool is_file_exist(const char *fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}

void read_params(){
	std::ifstream infile("coordinates.txt");

	std::string a, b, c;

	while (infile >> a >> b)
	{
		wells.push_back(std::make_pair(stoi(a),stoi(b)));
	}
	if (is_file_exist("./position.txt")){
		read_coordinates();
	}
}

void read_coordinates(){
	std::ifstream input_file("./position.txt");
	input_file >> x;
	input_file >> y;
	std::cout << "read x=" << x << " y=" << y << std::endl;
}

void write_coordinates(){
	std::ofstream output_file("./position.txt");
	output_file << x << " ";
	output_file << y << std::endl;
	std::cout << "write x=" << x << " y=" << y << std::endl;
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

  SDL_putenv("SDL_FBDEV=/dev/fb1");
  // Read parameter file
  read_params();


  diry=PIXELS_PER_STEP;
  dirx=PIXELS_PER_STEP;

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
  //atexit(write_coordinates);
    
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
     std::cout << "wells contains " << wells.size() << " elements.\n" << std::endl;
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
     

     dropping = false;

     button = read_input();
     if (button || wells.empty()) {
    	 wells.clear();
    	 diry = -PIXELS_PER_STEP;
         dirx = -PIXELS_PER_STEP;
    	 backwards();

    	 //if (x > DROPPER_X_BEGIN)
    	//	 x += PIXELS_PER_STEP;

    	 //if (y > PLATES_Y_BEGIN)
    	  //   y += PIXELS_PER_STEP;

    	 //if (y == PLATES_Y_BEGIN && x == DROPPER_X_BEGIN){
     		read_params();
    	 //	diry = PIXELS_PER_STEP;
         //	dirx = PIXELS_PER_STEP;
    	 //}
     }
     else if (y < TOTAL_STEPS){
     	std::cout << "next " << wells.front().first << " " << wells.front().second << std::endl;
        std::cout << "x=" << x << ", y=" << y << std::endl;

        if (y == wells.front().second) { 
		if (wells.front().first == x){
            		std::cout << "y == wells.front().second && wells.front().first == x" << std::endl;
           		wells.erase(wells.begin());
           		dropping = true;
                	std::cout << "drop()" << std::endl;
        		drop();
		}
		else {
			if (dirx == PIXELS_PER_STEP){
                    		std::cout << "right()" << std::endl;
          			right();
                	}
       			else{
                    		std::cout << "left()" << std::endl;
          			left();
                	}

       			x += dirx;
		}
        }
	else if (y > wells.front().second) {
       		std::cout << "backwards()" << std::endl;
       		backwards();
		diry = -PIXELS_PER_STEP;
       		y += diry;
	}
	else {
       		std::cout << "forward()" << std::endl;
       		forward();
		diry = PIXELS_PER_STEP;
       		y += diry;
	}

       	if (x > wells.front().first) {
           	std::cout << "x >= wells.front().first" << std::endl;
           	dirx = -PIXELS_PER_STEP;
       	}

       	if (x < wells.front().first){
           	std::cout << "x <= wells.front().first" << std::endl;
           	dirx = PIXELS_PER_STEP;
       	}
     }
     std::cout << "x=" << x << ", y=" << y << std::endl;
     write_coordinates();

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
