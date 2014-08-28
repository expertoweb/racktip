#include <cassert>
#include <iostream>
#include <vector>
#include <string.h>
#include <libgen.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/timer.hpp>

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

Uint32 get_pixel32( SDL_Surface *surface, int x, int y )
{
    //Convert the pixels to 32 bit
    Uint32 *pixels = (Uint32 *)surface->pixels;
    
    //Get the requested pixel
    return pixels[ ( y * surface->w ) + x ];
}

void put_pixel32( SDL_Surface *surface, int x, int y, Uint32 pixel )
{
    //Convert the pixels to 32 bit
    Uint32 *pixels = (Uint32 *)surface->pixels;
    
    //Set the pixel
    pixels[ ( y * surface->w ) + x ] = pixel;
}

int main(int argc, char* argv[])
{

  SDL_putenv("SDL_FBDEV=/dev/fb1");
  SDL_putenv("SDL_VIDEODRIVER=fbcon");

  std::string images_path (dirname(argv[0]));
  images_path += "/images";

  /* Init */
  if(SDL_Init(SDL_INIT_VIDEO) != 0)
	fprintf(stderr,"Could not initialize SDL: %s\n",SDL_GetError()); /* Error? */

  	
  // load support for the JPG and PNG image formats
  int flags=IMG_INIT_JPG|IMG_INIT_PNG;
  int initted=IMG_Init(flags);
  if(initted&flags != flags) {
    printf("IMG_Init: Failed to init required jpg and png support!\n");
    printf("IMG_Init: %s\n", IMG_GetError());
    // handle error
  }

  
  printf("SDL Initialized\n");
  SDL_Surface *screen;
  SDL_Rect dstRect;
  SDL_Surface *webcam, *inter;
  SDL_RWops *rwop;
  

  int button =  0;
  std::vector<int> gpios_out (1);
  gpios_out = {4};
  std::vector<int> gpios_in (1);
  gpios_in = {22};

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
  screen = SDL_SetVideoMode(128, 160, 8, SDL_DOUBLEBUF|SDL_SWSURFACE);
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
  	return 1;
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
       	// handle error
  } 

  SDL_Event event;
  bool quit = false;
  int x,y;

  x = y = 0;

  for (int &i : gpios_out)
  {
    	const std::string cmd = "echo \"" + boost::lexical_cast<std::string>(i) + "\" > /sys/class/gpio/export";
    	//std::clog << cmd << '\n';
    	const int error = std::system(cmd.c_str());
    	assert(!error);
  }	 
  for (int &i : gpios_out)
  {
        const std::string cmd = "echo \"out\" > /sys/class/gpio/gpio" + boost::lexical_cast<std::string>(i) + "/direction";
        //std::clog << cmd << '\n';
	const int error = std::system(cmd.c_str());
	assert(!error);
  }
  for (int &i : gpios_in)
  {
    	const std::string cmd = "echo \"" + boost::lexical_cast<std::string>(i) + "\" > /sys/class/gpio/export";
    	//std::clog << cmd << '\n';
    	const int error = std::system(cmd.c_str());
   	assert(!error);
  }	 
  for (int &i : gpios_in)
  {
        const std::string cmd = "echo \"in\" > /sys/class/gpio/gpio" + boost::lexical_cast<std::string>(i) + "/direction";
        //std::clog << cmd << '\n';
	const int error = std::system(cmd.c_str());
	assert(!error);
  }
  
  //This is the main message loop of the game
  while(!quit)
  {
     for (int &i : gpios_in)
     {
          const std::string cmd = "cat /sys/class/gpio/gpio" + boost::lexical_cast<std::string>(i) + "/value";
	  const std::string output = exec(cmd.c_str());
          //std::clog << output << '\n';
          button = atoi(output.c_str());
     }


     // load interruptor image
     //rwop=SDL_RWFromFile(images_path + "interruptor.jpg"), "rb");
     //inter=IMG_LoadJPG_RW(rwop);
     //if(!inter) {
     //   printf("IMG_LoadJPG_RW: %s\n", IMG_GetError());
        // handle error
     //}  

     //check the message queue for an event
     if(SDL_PollEvent(&event))
     {
  	//if an event was found
  	switch(event.type)
  	{
  	   //check to see if the window was closed via the "X"
  	   case SDL_QUIT:
  	   	//set the quit flag to true
  	   	quit = true;
             	break;

           /* If a button on the mouse is pressed. */
           case SDL_MOUSEBUTTONDOWN:
                /* If the left button was pressed. */
                if (event.button.button == SDL_BUTTON_LEFT){
    			SDL_LockSurface(webcam);
			Uint32 rawpixel = get_pixel32( webcam, event.button.x, event.button.y);
			Uint8 red, green, blue;
			SDL_GetRGB(rawpixel, webcam->format, &red, &green, &blue);
			printf("Pixel pressed is %d, %d, %d!\n", red, green, blue); 
    			SDL_UnlockSurface(webcam);
		}
		break;

  	   //check to see if the ESC key was pressed
  	   case SDL_KEYDOWN:
		
  		switch(event.key.keysym.sym)
  		{
  			case SDLK_ESCAPE:
  				//set quit flag to true
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
 
     if (button) {
     	for (int &i : gpios_out)
     	{
     		const std::string cmd = "echo \"1\" > /sys/class/gpio/gpio" + boost::lexical_cast<std::string>(i) + "/value";
       		//std::clog << cmd << '\n';
       		const int error = std::system(cmd.c_str());
       		assert(!error);
     	} 
        
     	// load  webcam image
     	rwop=SDL_RWFromFile((images_path + "/webcam.jpg").c_str(), "rb");
     	webcam=IMG_LoadJPG_RW(rwop);
     	if(!webcam) {
        	printf("IMG_LoadJPG_RW: %s\n", IMG_GetError());
        	// handle error
     	}  
     }
     else {
     	for (int &i : gpios_out)
     	{
     		const std::string cmd = "echo \"0\" > /sys/class/gpio/gpio" + boost::lexical_cast<std::string>(i) + "/value";
       		//std::clog << cmd << '\n';
       		const int error = std::system(cmd.c_str());
     		assert(!error);
     	}
     }
  
     //draw the background sprite
     //SDL_BlitSurface(bg, NULL, screen, NULL);

     dstRect.x = x;
     dstRect.y = y;

     SDL_BlitSurface(webcam, NULL, screen, &dstRect);
     SDL_BlitSurface(inter, NULL, screen, &dstRect);
  
     //update the current window
     SDL_UpdateRect(screen, 0, 0, 0, 0);
  };

  for (int &i : gpios_out)
  {
      	    const std::string cmd = "echo \"" + boost::lexical_cast<std::string>(i) + "\" > /sys/class/gpio/unexport";
       	    //std::clog << cmd << '\n';
       	    const int error = std::system(cmd.c_str());
       	    assert(!error);
  }

  for (int &i : gpios_in)
  {
      	    const std::string cmd = "echo \"" + boost::lexical_cast<std::string>(i) + "\" > /sys/class/gpio/unexport";
       	    //std::clog << cmd << '\n';
       	    const int error = std::system(cmd.c_str());
       	    assert(!error);
  }
  
  //free the allocated memory for the background surface
  SDL_FreeSurface(bg);

  /* Exit */
  SDL_Quit();
  printf("SDL Shutdown\n");
  /* Done */
  return 0;
}
