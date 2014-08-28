#include <cassert>
#include <iostream>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <SDL/SDL.h>
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

int main()
{

  SDL_putenv("SDL_FBDEV=/dev/fb1");
  SDL_putenv("SDL_VIDEODRIVER=fbcon");
  /* Init */
  if(SDL_Init(SDL_INIT_VIDEO) != 0)
	fprintf(stderr,"Could not initialize SDL: %s\n",SDL_GetError()); /* Error? */
  printf("SDL Initialized\n");
  SDL_Surface *screen;
  SDL_Rect dstRect;

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
  SDL_Surface* temp = SDL_LoadBMP("images/background.bmp");  //PROBLEM LINE
  if (temp == NULL)   //TEMP KEEPS RETURNING NULL
  {
  	printf("Unable to load bitmap: %s\n", SDL_GetError());
  	return 1;
  };
  
  //create the working SDL_Surface which matches the display format
  //of the temporary surface
  SDL_Surface* bg = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);

  temp = SDL_LoadBMP("images/ball.bmp");  //PROBLEM LINE
  if (temp == NULL)   //TEMP KEEPS RETURNING NULL
  {
  	printf("Unable to load bitmap: %s\n", SDL_GetError());
  	return 1;
  };

  SDL_Surface* ball = SDL_DisplayFormat(temp);
  
  //free memory allocated to the temp SDL_Surface
  SDL_FreeSurface(temp);
  
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
 
     /*
     if (button) {
     	for (int &i : gpios_out)
     	{
     		const std::string cmd = "echo \"1\" > /sys/class/gpio/gpio" + boost::lexical_cast<std::string>(i) + "/value";
       		//std::clog << cmd << '\n';
       		const int error = std::system(cmd.c_str());
       		assert(!error);
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
     */
  
     //draw the background sprite
     SDL_BlitSurface(bg, NULL, screen, NULL);
     dstRect.x = x;
     dstRect.y = y;

     SDL_BlitSurface(ball, NULL, screen, &dstRect);
  
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
