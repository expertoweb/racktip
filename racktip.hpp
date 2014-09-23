#include <cassert>
#include <iostream>
#include <libgen.h>
#include <vector>
#include <string>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "gpio.hpp"
#include "commands.hpp"
#include "constants.hpp"

std::vector<int> gpios_in = {22};
std::vector<int> gpios_out = {4, 17, 18, 23};
std::vector<int> gpios_first = {1, 1, 0, 1};
std::vector<int> gpios_second = {1, 0, 1, 1};
std::vector<int> gpios_stop = {0, 0, 0, 0};
