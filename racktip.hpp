#include <cassert>
#include <iostream>
#include <libgen.h>
#include <vector>
#include <utility>
#include <string>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "gpio.hpp"
#include "commands.hpp"

std::vector<int> gpios_in = {22};
std::vector<int> gpios_out = {4, 17, 18, 23};
std::vector<int> gpios_first = {1, 1, 0, 1};
std::vector<int> gpios_second = {1, 0, 1, 1};
std::vector<int> gpios_drop = {0, 0, 0, 0};

std::vector<std::pair<int, int>> wells;

int HEIGHT;
int WIDTH;            
int DELAY;            
int PLATE_LINE_STEPS;  
int TOTAL_STEPS;       
int PLATES_X_BEGIN;    
int PLATES_Y_BEGIN;    
int PLATE_Y_MOVE;      
int DROPPER_X_BEGIN;   
int DROPPER_X_END;     
int DROPPER_Y_BEGIN;   
int DROPPER_X_MOVE;    
int RADIUS;            
int STEPS_PER_PIXEL;   
int START_POSITION;    
int FIRST_LINE;        
int NEXT_LINE;         
int FIRST_WELL;        
int NEXT_WELL;         
int TEXT_X_BEGIN;      
int TEXT_Y_BEGIN;      

