#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <libgen.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <signal.h>
#include "gpio.hpp"

void setStep(std::vector<int> gpios_out, int values[4]){
    int j=0;

    for (int &i : gpios_out)
    {
        GPIOWrite(i, values[j]);
        j++;
    } 
}

void forward(std::vector<int> gpios_out, int delay, int steps){
  int values[2][4] = {{1, 0, 1, 1},
                      {1, 1, 0, 1}};

    int j=0;
    for (j=0; j<2; j++){
    	setStep(gpios_out, values[j]);
    	usleep(delay);
    }
}
 
void stop(std::vector<int> gpios_out){
  int values[4][4] = {{0, 0, 0, 0}};

  setStep(gpios_out, values[0]);
}

void Write(std::string s1, std::string s2){
    char line[100];
 
    std::ifstream is(s1.c_str());
    std::ofstream os(s2.c_str());

    if (is.is_open() && os.is_open()){
       while (!is.eof()){
          is.getline(line,100,'\n');
          os << line << std::endl;
       }
    }
    else{
       if (!is.is_open()){
          std::cout << std::endl << s1 <<" couldn't be opened. Creat and write something in a.txt, and try again." << std::endl;
       }
       else{
           std::cout << std::endl << s2 <<" couldn't be opened. Creat and write something in a.txt, and try again." << std::endl;
       }
    }
}

int main(int argc, char* argv[])
{
  int button =  1;
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
     
     //button = GPIORead(22); 
     if (button) {
        forward(gpios_out, delay, 100);
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
