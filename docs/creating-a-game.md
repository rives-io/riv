# Creating a game

This tutorial will guide on how to create a simple game and compile it into a cartridge.

For our first game we will choose the Snake game,
because it's a very simple game.

This tutorial will focus using the C programming language,
and will assume you understand basic C,
in the future it might be expanded to other languages.

To make thing easier (and also performant!),
the C code is structured without use of pointers or memory allocations,
so it should be easy to follow, even for people who know only basic C.

## Goals

At the end of this tutorial, you will have a working cartridge of the Snake game,
ready to be distributed or uploaded on RIVES.

During this tutorial you will learn to:
- Draw rectangles on the screen
- Draw sprites on the screen
- Handle gamepad inputs
- Play simple sounds
- Compile a cartridge

## Code for the game

If you are stuck, checkout the full source code for this tutorial at
[demos/tutorial-snake folder](https://github.com/rives-io/riv/tree/main/demos/tutorial-snake),
every chapter has its full source code there.

## Setup project files

Before you begin you must have RIVEMU installed first, if you did not download it yet,
please read [getting started page](/getting-started) first.

Let's begin creating a brand new directory called `tutorial-snake`
with the basic stuff you need there:

```sh
mkdir tutorial-snake
cd tutorial-snake
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

// Called every frame to draw the game
void draw() {
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
}

// Entry point
int main() {
    // Main loop, keep presenting frames until user quit or game ends
    do {
        // Draw game graphics
        draw();
    } while(riv_present());
    return 0;
}
```

Save and run it with:
```sh
alias rivemu-jitc='rivemu -sdk -workspace -no-loading -bench -exec riv-jit-c'
rivemu-jitc snake.c
```

You should see the game's first title screen.
Notice we added `-no-loading` option here,
this will skip the loading screen, so we can test faster,
the other options are explained in
[developing cartridges page](/developing-cartridges).

The code should be easy to understand so far,
it just create the main game loop,
and at every iteration it clears the screen and draws the game title.

Notice we used some constant for colors and fonts,
by default RIV comes with a predefined 32 color palette
and two small predefined monospaced fonts,
both are customizable,
however for this tutorial we will just use the default colors and fonts.

The source code for this chapter can be seen at
[snake-1.c](https://github.com/rives-io/riv/blob/main/demos/tutorial-snake/snake-1.c).

## Customizing the resolution and frame rate

By default RIV has a default resolution of 256x256
and target frame rate of 60 frames per second,
both are customizable and can be changed at any time, even during the game main loop.
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

// Called every frame to draw the game
void draw() {
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
}

// Entry point
int main() {
    // Set screen size and default frame rate
    riv->width = 128;
    riv->height = 128;
    riv->target_fps = 8;
    // Main loop, keep presenting frames until user quit or game ends
    do {
        // Draw game graphics
        draw();
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
but now the computation is much lighter.
In fact if you compare this code version with the first code using the `-bench` option
you will notice the overall rendering CPU cost is down from 2.90 MIPS to 0.12MIPS (96% decrease!).
If you want to learn more about the `-bench` option, check [developing cartridges page](/developing-cartridges).

The source code for this chapter can be seen at
[snake-2.c](https://github.com/rives-io/riv/blob/main/demos/tutorial-snake/snake-2.c).

## Starting the game

Ideally, every cartridge should have a start screen with a "press to start",
mainly for two reasons. One because you don't know when the player loading
screen will finish, the performance may vary between systems the emulator is running on,
and you don't want the game to start before the player's attention is on the screen.
Second because we will let RIV intentionally use the timing to press the button
to feed entropy to its pseudo random number generator,
so the apple on the snake game can start at random places despite
everything being deterministic.
Lastly the start screen is a good opportunity to make a nice
artwork with the game title and cover,
on RIVES the very first frame is used as the default cover for cartridges.

Let's update our code:

```c
// Header including all RIV APIs
#include <riv.h>

// Game state
bool started; // true when game has started
bool ended; // true when game has ended

// Called when game starts
void start_game() {
    riv_printf("GAME START\n");
    started = true;
}

// Called when game ends
void end_game() {
    riv_printf("GAME OVER\n");
    ended = true;
    // Quit in 3 seconds
    riv->quit_frame = riv->frame + 3*riv->target_fps;
}

// Update game logic
void update_game() {
    // TODO: update game
    end_game();
}

// Draw the game map
void draw_game() {
    // TODO: draw game
}

// Draw game start screen
void draw_start_screen() {
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
}

// Draw game over screen
void draw_end_screen() {
    // Draw last game frame
    draw_game();
    // Draw GAME OVER
    riv_draw_text("GAME OVER", RIV_SPRITESHEET_FONT_5X7, RIV_CENTER, 64, 64, 2, RIV_COLOR_RED);
}

// Called every frame to update game state
void update() {
    if (!started) { // Game not started yet
        // Let game start whenever a key has been pressed
        if (riv->key_toggle_count > 0) {
            start_game();
        }
    } else if (!ended) { // Game is progressing
        update_game();
    }
}

// Called every frame to draw the game
void draw() {
    // Clear screen
    riv_clear(RIV_COLOR_DARKSLATE);
    // Draw different screens depending on the game state
    if (!started) { // Game not started yet
        draw_start_screen();
    } else if (!ended) { // Game is progressing
        draw_game();
    } else { // Game ended
        draw_end_screen();
    }
}

// Entry point
int main() {
    // Set screen size and default frame rate
    riv->width = 128;
    riv->height = 128;
    riv->target_fps = 8;
    // Main loop, keep presenting frames until user quit or game ends
    do {
        // Update game state
        update();
        // Draw game graphics
        draw();
    } while(riv_present());
    return 0;
}
```

A lot was done here:
- We moved the title screen drawing into the `draw_start_screen()` method,
and added code to draw a blinking "PRESS TO START".
- The code was refactored to have `update()` and `draw()` methods that behaves
differently depending on the game state.
- A GAME OVER screen was added, which should be triggered after pressing a key for now.
When it is shown, the game quits after 3 seconds.
- Some `riv_printf` methods were added, just for debugging.

Most games will have base structure like this,
with this base structure set,
we can continue.

Next chapters will apply patches on this base structure to evolve the game,
so make sure you understand it before going on.

The source code for this chapter can be seen at
[snake-3.c](https://github.com/rives-io/riv/blob/main/demos/tutorial-snake/snake-3.c).

## Spawning the apple

Let's spawn our first apple, and learn how to draw a rectangle in the process.
First add an enum with some important game constants:

```c
enum {
    MAP_SIZE = 16, // 16x16 tiles
    TILE_SIZE = 8, // 8x8 pixels
};

```

Next, we can add a variable to store the state of our apple position:
```c
riv_vec2i apple_pos; // position of the current apple
```

Next, we can add a method to respawn apples:
```c
// Spawn apple in a new position
bool respawn_apple() {
    apple_pos = (riv_vec2i){riv_rand_uint(MAP_SIZE-1), riv_rand_uint(MAP_SIZE-1)};
    return true;
}
```

Notice we are using RIV random functions here, the `riv_rand_uint`,
games should always prefer to use RIV's random functions instead of the programming language random.

We will call this method when the game starts, so add the following line at the end of `start_game()`:
```c
respawn_apple();
```

Finally just draw it in `draw_game()`.
```c
void draw_game() {
    // Draw apple
    riv_draw_rect_fill(apple_pos.x*TILE_SIZE, apple_pos.y*TILE_SIZE, TILE_SIZE, TILE_SIZE, RIV_COLOR_LIGHTRED);
}
```

Here we draw a 8x8 square at the apple position filled with light red color.

Now run it multiple times, and notice every time we start the apple appears at random places
despite the game being deterministic.
That is because we took advantage of the starting screen timing and key inputs
to seed the pseudo random number generator.

The source code for this chapter can be seen at
[snake-4.c](https://github.com/rives-io/riv/blob/main/demos/tutorial-snake/snake-4.c).

## Moving the snake

Let's add our snake and get it to move.
To make things simple, we will not let it grow yet,
so the goal is to have a square moving on the screen while we can use it to catch apples,
this square will effectively be our snake head later.

First add some new variables to the game state:
```c
int apples; // total amount of apples eaten
riv_vec2i head_pos; // position of the snake head
riv_vec2i head_dir; // direction of the snake head
```

Next, we need to initialize those when the game starts inside `start_game()`,
just before `respawn_apple()` call:
```c
head_dir = (riv_vec2i){0, -1};
head_pos = (riv_vec2i){MAP_SIZE / 2, MAP_SIZE / 2};
```

Now we can implement our `update_game()` method:
```c
void update_game() {
    // Set head direction based on inputs
    if (riv->keys[RIV_GAMEPAD_UP].press) {
        head_dir = (riv_vec2i){0, -1};
    } else if (riv->keys[RIV_GAMEPAD_DOWN].press) {
        head_dir = (riv_vec2i){0, 1};
    } else if (riv->keys[RIV_GAMEPAD_LEFT].press) {
        head_dir = (riv_vec2i){-1, 0};
    } else if (riv->keys[RIV_GAMEPAD_RIGHT].press) {
        head_dir = (riv_vec2i){1, 0};
    }
    // Move head
    riv_vec2i next_head_pos = (riv_vec2i){head_pos.x + head_dir.x, head_pos.y + head_dir.y};
    // Check if the head collides with the map boundary
    if (next_head_pos.x < 0 || next_head_pos.y < 0 || next_head_pos.x >= MAP_SIZE || next_head_pos.y >= MAP_SIZE) {
        end_game();
    } else {
        // Place the head on the body (will be overwritten later)
        head_pos = next_head_pos;
        // Check if the head collides with the apple
        if (head_pos.x == apple_pos.x && head_pos.y == apple_pos.y) { // Apple was eaten
            apples++;
            riv_printf("APPLES %d\n", apples);
            if (!respawn_apple()) { // End game when cannot spawn more apples
                end_game();
            }
        }
    }
}
```

Notice in the beginning we check `pressed` state for gamepad buttons,
this boolean becomes true only in the frame the button was pressed.
RIV emulates gamepads even with keyboards, but allows using them
when a gamepad is plugged (such as PS4 Controller, XBOX or Nintendo Switch controller).

Notice some logic was added to check if the head collides with map boundaries
or it collides with an apple, when it collides with an apple we spawn a new apple.

Finally let's draw the snake head at the end of `draw_game()`:
```c
// Draw snake head
riv_draw_rect_fill(head_pos.x*TILE_SIZE, head_pos.y*TILE_SIZE, TILE_SIZE, TILE_SIZE, RIV_COLOR_LIGHTGREEN);
```

Run it, now you can move and catch apples!

Notice every time we collides with an apple the "APPLES" message is printed,
and it respawns it in a new position.

The source code for this chapter can be seen at
[snake-5.c](https://github.com/rives-io/riv/blob/main/demos/tutorial-snake/snake-5.c).

## Growing the snake

Now we need to get our snake to grow its body when it eats and apple,
and move its body otherwise.
In order to do this we will need to keep track of its tail and head.
The idea to move is simple, just place its head in a new tile while removing its tail tile.
In order to keep track of the tail, we also need to store move direction for every tile in the body.

First add some new game state variable:
```c
riv_vec2i tail_pos; // position of the snake tail
riv_vec2i snake_body[MAP_SIZE][MAP_SIZE]; // move directions for the snake body
```

Next, initialize them in `game_start()`, before the call to `respawn_apple()`.
```c
tail_pos = (riv_vec2i){head_pos.x - head_dir.x, head_pos.y - head_dir.y};
snake_body[head_pos.y][head_pos.x] = head_dir;
snake_body[tail_pos.y][tail_pos.x] = head_dir;
```

Next, add an utility function to check if a position collides with the snake body:
```c
// Check if position collides with snake body
bool collides_with_body(riv_vec2i pos) {
    return !(snake_body[pos.y][pos.x].x == 0 && snake_body[pos.y][pos.x].y == 0);
}
```

We have to rework or `respawn_apple()` function to not let it spawn over the snake body:
```c
bool respawn_apple() {
    // Returns false when there is not enough space to spawn more apples
    if ((apples + 2) < (MAP_SIZE * MAP_SIZE)) {
        do { // Try to spawn until there is no collision with its body
            apple_pos = (riv_vec2i){riv_rand_uint(MAP_SIZE-1), riv_rand_uint(MAP_SIZE-1)};
        } while (collides_with_body(apple_pos));
        return true;
    }
    return false;
}
```

Out `update_game()` will need some new lines, I will just paste it here:
```c
void update_game() {
    // Set head direction based on inputs
    if (riv->keys[RIV_GAMEPAD_UP].press) {
        head_dir = (riv_vec2i){0, -1};
    } else if (riv->keys[RIV_GAMEPAD_DOWN].press) {
        head_dir = (riv_vec2i){0, 1};
    } else if (riv->keys[RIV_GAMEPAD_LEFT].press) {
        head_dir = (riv_vec2i){-1, 0};
    } else if (riv->keys[RIV_GAMEPAD_RIGHT].press) {
        head_dir = (riv_vec2i){1, 0};
    }
    // Save head move direction
    snake_body[head_pos.y][head_pos.x] = head_dir;
    // Move head
    riv_vec2i next_head_pos = (riv_vec2i){head_pos.x + head_dir.x, head_pos.y + head_dir.y};
    // Check if the head collides with the map boundary
    if (next_head_pos.x < 0 || next_head_pos.y < 0 || next_head_pos.x >= MAP_SIZE || next_head_pos.y >= MAP_SIZE) {
        end_game();
    // Check if the head collides with its body
    } else if (collides_with_body(next_head_pos)) {
        end_game();
    } else {
        // Place the head on the body (will be overwritten later)
        head_pos = next_head_pos;
        snake_body[head_pos.y][head_pos.x] = head_dir;
        // Check if the head collides with the apple
        if (head_pos.x == apple_pos.x && head_pos.y == apple_pos.y) { // Apple was eaten
            apples++;
            riv_printf("APPLES %d\n", apples);
            if (!respawn_apple()) { // End game when cannot spawn more apples
                end_game();
            }
        } else { // Apple was not eaten
            // Remove tail
            riv_vec2i tail_dir = snake_body[tail_pos.y][tail_pos.x];
            snake_body[tail_pos.y][tail_pos.x] = (riv_vec2i){0,0};
            // Update tail position
            tail_pos = (riv_vec2i){tail_pos.x + tail_dir.x, tail_pos.y + tail_dir.y};
        }
    }
}
```

The important additions were related to the snake body and the tail.

Finally, we need to replace the draw snake head code with a code to draw its body in `draw_game()`:
```c
void draw_game() {
    // Draw apple
    riv_draw_rect_fill(apple_pos.x*TILE_SIZE, apple_pos.y*TILE_SIZE, TILE_SIZE, TILE_SIZE, RIV_COLOR_LIGHTRED);
    // Draw snake body
    for (int y = 0; y < MAP_SIZE; y++) {
        for (int x = 0; x < MAP_SIZE; x++) {
            if (collides_with_body((riv_vec2i){x,y})) {
                riv_draw_rect_fill(x*TILE_SIZE, y*TILE_SIZE, TILE_SIZE, TILE_SIZE, RIV_COLOR_LIGHTGREEN);
            }
        }
    }
}
```

Run it, now our snake grows!

The game logic is effectively finished.
In the next chapters we will focus on polishing it.

The source code for this chapter can be seen at
[snake-6.c](https://github.com/rives-io/riv/blob/main/demos/tutorial-snake/snake-6.c).

## Adding sounds
The game has no sound yet, let's add it.

RIV comes with a simple level API for synthesizing sound waves using
[ADSR envelope](https://en.wikipedia.org/wiki/Envelope_(music)#ADSR)
on simple waveforms (such as square, triangular and sine waves),
and you can play different tones by varying the waveform frequency.
This is how old school game consoles generated sounds.

Let's add on top of the file sound effect configs for start game, end game and eat apple:
```c
// Sound effects
riv_waveform_desc start_sfx = {
    .type = RIV_WAVEFORM_PULSE,
    .attack = 0.01f, .decay = 0.01f, .sustain = 0.1f, .release = 0.01f,
    .start_frequency = RIV_NOTE_A3, .end_frequency = RIV_NOTE_A4,
    .amplitude = 0.25f, .sustain_level = 0.5f,
};
riv_waveform_desc end_sfx = {
    .type = RIV_WAVEFORM_PULSE,
    .attack = 0.01f, .decay = 0.01f, .sustain = 0.1f, .release = 0.01f,
    .start_frequency = RIV_NOTE_A3, .end_frequency = RIV_NOTE_A2,
    .amplitude = 0.5f, .sustain_level = 0.5f,
};
riv_waveform_desc eat_sfx = {
    .type = RIV_WAVEFORM_PULSE,
    .attack = 0.01f, .decay = 0.01f, .sustain = 0.1f, .release = 0.01f,
    .start_frequency = RIV_NOTE_A4, .end_frequency = RIV_NOTE_A4,
    .amplitude = 0.25f, .sustain_level = 0.5f,
};
```

Here we choose pulse for chiptune style sounds.
The start sound has its wave frequency growing, giving the feeling that something will begin.
The end sound has its wave frequency decaying, giving the feeling that something will end.
The eat sound has its wave frequency steady, giving the feeling you got a point.
You can play with these values to synthesize different sound styles.

Now we just need to emit these sound effects at the right places.
First in `start_game()`:
```c
// Play start sound
riv_waveform(&start_sfx);
```

Next in `end_game()`:
```c
// Play end sound
riv_waveform(&end_sfx);
```

When an apple is eaten, inside `update_game()`, just near the `apples++` increment;
```c
// Play eat sound
riv_waveform(&eat_sfx);
```

Run it, now the game has sounds!

The source code for this chapter can be seen at
[snake-7.c](https://github.com/rives-io/riv/blob/main/demos/tutorial-snake/snake-7.c).

## Drawing sprites

So far we have draw only solid squares,
let's upgrade our code to actually draw some sprites for the apple and snake head.
First we will create a 16x16 image in the PNG format with 4 tiles of 8x8 pixels:
- In the top left, the apple sprite, will have id 0
- In the top right, left empty, will have id 1
- In the bottom left, the snake head looking to the bottom, will have id 2
- In the bottom right, the snake head looking to the right, will have id 3

Use an image editor like GIMP or your favorite pixel art editor to do this,
then save it in `sprites.png` file in the same folder as `snake.c`.

For the code changes, first we need to load this sprite file, add the following in the `main()`
function before the main loop:

```c
// Load sprites
riv_make_spritesheet(riv_make_image("sprites.png", 0), 8, 8);
```

This will load the image `sprites.png` into a sprite sheet of 8x8 pixels of id `1`.
The first sprite sheet loaded will always have its id set to `1`, and the next `2`, and so on.
To make our code clear, let's define this sprite sheet id in our constants enum:

```c
enum {
    MAP_SIZE = 16, // 16x16 tiles
    TILE_SIZE = 8, // 8x8 pixels
    GAME_SPRITESHEET = 1,
};
```

Next, to draw the apple and the snake, just replace its draw code in `draw_game()`:
```c
// Draw apple
riv_draw_sprite(0, GAME_SPRITESHEET, apple_pos.x*TILE_SIZE, apple_pos.y*TILE_SIZE, 1, 1, 1, 1);
```

Finally, add code to draw the snake head at the end of `draw_game()`:
```c
// Draw snake face
int spr_id = 2 + ((head_dir.x != 0) ? 1 : 0);
int flip_x = (head_dir.x == -1) ? -1 : 1;
int flip_y = (head_dir.y == -1) ? -1 : 1;
riv_draw_sprite(spr_id, GAME_SPRITESHEET, head_pos.x*TILE_SIZE, head_pos.y*TILE_SIZE, 1, 1, flip_x, flip_y);
```

We do some tricks here to choose the correct sprite and flip it depending on the head direction.

Run it, now the game has more interesting visuals!

The source code for this chapter can be seen at
[snake-8.c](https://github.com/rives-io/riv/blob/main/demos/tutorial-snake/snake-8.c).

## Adding scores

To make this cartridge usable in RIVES in contests and achievements,
you have to output the game score every frame.
We do this by saving a JSON to the game *outcard* every frame.

An interesting game score always take time into account,
so people can challenge others on who is the fastest to accomplish something.
Remembers also that RIV has a computation limit,
so decreasing the game score as the game progresses can encourage people to play faster,
and avoid this computation limit to be reached.

We will create a score taking into account the amount of apples eaten while decreasing
the amount of game ticks (iterations or frames).

First create some new variables to keep track of the score:
```c
int ticks; // total amount of game iterations
int score; // game overall score
```

Next create a function to update the score, and save it to the *outcard*:
```c
// Output scores on the output card
void update_scores() {
    score = apples * MAP_SIZE * 2 - ticks;
    riv->outcard_len = riv_snprintf((char*)riv->outcard, RIV_SIZE_OUTCARD, "JSON{\"score\":%d,\"apples\":%d,\"ticks\":%d}", score, apples, ticks);
}
```

Here we save a table with interesting game statistics in JSON format,
RIVES can use this table to create tournament using different formulas for the score.
This table should always have the `"score"` field, this is the basic overall score for the game.

Next, update the score every frame, by adding at the end of `update()`:
```c
// Output scores
update_scores();
```

Next, we want to increment `ticks` every `update_game()`, so add to its begin:
```c
ticks++;
```

Finally, let's draw the overall score on the screen, add at the end of `draw_map()`:
```c
// Draw score
char buf[128];
riv_snprintf(buf, sizeof(buf), "SCORE %d", score);
riv_draw_text(buf, RIV_SPRITESHEET_FONT_3X5, RIV_BOTTOMLEFT, 1, 128-1, 1, RIV_COLOR_WHITE);
```

Run it, now the game has scores!

The source code for this chapter can be seen at
[snake-9.c](https://github.com/rives-io/riv/blob/main/demos/tutorial-snake/snake-9.c).

## Compiling the cartridge

Compiling a cartridge has been covered in the [developing cartridges page](/developing-cartridges)
guide, read there for more details.

But in a short, having the SDK, you can compile the final snake cartridge and test it with:

```sh
alias rivemu-exec='rivemu -quiet -no-window -sdk -workspace -exec'
rivemu-exec gcc snake.c -o snake '$(riv-opt-flags -Ospeed)'
rivemu-exec riv-strip snake
rivemu-exec riv-mksqfs snake sprites.png snake.sqfs
rivemu snake.sqfs
```

## Where to go now?

You can continue and do more improvements in the snake, such as:
- Improve the input handling. Right now if you release a key while another is being pressed, the snake will not move.
- Increase the snake speed as the game progresses, you could increase target fps while running for this.
- Tweak and add more sounds.
- Tweak and add more graphics.
- Make a more interesting score screen, with more statistics.

After you finish your tweaking, you could try uploading it on RIVES.
