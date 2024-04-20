# Creating a game

This tutorial will guide on how to create a simple game and compile it into a cartridge.

For our first game we will choose the Snake game,
because it's a very simple game and can be implemented pretty much in
any programming language.
This tutorial will focus using the C programming language,
in the future it might be expanded to other languages.

## Goals

At the end of this tutorial, you will have a working cartridge of the Snake game,
ready to be distributed or uploaded on rives.io .

During this tutorial you will learn to:
- Draw primitives on the screen
- Play simple sounds
- Handle user input
- Compile your cartridge

## Code for the game

If you are stuck, checkout the full source code for this tutorial at
[https://github.com/rives-io/riv/blob/main/samples/snake-tutorial.c].

## Setup project files

Before you begin you must have RIVEMU installed first, if you did not download it yet,
please read [getting started page](/getting-started) first.

Let's begin creating a brand new directory called `snake-tutorial`
with the basic stuff you need there:

```sh
mkdir snake-tutorial
cd snake-tutorial
echo "void main() {}" > snake.c
rivemu -workspace -exec riv-jit-c snake.c
```

The last line execute this simple C program, and you should see an output similar to:
```
$ rivemu -sdk -workspace -exec riv-jit-c snake.c
[RIVEMU] Starting up...
[RIVEMU] Open RIV device
[RIVEMU] Close RIV device
[RIVEMU] Cycles 58018349
[RIVEMU] Shutting down...
```

The C program did nothing interesting yet,
it just opened and closed the RIV device,
the device responsible for handling graphics, audio and inputs.

## Drawing the title screen

Now open `snake.c` in your favorite text editor,
and add the following code:

```c
// Header including all RIV APIs
#include <riv.h>

// Cartridge entry point
int main() {
    // Main loop
    do {
        // Clear screen
        riv_clear(RIV_COLOR_DARKSLATE);
        // Draw snake title
        riv_draw_text(
            "snake",                  // text to draw
            RIV_SPRITESHEET_FONT_5X7, // sprite sheet id of the font
            RIV_CENTER,               // anchor point on the text bounding box
            128,                      // anchor x
            128,                      // anchor y
            4,                        // text size multiplier
            RIV_COLOR_LIGHTGREEN      // text color
        );
        // Keep presenting frames until user quits
    } while(riv_present());
    return 0;
}
```

Save and run it with:
```sh
alias rivemu-jitc='rivemu -sdk -workspace -no-loading -bench -exec riv-jit-c'
rivemu-jitc snake.c
```

You should see the game first title screen.
Notice we added `-no-loading` option here,
this will skip the loading screen, so we can test faster,
the other options are explained in
[developing cartridges page](/develping-cartridges).

The code should be easy to understand so far,
it just create the main game loop,
and at every iteration it clears the screen and draws the game title.

Notice we used some constant for colors and fonts,
by default RIV comes with a 32 color palette,
and some small monospaced fonts,
both are customizable,
however for this tutorial we will skip using the default one.

## Customizing the resolution and frame rate

By default RIV has a default resolution of 256x256
and target frame rate of 60 frames per second,
both are customizable and can be changed at any time, even during game main loop.
We will change the resolution down to 128x128 and target fps down o 8,
as an exercise and because our snake game will be so simple
that drawing at a lower resolution and lower frame rate
can save us CPU overhead, and make game processing lighter.
Well designed cartridges should try to use the minimal resolution and
frame rate to save CPU resources.

Let's update our code with the new resolution:

```c
// Header including all RIV APIs
#include <riv.h>

// Cartridge entry point
int main() {
    // Set screen size and default frame rate
    riv->width = 128;
    riv->height = 128;
    riv->target_fps = 30;
    // Main loop
    do {
        // Clear screen
        riv_clear(RIV_COLOR_DARKSLATE);
        // Draw snake title
        riv_draw_text(
            "snake",                  // text to draw
            RIV_SPRITESHEET_FONT_5X7, // sprite sheet id of the font
            RIV_CENTER,               // anchor point on the text bounding box
            64,                       // anchor x
            64,                       // anchor y
            2,                        // text size multiplier
            RIV_COLOR_LIGHTGREEN      // text color
        );
        // Keep presenting frames until user quits
    } while(riv_present());
    return 0;
}
```

Let's run again, this is the last time in this tutorial we will paste
the command to run, next time we say "run" just execute the same command,
let's run it:

```sh
rivemu-jitc snake.c
```

It should just draw the exactly same title as before,
but now it's the computation is much lighter.
In fact if you compare this code version with the first code using the `-bench` option
you will notice the overall rendering CPU cost is down from 2.90 MIPS to 0.18MIPS (94% decrease!).
If you want to learn more about the `-bench` option, check [developing cartridges page](/develping-cartridges).

## Starting the game

Ideally, every cartridge should have a title screen with a "press to start",
mainly for two reasons. One because you don't know when the player loading
screen will finish, the performance may vary between systems the emulator is running on,
and you don't want the game to start before the player attention is on the screen.
Second because we will let RIV intentionally use the timing to press the button
to feed entropy to its pseudo random number generator,
so the apple on the snake game can start at random places despite
everything being deterministic.
Lastly the title screen is a good opportunity to make a nice
artwork for the game cover,
on RIVES the very first frame is used as the default cover for cartridges.

Let's update our code:

```c
// Header including all RIV APIs
#include <riv.h>

// Game state
static bool started; // true when game has started

// Handle title screen
void title_screen() {
    // Draw snake title
    riv_draw_text(
        "snake",                  // text to draw
        RIV_SPRITESHEET_FONT_5X7, // sprite sheet id of the font
        RIV_CENTER,               // anchor point on the text bounding box
        64,                       // anchor x
        64,                       // anchor y
        2,                        // text size multiplier
        RIV_COLOR_LIGHTGREEN      // text color
    );
    // Make "press to start blink" by changing the color depending on the frame number
    uint32_t col = (riv->frame % 2 == 0) ? RIV_COLOR_LIGHTRED : RIV_COLOR_DARKRED;
    // Draw press to start
    riv_draw_text("PRESS TO START", RIV_SPRITESHEET_FONT_5X7, RIV_CENTER, 64, 64+16, 1, col);
    // Let game start whenever a key has been pressed
    started = riv->key_toggle_count > 0;
}

// Handle a game iteration
void game() {
    // Game logic not implemented yet, let's quit
    riv->quit = true;
}

// Cartridge entry point
int main() {
    // Set screen size and default frame rate
    riv->width = 128;
    riv->height = 128;
    riv->target_fps = 8;
    // Main loop
    do {
        // Clear screen
        riv_clear(RIV_COLOR_DARKSLATE);
        // Decide the game state
        if (!started) {
            title_screen();
        } else {
            game();
        }
        // Keep presenting frames until user quits
    } while(riv_present());
    return 0;
}
```

We moved the title screen drawing into the `title_screen()` method,
and added code to draw a blinking "PRESS TO START" text by using `riv->frame`.
The

## Spawning the apple

## Moving the snake

## Growing the snake

## Adding sounds

## Outputting scores

## Handling parameters

## Compiling the cartridge


