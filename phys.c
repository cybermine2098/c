#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

SDL_Window* window;
SDL_Surface* surface;

#define WIDTH 1600
#define HEIGHT 900
//Define scales of w and h
#define YSCALE 90
#define XSCALE 160

struct v2d {
  long double x;
  long double y;
};

struct box{
  struct v2d velocity;
  struct v2d position;
  struct v2d size;
  int mass;
};

uint32_t* pixels;

struct v2d pixl(int x, int y); 
void pixel(int x, int y, uint32_t color);
uint32_t rgb(uint8_t r, uint8_t g, uint8_t b);

void drawBox(struct box *target){
  struct v2d *bSize = &(target->size);
  int pix = (int)((bSize->x * 2)*(bSize->y*2));
  struct v2d midScreen = pix(XSCALE,YSCALE);
  printf("pixels:%i",pix);
  
}

int main() {
  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow("Computing Pi with colliding blocks", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
  surface = SDL_GetWindowSurface(window);
  pixels = (uint32_t*)surface->pixels;

  //Base box
  struct box base;
  base.mass = 1;
  base.position.x = 4.0; base.position.y = 0.0;
  base.size.x = 1.0;     base.size.y = 1.0;
  base.velocity.x = 0.0; base.velocity.y = 0.0;

  //Incoming box
  struct box initial;
  initial.mass = 1;
  initial.position.x = 10.0; initial.position.y = 0.0;
  initial.size.x = 2.0;     initial.size.y = 2.0;
  initial.velocity.x = 0.0; initial.velocity.y = 0.0;

  bool quit = false;
  SDL_Event e;
  while (!quit) {
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            quit = true;
        }
        SDL_LockSurface(surface);
        memset(pixels, 0, sizeof(uint32_t) * WIDTH * HEIGHT);

        for (int y = 0; y < 255; y++) {
            for (int x = 0; x < 255; x++) {
                pixel(x+100, y+100, rgb(x, 0, y));
            }
        }

        SDL_UnlockSurface(surface);
    }
    SDL_UpdateWindowSurface(window);
  }
  
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void pixel(int x, int y, uint32_t color) {
  if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;
  pixels[y * WIDTH + x] = color;
};
uint32_t rgb(uint8_t r, uint8_t g, uint8_t b) {
 return (uint32_t)SDL_MapRGB(surface->format, r, g, b);
};
struct v2d pixl(int x, int y){
  double xres = XSCALE/(double)WIDTH;
  double yres = YSCALE/(double)HEIGHT;
  struct v2d loc = {x*(1/xres),y*(1/yres)};
  return loc;
};