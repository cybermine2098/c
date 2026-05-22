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
//These are now loaded from config.txt
int WIDTH;           //Keeping these next 4 in ratios of each other is ideal! (8s=w)
int HEIGHT;
int WWIDTH;
int WHEIGHT;
int RENDER_FRAMERATE;
int PHYSICS_SUBSTEPS;
int DIGITS;
int SPEED;
int RENDERGRID;
//Structures
struct v2d { // Vector 2d, used for all sorts of 2d values.
  long double x;
  long double y;
};
struct v2di { // Vector 2d integer. Mostly used for pixels.
  int x;
  int y;
};

struct box{// Box struct, self explanatory
  struct v2d velocity;//Stored as workplace units / second
  struct v2d position;//stored as workplace units
  struct v2d size;//stored as workplace units
  long int mass;// stored as kilograms (long int for huge number calculations.)
  uint32_t color;// Stores the color of the box :)
};

void setAttributes();
//Rendering functions
struct v2di pixl(long double x, long double y); //outputs a pixel from a workspace coordinate
void pixel(int x, int y, uint32_t color);// Draws a pixel onscreen
uint32_t rgb(uint8_t r, uint8_t g, uint8_t b);//converts r g b to a uint32 for sdl.
void renderUnitGrid();//Function to actually render the unit grid.
void drawBox(struct box *target);//Self explanatory
void drawStraightLine(struct v2di start, struct v2di end, uint32_t color);//Also self explanatory

//Movement and physics functions
void step(struct box *box1, struct box *box2);//Physics collider
void move(struct box *b);//

//Global Vars
long double XSCALE;
long double YSCALE;

uint32_t* pixels;
int coll;
bool quit;
int frame;
int done;

