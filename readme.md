# This is a project for my physics class. 
I wouldn't reccomend looking at it too hard since I am an amateur when it comes to C.

## Purpose
As outlined in [this](https://www.youtube.com/watch?v=6dTyOl1fmDo) 3blue1brown video: you can compute increasing digits of pi using colliding blocks. I've built an interactive simulation capable of doing this. 

This was an originally unsolved problem, but this reddit thread actually clears up why this happens:

**[Explanation like I'm five](https://www.reddit.com/r/explainlikeimfive/comments/177gm48/comment/k4w3705/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button)**

This comment describes how the behaviour is caused by the fact that this system emulates a wave, and waves are inherently 'circular' in their nature. This system also has a frequency and an oscillation creating a wave that can be defined by it's properties of collisions.

**[Other simpler explanation](https://www.reddit.com/r/explainlikeimfive/comments/177gm48/comment/k4ta3ec/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button)**

This comment details that this system depends on the pythagorean theorum, one that can be used to re-build Pi using it's source. This is a fundamental principle of our euclidian reality and it helps explain why this happens.

## Included files
- **shell.nix** -- I use nixos and this is critical for ANYTHING to EVER work on this godforsaken operating system.
- **compile.sh** -- A simple bash script to help me speed compiling and executing up. 
- **phys & phys.c** -- A compiled and uncompiled version of the program
- **readme.md** -- this file

## How to use

### Definitions:
In the config file (`config.txt`) there are many attributes, this is what they all do
- **HEIGHT**: Defines the window height
- **WIDTH**: Defines the window width
- **WWIDTH**: Defines the workspace Width
- **WHEIGHT**: Defines the workspace Height
- **RENDER_FRAMERATE**: Set how many frames are rendered per second. Note decreasing this will decrease the physics accuracy as well.
- **PHYSICS_SUBSTEPS**: Sets how many physics steps are taken per frame. Decreasing this may improve performance but will decrease simulation accuracy.
- **DIGITS**: How many digits of pi to compute. Keep in mind this has exponental compute demand per linear increase.
- **SPEED**: Controls the initial velocity of the simulation. The output is the same but running the simulation for longer will increase it's accuracy, so turning down the velocity can help improve accuracy for high loads.
- **RENDERGRID**: Sets if the workspace grid is rendered or not. 0 -> false | 1-> true

### What will happen:
The simulation will start and the block will roll towards the other. When the 'final' state is reached, the window will close itself, or you may have to close it manually due to FP errors. The total # of collisions will print in your terminal.

### What's missing:
- SDL Compiled folder. I can't legally include this in my git repo but you can figure out how to add it for yourself.
- Windows & MacOS compiler
- gcc

*This project was made without the use of Generative AI or similar devices.*
*All references come from reddit, stack overflow, geeks4geeks, or w3schools.*