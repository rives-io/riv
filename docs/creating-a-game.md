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

## Setup

Before you begin you must have RIVEMU installed first, if you did not download it yet,
please read [getting started page](/getting-started) first.

## Creating project files

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

## The screen frame buffer

Now open `snake.c` in your favorite text editor,
and add the following code:

```c
#include <riv.h>

int main() {
    do {
        // clear screen
        riv_clear(RIV_COLOR_BLUE);
        // draw hello world
        riv_draw_text("snake", RIV_SPRITESHEET_FONT_5X7, RIV_CENTER, 128, 128, 2, RIV_COLOR_WHITE);
    } while(riv_present());
    return 0;
}
```
