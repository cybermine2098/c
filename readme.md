# This is a project for my physics class.

I wouldn't reccomend looking at it too hard since I am an amateur when it comes to C.

## Purpose

As outlined in [this](https://www.youtube.com/watch?v=6dTyOl1fmDo) 3blue1brown video: you can compute increasing digits of pi using colliding blocks. I've built an interactive simulation capable of doing this.

This was an originally listed as unsolved problem in the video, but this reddit thread actually clears up why this happens:

**[Explanation like I'm five](https://www.reddit.com/r/explainlikeimfive/comments/177gm48/comment/k4w3705/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button)**:<br>
This comment describes how the behaviour is caused by the fact that this system emulates a wave, and waves are inherently 'circular' in their nature. This system also has a frequency and an oscillation creating a wave that can be defined by it's properties of collisions.

**[Other simpler explanation](https://www.reddit.com/r/explainlikeimfive/comments/177gm48/comment/k4ta3ec/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button)**:<br>
This comment details that this system depends on the pythagorean theorum, one that can be used to re-build Pi using it's source. This is a fundamental principle of our euclidian reality and it helps explain why this happens.

## Included files

<ul>
    <li><strong>shell.nix</strong>: I use nixos and this is critical for ANYTHING to EVER work on this godforsaken operating system.</li>
    <li><strong>compile.sh</strong>: A simple bash script to help me speed compiling and executing up. </li>
    <li><strong>phys & phys.c</strong>: A compiled and uncompiled version of the program</li>
    <li><strong>readme.md</strong>: this file</li>
</ul>

## How to use

### Definitions:

In the config file (`config.txt`) there are many attributes, this is what they all do

<table>
    <thead>
        <th>Key</th>
        <th>Purpose</th>
    </thead>
    <tbody>
        <tr>
            <td>HEIGHT</td>
            <td>Defines the window height</td>
        </tr>
        <tr>
            <td>WIDTH</td>
            <td>Defines the window width</td>
        </tr>
        <tr>
            <td>WWIDTH</td>
            <td>Defines the workspace Width</td>
        </tr>
        <tr>
            <td>WHEIGHT</td>
            <td>Defines the workspace Height</td>
        </tr>
        <tr>
            <td>RENDER_FRAMERATE</td>
            <td>Set how many frames are rendered per second. Note decreasing this will decrease the physics accuracy as well.</td>
        </tr>
        <tr>
            <td>PHYSICS_SUBSTEPS</td>
            <td>Sets how many physics steps are taken per frame. Decreasing this may improve performance but will decrease simulation accuracy.</td>
        </tr>
        <tr>
            <td>DIGITS</td>
            <td>How many digits of pi to compute. Keep in mind this has exponental compute demand per linear increase.</td>
        </tr>
        <tr>
            <td>SPEED</td>
            <td> Controls the initial velocity of the simulation. The output is the same but running the simulation for longer will increase it's accuracy, so turning down the velocity can help improve accuracy for high loads.</td>
        </tr>
        <tr>
            <td>RENDERGRID</td>
            <td>Sets if the workspace grid is rendered or not. 0 -> false | 1-> true</td>
        </tr>
    </tbody>
</table>

### Running

> This is C code, and SDL comes bundled, so if you want to see it in action just run the executable:

All major desktop OSes:

        ./phys

To edit the simulation variables see `config.txt`.

### Compiling

NixOS: `nix-shell --run "./cr.sh phys -r"`

**Other Operating systems:**

<ol>
    <li>Make sure you have [SDL2](https://wiki.libsdl.org/SDL2/Installation) installed. For NixOS specifically there's a premade shell. You're welcome.</li>
    <li>For most linux systems the provided compiler manager should work assuming SDL2 and pkg-config is installed: <code>./cr.sh phys -r</code></li>
    <li>For unix systems you may have to run the commands manually. This varies from OS to OS, but It's best to google it.</li>
    <li>Windows: Man idk good luck.</li>
</ol>

## What will happen:

The simulation will start and the block will roll towards the other. When the 'final' state is reached, the window will close itself, or you may have to close it manually due to FP errors. The total # of collisions will print in your terminal.

_This project was made without the use of Generative AI or similar devices._
_All references come from reddit, stack overflow, geeks4geeks, or w3schools._
