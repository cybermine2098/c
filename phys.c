#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

SDL_Window* window;
SDL_Surface* surface;

//Define scales of w and h
#define WIDTH 1600
#define HEIGHT 900
#define WWIDTH 160
#define WHEIGHT 90

//Structures
struct v2d {
  long double x;
  long double y;
};
struct v2di {
  int x;
  int y;
};

struct box{
  struct v2d velocity;
  struct v2d position;
  struct v2d size;
  int mass;
};

uint32_t* pixels;

//Define funcs
struct v2d pixl(int x, int y); 
void pixel(int x, int y, uint32_t color);
uint32_t rgb(uint8_t r, uint8_t g, uint8_t b);
struct v2di snap(struct v2d *mes);
//Global Vars
struct v2d screenScale;
long double XSCALE;
long double YSCALE;


void drawBox(struct box *target){
  struct v2d bSize = target->size;
  struct v2d bPos = target->position;
  struct v2d tmps = pixl(bPos.x-bSize.x,bPos.y-bSize.y);
  struct v2d tmpe = pixl(bPos.x+bSize.x,bPos.y+bSize.y);
  const struct v2di sPos = snap(&tmps);//Starting pixel (top left)
  const struct v2di ePos = snap(&tmpe);//Ending pixel (bottom right)
  for(int x = sPos.x; x < ePos.x; x++){
    for(int y = sPos.y; y < ePos.y; y++){
      pixel(x, y, rgb(255, 255, 255));
    }
  }
}

int main() {
  SDL_Init(SDL_INIT_VIDEO);
  screenScale = pixl(WIDTH, HEIGHT);
  XSCALE = WIDTH/WWIDTH;
  YSCALE = HEIGHT/WHEIGHT;
  printf("Screen scaler: x:%Lf y:%Lf",screenScale.x,screenScale.y);
  window = SDL_CreateWindow("Computing Pi with colliding blocks", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
  surface = SDL_GetWindowSurface(window);
  pixels = (uint32_t*)surface->pixels;

  //Base box
  struct box base;
  base.mass = 1;
  base.position.x = (WWIDTH/2)-10; base.position.y = WHEIGHT/2;
  base.size.x = 1.0;     base.size.y = 1.0;
  base.velocity.x = 0.0; base.velocity.y = 0.0;

  //Incoming box
  struct box initial;
  initial.mass = 1;
  initial.position.x = WWIDTH/2; initial.position.y = WHEIGHT/2;
  initial.size.x = 2.0;     initial.size.y = 2.0;
  initial.velocity.x = 0.0; initial.velocity.y = 0.0;

  bool quit = false;
  SDL_Event e;
  while (!quit) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }
    }

    if (SDL_LockSurface(surface) == 0) {
      memset(pixels, 0, sizeof(uint32_t) * WIDTH * HEIGHT);
      //RENDER START
      drawBox(&base);
      drawBox(&initial);
      //RENDER END
      SDL_UnlockSurface(surface);
    }

    SDL_UpdateWindowSurface(window);
    SDL_Delay(16);
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
  double xres = (double)WIDTH / WWIDTH;
  double yres = (double)HEIGHT / WHEIGHT;
  struct v2d loc = {x * xres, y * yres};
  return loc;
};
struct v2di snap(struct v2d *mes){
  struct v2di re;
  re.x = (int)mes->x;
  re.y = (int)mes->y;
  return re;
}