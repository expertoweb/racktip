#include <cmath>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

void set_pixel(SDL_Surface *surface, int x, int y, Uint8 pixel);
void draw_circle(SDL_Surface *surface, int n_cx, int n_cy, int radius, Uint8 pixel);
void fill_circle(SDL_Surface *surface, int cx, int cy, int radius, Uint8 pixel);
