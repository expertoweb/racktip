#include "racktip.hpp"

int diry, dirx, x=DROPPER_X_BEGIN, y=PLATES_Y_BEGIN;
int button = 0;
bool dropping = false;
bool running = false;
bool quit = false;
bool left_pressed=false; 
bool right_pressed=false; 
bool up_pressed=false;
bool down_pressed=false;

bool is_file_exist(const char *fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}

void read_coordinates(){
	std::ifstream infile("coordinates.txt");

	std::string a, b, c;

	while (infile >> a >> b)
	{
		wells.push_back(std::make_pair(stoi(a),stoi(b)));
	}
}

void read_position(){
	if (is_file_exist("position.txt")){
		std::ifstream input_file("./position.txt");
		input_file >> x;
		input_file >> y;
		std::cout << "read x=" << x << " y=" << y << std::endl;
	}
}

void write_position(){
	std::ofstream output_file("./position.txt");
	output_file << x << " ";
	output_file << y << std::endl;
	std::cout << "write x=" << x << " y=" << y << std::endl;
}

void draw(SDL_Surface *screen, SDL_Surface *bg, SDL_Surface *dropper, SDL_Surface *arrows, TTF_Font *font){
	SDL_Surface *textSurface;
	SDL_Rect dstRect;
	SDL_Rect textLocation;
	SDL_Color foregroundColor = { 255, 255, 255 };
	SDL_Color backgroundColor = { 0, 0, 255 };
	char subtitle[] = "                                       ";

	// Initialize parameters
	textLocation = {(Sint16)TEXT_X_BEGIN, (Sint16)TEXT_Y_BEGIN, 0, 0 };

 	SDL_FillRect(screen, NULL, 0xFF00FF00);
	dstRect.x = PLATES_X_BEGIN;dstRect.y = y;
	//draw the background sprite
	SDL_BlitSurface(bg, NULL, screen, &dstRect);


	if (arrows != NULL) {
		dstRect.x = ARROWS_X_BEGIN; dstRect.y = ARROWS_Y_BEGIN;
		SDL_BlitSurface(arrows, NULL, screen, &dstRect);
		sprintf(subtitle, "ENTER start, ARROWS calibrate");
	}
	else {
		//update the current window
		sprintf(subtitle, "x: %d , y: %d                   ", x, y);

	}
	//draw the pin dropper sprite
	if (dropper != NULL){
		dstRect.x = x;dstRect.y = DROPPER_Y_BEGIN;
		SDL_BlitSurface(dropper, NULL, screen, &dstRect);
	}
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
  SDL_EnableKeyRepeat(0, 0);

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

  //load the arrow bitmap to a temporary surface
  temp = SDL_LoadBMP((images_path + "/arrows.bmp").c_str());
  if (temp == NULL)   //TEMP KEEPS RETURNING NULL
  {
      printf("Unable to load bitmap: %s\n", SDL_GetError());
      exit(1);
  };
  
  //create the working SDL_Surface which matches the display format
  //of the temporary surface
  SDL_Surface* arrows = SDL_DisplayFormat(temp);
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

  read_position();

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

	     case SDL_KEYDOWN:
		   if (left_pressed || right_pressed || down_pressed || up_pressed){
			running = false;
			left_pressed=false; 
			right_pressed=false; 
			up_pressed=false;
			down_pressed=false;
			stop();
		   	break;
		   }
		   switch(event.key.keysym.sym)
		   {
			case SDLK_ESCAPE:
				quit = true;
				break;
			case SDLK_LEFT:
				running = false;
				x=DROPPER_X_BEGIN; 
				y=PLATES_Y_BEGIN;
          			left(1);
				left_pressed=true;

				break;
			case SDLK_RIGHT:
				running = false;
				x=DROPPER_X_BEGIN; 
				y=PLATES_Y_BEGIN;
          			right(1);
				right_pressed=true;

				break;
			case SDLK_UP:
				running = false;
				x=DROPPER_X_BEGIN; 
				y=PLATES_Y_BEGIN;
          			backwards(1);
				up_pressed=true;
				
				break;
			case SDLK_DOWN:
				running = false;
				x=DROPPER_X_BEGIN; 
				y=PLATES_Y_BEGIN;
          			forward(1);
				down_pressed=true;

				break;
			case SDLK_RETURN:
				if(wells.empty()){
					wells.clear();
     					read_coordinates();
				}
				running = true;
				
				break;
		   }
		   break;
	}
     }
     


     if (running && !wells.empty()){
        std::cout << wells.size() << " wells left" << std::endl;
        std::cout << "x=" << x << ", y=" << y << std::endl;
     	std::cout << "next: " << wells.front().first << " " << wells.front().second << std::endl;


        if (y == wells.front().second) { 
		if (wells.front().first == x){
            		std::cout << "y == wells.front().second && wells.front().first == x" << std::endl;
           		wells.erase(wells.begin());
           		dropping = true;
     			draw(screen, bg, red_dropper, NULL, font);
                	std::cout << "drop()" << std::endl;
        		drop();
		}
		else {
			dropping = false;
     			draw(screen, bg, blue_dropper, NULL, font);
			if (dirx == 1){
                    		std::cout << "right(" << (wells.front().first - x)  << ")" << std::endl;
          			right(wells.front().first - x);
				stop();
                	}
       			else{
                    		std::cout << "left(" << (x - wells.front().first)  << ")" << std::endl;
          			left(x - wells.front().first);
				stop();
                	}
       			x = wells.front().first;
		}
        }
	else if (y > wells.front().second) {
       		std::cout << "backwards(" << (y - wells.front().second) << ")" << std::endl;
       		backwards(y - wells.front().second);
		stop();
		diry = -1;
	}
	else {
       		std::cout << "forward(" << (wells.front().second - y) << ")" << std::endl;
       		forward(wells.front().second - y);
		stop();
		diry = 1;
	}
       	y = wells.front().second;

       	if (x > wells.front().first) {
           	std::cout << "x >= wells.front().first" << std::endl;
           	dirx = -1;
       	}

       	if (x < wells.front().first){
           	std::cout << "x <= wells.front().first" << std::endl;
           	dirx = 1;
       	}

     	write_position();
     }
     else { 
     	draw(screen, bg, blue_dropper, arrows, font);
     }	
  }

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
