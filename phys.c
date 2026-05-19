#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <time.h>

SDL_Window* window;
SDL_Surface* surface;

//Definitons. read the readme please.
#define WIDTH 1920 //Keeping these next 4 in ratios of each other is ideal!
#define HEIGHT 1080
#define WWIDTH 96
#define WHEIGHT 54 
#define RENDER_FRAMERATE 60 //Visual framerate
#define PHYSICS_SUBSTEPS 50 //Physics steps per render frame, increase for higher mass ratios
#define DIGITS 1
#define SPEED 5
#define RENDERGRID 1
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
  struct v2d velocity;//Stored as workplace units / second
  struct v2d position;//stored as workplace units
  struct v2d size;//stored as workplace units
  int mass;// stored as kilograms
};

uint32_t* pixels;

//Rendering functions
struct v2di pixl(long double x, long double y); 
void pixel(int x, int y, uint32_t color);
uint32_t rgb(uint8_t r, uint8_t g, uint8_t b);
void renderUnitGrid();

//Struct functions
struct v2di snap(struct v2d *mes);

//Movement and physics functions
void step(struct box *box1, struct box *box2);//Physics collider
void move(struct box *b);

//Global Vars
long double XSCALE;
long double YSCALE;
int coll;
bool quit;


void drawBox(struct box *target){
  struct v2d bSize = target->size;
  struct v2d bPos = target->position;
  const struct v2di sPos = pixl(bPos.x-bSize.x,bPos.y-bSize.y);
  const struct v2di ePos = pixl(bPos.x+bSize.x,bPos.y+bSize.y);
  for(int x = sPos.x; x < ePos.x; x++){
    for(int y = sPos.y; y < ePos.y; y++){
      pixel(x, y, rgb(255, 255, 255));
    }
  }
}
void drawStraightLine(struct v2di start, struct v2di end, uint32_t color) {
  int dx = abs(end.x - start.x);
  int dy = abs(end.y - start.y);
  int sx = start.x < end.x ? 1 : -1;
  int sy = start.y < end.y ? 1 : -1;
  int err = dx - dy;

  int x = start.x, y = start.y;
  while (1) {
    pixel(x, y, color);
    if (x == end.x && y == end.y) break;
    int e2 = 2 * err;
    if (e2 > -dy) err -= dy, x += sx;
    if (e2 < dx) err += dx, y += sy;
  }
}
int main() {
  SDL_Init(SDL_INIT_VIDEO);
  XSCALE = WIDTH/WWIDTH;
  YSCALE = HEIGHT/WHEIGHT;
  window = SDL_CreateWindow("Computing Pi with colliding blocks", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
  surface = SDL_GetWindowSurface(window);
  pixels = (uint32_t*)surface->pixels;

  //Base box
  struct box base;
  base.mass = 1;
  base.position.x = (WWIDTH/2)-5; base.position.y = WHEIGHT/2;
  base.size.x = 1.0;     base.size.y = 1.0;
  base.velocity.x = 0.0; base.velocity.y = 0.0;

  //Incoming box
  struct box initial;
  const int setMass = pow(100,(DIGITS-1));
  printf("Using block mass: %i\n",setMass);
  initial.mass = setMass;
  initial.position.x = WWIDTH/2; initial.position.y = (WHEIGHT/2)-0.5;
  initial.size.x = 1.5;     initial.size.y = 1.5;
  initial.velocity.x = -SPEED; initial.velocity.y = 0.0;
  quit = false;
  int frame = 0;
  SDL_Event e;
  while (!quit) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }
    }
    if (SDL_LockSurface(surface) == 0) {
      memset(pixels, 0, sizeof(uint32_t) * WIDTH * HEIGHT);
      //render START
      RENDERGRID ? renderUnitGrid() : 0;
      drawBox(&base);
      drawBox(&initial);
      //RENDER END
      SDL_UnlockSurface(surface);
    }
    SDL_UpdateWindowSurface(window);
    SDL_Delay(round((1000.0/RENDER_FRAMERATE)));
    frame++;//Increment frame counter;
    // printf("Finished frame %i\n",frame);
    //Physics simulation START
    for(int substep = 0; substep < PHYSICS_SUBSTEPS; substep++){
      step(&base,&initial);
    }
  }
  SDL_DestroyWindow(window);
  SDL_Quit();
  printf("Finished! %i collisions",coll);
}
double distance(struct v2d *pos1, struct v2d *pos2){
  //Pythagorean Theorum.
  const double dx = fabs(pos1->x-pos2->x);
  const double dy = fabs(pos1->y-pos2->y);
  return (sqrt((dx*dx)+(dy*dy)));
}
void renderUnitGrid(){
  for(int col = 0; col < WWIDTH;col++){
    struct v2di s = {col*XSCALE,0.0};
    struct v2di e = {col*XSCALE,HEIGHT};
    drawStraightLine(s,e,rgb(255,0,0));
  }
  for(int row = 0; row < WHEIGHT; row++){
    struct v2di s = {0.0,row*YSCALE}; 
    struct v2di e = {WIDTH,row*YSCALE};
    drawStraightLine(s,e,rgb(255,0,0));
  }
}
void pixel(int x, int y, uint32_t color) {
  if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;
  pixels[y * WIDTH + x] = color;
};
uint32_t rgb(uint8_t r, uint8_t g, uint8_t b) {
 return (uint32_t)SDL_MapRGB(surface->format, r, g, b);
};
struct v2di pixl(long double x, long double y){
  long double xres = (long double)WIDTH / WWIDTH;
  long double yres = (long double)HEIGHT / WHEIGHT;
  struct v2di loc = {round(x * xres), round(y * yres)};
  return loc;
};

struct v2di snap(struct v2d *mes){
  struct v2di re;
  re.x = (int)mes->x;
  re.y = (int)mes->y;
  return re;
}

void step(struct box *box1, struct box *box2){
  //Step 1: See if any collisions need to be made -- radial cast.
  if(distance(&(box1->position),&(box2->position)) <= box1->size.x+box2->size.x){
    //Elastic collision: exchange velocities based on mass
    const double v1 = box1->velocity.x;
    const double v2 = box2->velocity.x;
    const int m1 = box1->mass;
    const int m2 = box2->mass;
    
    //Elastic collision formulas
    box1->velocity.x = ((m1 - m2) * v1 + 2 * m2 * v2) / (m1 + m2);
    box2->velocity.x = ((m2 - m1) * v2 + 2 * m1 * v1) / (m1 + m2);
    coll++;
  }
  move(box1);
  move(box2);
  if(box1->velocity.x >= 0 && box1->velocity.x < box2->velocity.x && box2->velocity.x >= 0){//win condition
    quit = true;
  }
}
void move(struct box *b){
  const long double physics_fps = (long double)RENDER_FRAMERATE * PHYSICS_SUBSTEPS;
  const long double xscaled = (b->velocity.x)/physics_fps;
  const long double yscaled = (b->velocity.y)/physics_fps;
  b->position.x += xscaled;
  b->position.y += yscaled;
  if(b->position.x-b->size.x <= 0){//bounce off the left side of the screen.
    b->velocity.x = -(b->velocity.x);
    coll++;
  }
}