//BEGIN PROGRAM EXEC
int main() {
  //First read all the configs from the config file!
  setAttributes();
  SDL_Init(SDL_INIT_VIDEO);//Open a video channel.
  XSCALE = WIDTH/WWIDTH;//Calculate X and Y scale variables.
  YSCALE = HEIGHT/WHEIGHT;
  window = SDL_CreateWindow("Computing Pi with colliding blocks", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);//Create a video
  surface = SDL_GetWindowSurface(window);//And be able to write to it.
  pixels = (uint32_t*)surface->pixels;//Set the pixels to a pointer of the surface pixels. 

  //Create and format boxes.

  //Base box
  struct box base;
  base.mass = 1;
  base.position.x = round(WWIDTH/4);  base.position.y = WHEIGHT-2;
  base.size.x = 2.0;     base.size.y = 2.0;
  base.velocity.x = 0.0; base.velocity.y = 0.0;
  base.color = rgb(255,128,128);

  //Incoming box
  struct box initial;
  const long int setMass = pow(100,(DIGITS-1));//How much mass should the second box have?
  //Some logging info:
  printf("Using block mass: %lukg\n",setMass);
  printf("Calculating %i Physics Steps / second\n",PHYSICS_SUBSTEPS*RENDER_FRAMERATE);
  printf("Working on it...\n\n");
  //Finish the second box.
  initial.mass = setMass;
  initial.position.x = round(WWIDTH/3);     initial.position.y = WHEIGHT-3.5;
  initial.size.x = 3.5;        initial.size.y = 3.5;
  initial.velocity.x = -SPEED; initial.velocity.y = 0.0;
  initial.color = rgb(255,0,255);
  //initialize some sim variables.
  quit = false;frame = 0;done = 0;SDL_Event e;
  //And run the sim!
  while (!quit) {
    while (SDL_PollEvent(&e)) {//Check if the window was closed or ctrl+c pressed.
      if (e.type == SDL_QUIT) {
        quit = true;
      }
    }
    if (SDL_LockSurface(surface) == 0) {//If the surface is ready to be locked (EG not resizing, etc)
      memset(pixels, 0, sizeof(uint32_t) * WIDTH * HEIGHT);//First clear the whole screen and set pixels to black
      //render START
      RENDERGRID ? renderUnitGrid() : 0;//If render grid is enabled, do that first since it's the background.
      drawBox(&base);// Lets draw the base box. See the function definition for more.
      drawBox(&initial);// And the heavy box
      //RENDER END
      SDL_UnlockSurface(surface);//Now unlock the surface
    }
    SDL_UpdateWindowSurface(window);//Draw the frame to screen (no vsync artifacts)
    SDL_Delay(round((1000.0/RENDER_FRAMERATE)));//Wait until the frame is done.
    frame++;//Increment frame counter;
    //Physics simulation START
    for(int substep = 0; substep < PHYSICS_SUBSTEPS; substep++){//Now take the number of physics steps
      step(&base,&initial);//See function definition.
    }
    if(done > 0 && done + (RENDER_FRAMERATE) <= frame){//If the "win condition" was set 1 sec ago, quit the program on next loop.
      quit = true;
    }
  }
  SDL_DestroyWindow(window);//Kill the window
  SDL_Quit();//Stop SDL
  //And print Pi!
  printf("\033[35m%i total collisions\033[0m\n",coll);                      
  //This is the formula for the number of expected collisions
  printf("\033[34m%i expected collisions\033[0m",(int)(sqrt(setMass)*M_PI));
}
//Helper function to set all the global variables.
void setAttributes(){
  FILE *fptr;//Create a file pointer
  fptr = fopen("config.txt", "r");//Open the config in read-only mode
  if(fptr != NULL) {
    char currentLine[50];//Create a current line variable.
    while(fgets(currentLine,50,fptr)){//While there exists data to read,
      char key[50];char value[50];//Keep a key and a value. This will be used later.
      int keyIdx = 0;int valueIdx = 0;int idx = 0;//Indexing variables.
      while(currentLine[idx] != '=' && currentLine[idx] != '\0'){//Read the current line and count lengths of the key and value.
        key[keyIdx++] = currentLine[idx++];
      }
      key[keyIdx] = '\0';//Set the key to null;
      if(currentLine[idx] == '=') idx++;//If there is no value, keep going.
      while(idx < 50 && currentLine[idx] != '\n' && currentLine[idx] != '\0'){
        value[valueIdx++] = currentLine[idx++];//Similar to original line, but this time actually read the value
      }
      value[valueIdx] = '\0';
      //Now check each case and assign a variable.
      if(strcmp(key, "WIDTH") == 0) WIDTH = atoi(value);
      else if(strcmp(key, "HEIGHT") == 0) HEIGHT = atoi(value);
      else if(strcmp(key, "WWIDTH") == 0) WWIDTH = atoi(value);
      else if(strcmp(key, "WHEIGHT") == 0) WHEIGHT = atoi(value);
      else if(strcmp(key, "RENDER_FRAMERATE") == 0) RENDER_FRAMERATE = atoi(value);
      else if(strcmp(key, "PHYSICS_SUBSTEPS") == 0) PHYSICS_SUBSTEPS = atoi(value);
      else if(strcmp(key, "DIGITS") == 0) DIGITS = atoi(value);
      else if(strcmp(key, "SPEED") == 0) SPEED = atoi(value);
      else if(strcmp(key, "RENDERGRID") == 0) RENDERGRID = atoi(value);
    }
    fclose(fptr);
  }
}
double distance(struct v2d *pos1, struct v2d *pos2){
  //Pythagorean Theorum. Self explanatory
  const double dx = fabs(pos1->x-pos2->x);
  const double dy = fabs(pos1->y-pos2->y);
  return (sqrt((dx*dx)+(dy*dy)));
}
//draws rows and columns onscreen. Self explanatory
void renderUnitGrid(){
  for(int col = 0; col < WWIDTH;col++){
    struct v2di s = {col*XSCALE,0.0};
    struct v2di e = {col*XSCALE,HEIGHT};
    drawStraightLine(s,e,rgb(100,0,0));
  }
  for(int row = 0; row < WHEIGHT; row++){
    struct v2di s = {0.0,row*YSCALE}; 
    struct v2di e = {WIDTH,row*YSCALE};
    drawStraightLine(s,e,rgb(0,0,100));
  }
}
//Writes a pixel. Finds its offset and writes it. Simple.
void pixel(int x, int y, uint32_t color) {
  if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;
  pixels[y * WIDTH + x] = color;
};
//Uses the inbuild sdl function to appropriately write the right color to screen.
uint32_t rgb(uint8_t r, uint8_t g, uint8_t b) {
 return (uint32_t)SDL_MapRGB(surface->format, r, g, b);
};
//converts a workspace coordinate (float) to a pixel (int)
struct v2di pixl(long double x, long double y){
  long double xres = (long double)WIDTH / WWIDTH;
  long double yres = (long double)HEIGHT / WHEIGHT;
  struct v2di loc = {round(x * xres), round(y * yres)};
  return loc;
};
//Does a physics step.
void step(struct box *box1, struct box *box2){
  //Step 1: See if any collisions need to be made, using radial cast.
  if(distance(&(box1->position),&(box2->position)) <= box1->size.x+box2->size.x){
    //Elastic collision: exchange velocities based on mass
    const long double v1 = box1->velocity.x;
    const long double v2 = box2->velocity.x;
    const long int m1 = box1->mass;
    const long int m2 = box2->mass;
    
    //Elastic collision formulas
    box1->velocity.x = ((m1 - m2) * v1 + 2 * m2 * v2) / (m1 + m2);
    box2->velocity.x = ((m2 - m1) * v2 + 2 * m1 * v1) / (m1 + m2);
    coll++;
  }
  //Move both boxes
  move(box1);
  move(box2);
  if(box1->velocity.x >= 0 && box1->velocity.x < box2->velocity.x && box2->velocity.x >= 0 && done == 0){//win condition
    done = frame;
    printf("Reached final conditon, stopping in 1sec...\n");
  }
}
//use to move a box.
void move(struct box *b){
  const long double physics_fps = (long double)RENDER_FRAMERATE * PHYSICS_SUBSTEPS;// convert to wu/physs
  const long double xscaled = (b->velocity.x)/physics_fps;// conver to wu/s
  const long double yscaled = (b->velocity.y)/physics_fps;
  b->position.x += xscaled;
  b->position.y += yscaled;
  if(b->position.x-b->size.x <= 0){//bounce off the left side of the screen.
    b->velocity.x = -(b->velocity.x);//Invert the velocity
    coll++;//This also counts as a collision.
  }
}
//Draws a box on screen!
void drawBox(struct box *target){
  struct v2d bSize = target->size;//Stores size
  struct v2d bPos = target->position;//Stores position
  const struct v2di sPos = pixl(bPos.x-bSize.x,bPos.y-bSize.y);//Starting pixel
  const struct v2di ePos = pixl(bPos.x+bSize.x,bPos.y+bSize.y);//Ending pixel
  for(int x = sPos.x; x < ePos.x; x++){
    for(int y = sPos.y; y < ePos.y; y++){
      pixel(x, y, target->color);//Now draw each pixel in the box.
    }
  }
}
//Bresenham's line algorithm. I did not invent this. Takes a start and end coordinate and draws a line of specified color to it.
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