#include <cassert>
#include <iostream>
#include <fstream>
#include <iterator>
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
std::vector<int> gpios_step = {1};
std::vector<int> gpios_forward = {0, 0, 1, 0};
std::vector<int> gpios_backwards = {0, 0, 1, 1};
std::vector<int> gpios_right = {1, 0, 0, 1};
std::vector<int> gpios_left = {1, 0, 0, 0};
std::vector<int> gpios_stop = {0, 0};
std::vector<int> gpios_drop = {0, 1,};

std::vector<std::pair<int, int>> wells;

int HEIGHT=160;
int WIDTH=128;
int MOVE_DELAY=1000000;
int DROP_DELAY=1000000;
int STEPS=1;
int TOTAL_STEPS=160;
int PLATES_X_BEGIN=0;
int PLATES_Y_BEGIN=-120;
int PLATE_Y_MOVE=6;
int DROPPER_X_BEGIN=12;
int DROPPER_X_END=120;
int DROPPER_Y_BEGIN=30;
int DROPPER_X_MOVE=1;
int RADIUS=5;
int PIXELS_PER_STEP=6;
int START_POSITION=100;
int FIRST_LINE=-120;
int NEXT_LINE=6;
int FIRST_WELL=13;
int NEXT_WELL=6;
int TEXT_X_BEGIN=0;
int TEXT_Y_BEGIN=147;


bool is_file_exist(const char *fileName);
void read_params();
void draw(SDL_Surface *screen, SDL_Surface *bg, SDL_Surface *dropper, TTF_Font *font);
void read_coordinates();
void write_coordinates();
