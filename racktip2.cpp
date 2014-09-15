#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <libgen.h>
#include <stdio.h>
//#include <SDL/SDL.h>
//#include <SDL/SDL_image.h>
#include <signal.h>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/gil/extension/io/jpeg_io.hpp>
#include <boost/timer.hpp>

#define IN  0
#define OUT 1

#define LOW  0
#define HIGH 1

std::string exec(std::string cmd) {
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
    	if(fgets(buffer, 128, pipe) != NULL)
    		result += buffer;
    }
    pclose(pipe);
    return result;
}


static int GPIOExport(int pin)
{
#define BUFFER_MAX 3
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/gpio/export", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open export for writing!\n");
		return(-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}

static int GPIOUnexport(int pin)
{
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open unexport for writing!\n");
		return(-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}

static int GPIODirection(int pin, int dir)
{
	static const char s_directions_str[]  = "in\0out";

#define DIRECTION_MAX 35
	char path[DIRECTION_MAX];
	int fd;

	snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio direction for writing!\n");
		return(-1);
	}

	if (-1 == write(fd, &s_directions_str[IN == dir ? 0 : 3], IN == dir ? 2 : 3)) {
		fprintf(stderr, "Failed to set direction!\n");
		return(-1);
	}

	close(fd);
	return(0);
}

static int GPIORead(int pin)
{
#define VALUE_MAX 30
	char path[VALUE_MAX];
	char value_str[3];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_RDONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio value for reading!\n");
		return(-1);
	}

	if (-1 == read(fd, value_str, 3)) {
		fprintf(stderr, "Failed to read value!\n");
		return(-1);
	}

	close(fd);

	return(atoi(value_str));
}

static int GPIOWrite(int pin, int value)
{
	static const char s_values_str[] = "01";

	char path[VALUE_MAX];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio value for writing!\n");
		return(-1);
	}

	if (1 != write(fd, &s_values_str[LOW == value ? 0 : 1], 1)) {
		fprintf(stderr, "Failed to write value!\n");
		return(-1);
	}

	close(fd);
	return(0);
}

void setStep(std::vector<int> gpios_out, int values[4]){
    int j=0;

    for (int &i : gpios_out)
    {
        GPIOWrite(i, values[j]);
        j++;
    } 
}

void forward(std::vector<int> gpios_out, int delay, int steps){
  int values[4][4] = {{1, 0, 1, 0},
                      {1, 0, 0, 1},
                      {0, 1, 0, 1},
                      {0, 1, 1, 0}};

  for (int i = 0; i < steps; i++){
    setStep(gpios_out, values[0]);
    usleep(delay);
    setStep(gpios_out, values[1]);
    usleep(delay);
    setStep(gpios_out, values[2]);
    usleep(delay);
    setStep(gpios_out, values[3]);
    usleep(delay);
  }
}
 
void backwards(std::vector<int> gpios_out, int delay, int steps){  

  int values[4][4] = {{1, 0, 1, 0},
                      {1, 0, 0, 1},
                      {1, 0, 0, 1},
                      {0, 1, 1, 0}};

  for (int i = 0; i < steps; i++){
    setStep(gpios_out, values[0]);
    usleep(delay);
    setStep(gpios_out, values[1]);
    usleep(delay);
    setStep(gpios_out, values[2]);
    usleep(delay);
    setStep(gpios_out, values[3]);
    usleep(delay);
  }
}

void stop(std::vector<int> gpios_out){
  int values[4][4] = {{0, 0, 0, 0}};

  setStep(gpios_out, values[0]);
}
  

//Uint32 get_pixel32( SDL_Surface *surface, int x, int y )
//{
    //Convert the pixels to 32 bit
//    Uint32 *pixels = (Uint32 *)surface->pixels;
    
    //Get the requested pixel
//    return pixels[ ( y * surface->w ) + x ];
//}

//void put_pixel32( SDL_Surface *surface, int x, int y, Uint32 pixel )
//{
    //Convert the pixels to 32 bit
//    Uint32 *pixels = (Uint32 *)surface->pixels;
    
    //Set the pixel
//    pixels[ ( y * surface->w ) + x ] = pixel;
//}

/*
void Write(std::string s1, std::string s2){
    char line[100];
 
    ifstream is(s1.c_str());
    ofstream os(s2.c_str());

    if (is.is_open() && os.is_open()){
       while (!is.eof()){
          is.getline(line,100,'\n');
          os << line << endl;
       }
    }
    else{
       if (!is.is_open()){
          cout << endl << s1 <<" couldn't be opened. Creat and write something in a.txt, and try again." << endl;
       }
       else{
           cout << endl << s2 <<" couldn't be opened. Creat and write something in a.txt, and try again." << endl;
       }
    }
}
*/

int main(int argc, char* argv[])
{
  int button =  0;
  int delay = 1000;
  std::vector<int> gpios_in (1);
  std::vector<int> gpios_out (4);
  gpios_out = {4, 17, 18, 23};
  gpios_in = {22};

  if (argc > 0){
      delay = atoi(argv[1]);
  }

  //SDL_putenv("SDL_FBDEV=/dev/fb1");
  //SDL_putenv("SDL_VIDEODRIVER=fbcon");

  //std::string images_path (dirname(argv[0]));
  //images_path += "/images";

  /* Init */
  //if(SDL_Init(SDL_INIT_VIDEO) != 0)
//	fprintf(stderr,"Could not initialize SDL: %s\n",SDL_GetError()); /* Error? */

  	
  // load support for the JPG and PNG image formats
  //int flags=IMG_INIT_JPG|IMG_INIT_PNG;
  //int initted=IMG_Init(flags);
  //if(initted&flags != flags) {
  //  printf("IMG_Init: Failed to init required jpg and png support!\n");
  //  printf("IMG_Init: %s\n", IMG_GetError());
    // handle error
  //}
  
  //printf("SDL Initialized\n");
  //SDL_Surface *screen;
  //SDL_Rect dstRect;
  //SDL_Surface *webcam, *inter;
  //SDL_RWops *rwop;


  //if( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
  //    fprintf(stderr,
  //            "Couldn't initialize SDL: %s\n", SDL_GetError());
  //    exit(1);
  //}

  /* Clean up on exit */
  //atexit(SDL_Quit);
    
  /*
   * Initialize the display in a 128x160 8-bit palettized mode,
   * requesting a software surface
   */
  //screen = SDL_SetVideoMode(128, 160, 8, SDL_DOUBLEBUF|SDL_SWSURFACE);
  //if ( screen == NULL ) {
  //      fprintf(stderr, "Couldn't set 128x160 video mode: %s\n",
   //                     SDL_GetError());
   //     exit(1);
  //}

  //load the SDL logo bitmap to a temporary surface
  //SDL_Surface* temp = SDL_LoadBMP((images_path + "/background.bmp").c_str());  //PROBLEM LINE
  //if (temp == NULL)   //TEMP KEEPS RETURNING NULL
  //{
  //	printf("Unable to load bitmap: %s\n", SDL_GetError());
  //	return 1;
  //};
  
  //create the working SDL_Surface which matches the display format
  //of the temporary surface
  //SDL_Surface* bg = SDL_DisplayFormat(temp);
  //SDL_FreeSurface(temp);

  // load  webcam image
  //rwop=SDL_RWFromFile((images_path + "/webcam.jpg").c_str(), "rb");
  //webcam=IMG_LoadJPG_RW(rwop);
  //if(!webcam) {
   //    	printf("IMG_LoadJPG_RW: %s\n", IMG_GetError());
  //} 

  //Write(s2,s3);

  //SDL_Event event;
  bool quit = false;
  int y=0;
  int x=0;
  for (int &i : gpios_out)
  {
	GPIOExport(i);
  }	 
  for (int &i : gpios_out)
  {
	GPIODirection(i,OUT);
  }	 
  for (int &i : gpios_in)
  {
	GPIOExport(i);
  }	 
  for (int &i : gpios_in)
  {
	GPIODirection(i,OUT);
  }
  
  //This is the main message loop of the game
  while(!quit)
  {
     // load interruptor image
     //rwop=SDL_RWFromFile(images_path + "interruptor.jpg"), "rb");
     //inter=IMG_LoadJPG_RW(rwop);
     //if(!inter) {
     //   printf("IMG_LoadJPG_RW: %s\n", IMG_GetError());
        // handle error
     //}  

     //check the message queue for an event
     /*if(SDL_PollEvent(&event))
     {
  	switch(event.type)
  	{
  	   case SDL_QUIT:
  	   	quit = true;
             	break;

           case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT){
    			SDL_LockSurface(webcam);
			Uint32 rawpixel = get_pixel32( webcam, event.button.x, event.button.y);
			Uint8 red, green, blue;
			SDL_GetRGB(rawpixel, webcam->format, &red, &green, &blue);
			printf("Pixel pressed is %u: %d, %d, %d!\n", rawpixel, red, green, blue); 
    			SDL_UnlockSurface(webcam);
		}
		break;

  	   case SDL_KEYDOWN:
		
  		switch(event.key.keysym.sym)
  		{
  			case SDLK_ESCAPE:
  				quit = true;
  				break;
  			case SDLK_UP:
           		    for (int &i : gpios_out)
      			    {
        			const std::string cmd = "echo \"1\" > /sys/class/gpio/gpio" + boost::lexical_cast<std::string>(i) + "/value";
        			//std::clog << cmd << '\n';
        			const int error = std::system(cmd.c_str());
        			assert(!error);
      			    } 
  			    x = x + 1;
  			    break;
  			case SDLK_DOWN:
           		    for (int &i : gpios_out)
      			    {
        			const std::string cmd = "echo \"0\" > /sys/class/gpio/gpio" + boost::lexical_cast<std::string>(i) + "/value";
        			//std::clog << cmd << '\n';
        			const int error = std::system(cmd.c_str());
        			assert(!error);
      			    }
  			    y = y + 1;
  			    break;
  		}
                
  		break;
  	};

     };
*/
     
     button = GPIORead(22); 
     if (button) {
        forward(gpios_out, delay, 10);
     	// load  webcam image
     	//rwop=SDL_RWFromFile((images_path + "/webcam.jpg").c_str(), "rb");
     	//webcam=IMG_LoadJPG_RW(rwop);
     	//if(!webcam) {
//        	printf("IMG_LoadJPG_RW: %s\n", IMG_GetError());
        	// handle error
     	//}  
     }
     else {
	stop(gpios_out);
     }
  
     //draw the background sprite
     //SDL_BlitSurface(bg, NULL, screen, NULL);

     //dstRect.x = x;
     //dstRect.y = y;

     //SDL_BlitSurface(webcam, NULL, screen, &dstRect);
     //SDL_BlitSurface(inter, NULL, screen, &dstRect);
  
     //update the current window
     //SDL_UpdateRect(screen, 0, 0, 0, 0);
  };

  for (int &i : gpios_in)
  {
	GPIOUnexport(i);
  }	 

  for (int &i : gpios_in)
  {
	GPIOUnexport(i);
  }
  
  //free the allocated memory for the background surface
  //SDL_FreeSurface(bg);
  //SDL_Delay(2000);

  /* Exit */
  //SDL_Quit();
  //printf("SDL Shutdown\n");
  /* Done */
  return 0;
}