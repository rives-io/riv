# Developing cartridges

This tutorial will guide on how to setup your
development workflow and how to create your first cartridge.

By the end of this tutorial you will know how to create cartridge files,
compile cartridges, debug cartridges, benchmark cartridges,
inspect cartridges and how to use other programming languages to create cartridges.

This tutorial will begin quickly by running your first graphical application,
and then expand slowly about all the tools RIV SDK offers to develop cartridges,
you will learn some new interesting RIVEMU options on the go.

If you have not installed RIVEMU yet, please read
the [getting started page](/getting-started) first.

## Creating your first application in C

Technically you can use any programming supported by Linux RISC-V to create cartridges,
however RIV offers its standard API in C with `libriv`,
therefore you can use C to create RIV cartridges.
This tutorial will focus on using just C to getting started and understand the basics,
later it will expand on how to use other programming languages.

Let's begin creating a file named `hello.c` with the following contents:

```c
#include <riv.h>

int main() {
    do {
        // clear screen
        riv_clear(RIV_COLOR_BLACK);
        // draw hello world
        riv_draw_text("hello world!", RIV_SPRITESHEET_FONT_5X7, RIV_CENTER, 128, 128, 2, RIV_COLOR_WHITE);
    } while(riv_present());
    return 0;
}
```

The standard RIV OS comes with a JIT for C programming language called `riv-jit-c`,
so you can run this example directly without having to actually compile it:

```sh
rivemu -workspace -exec riv-jit-c hello.c
```

You should see a screen popup, and that is it, your first hello world program running in RIV!

The `-exec` option overrides the standard machine entrypoint to execute a custom command,
in this case we are executing `riv-jit-c` to run the `hello.c` file.
The `-workspace` option exposes current directory files into the machine,
it will be explained in detail in the next section.

The C code presented so far should be self explanatory,
we have a line that clear the screen,
another to draw a text and finally a line to present the cartridge frame.
You can read the full *libriv* C API in its
header at [riv.h](https://github.com/rives-io/riv/blob/main/libriv/riv.h),
for now we won't give much details on its functions,
let's continue understanding the RIV development workflow first.

## Interactive terminal with workspaces

Notice in the last example the `-workspace` option,
it mounts the current working directory from your host to the `/workspace` directory in the RIV machine,
to make development easier.

This is a good time show how RIV is also a Linux,
where you can work in its own terminal when combining with the `-it` option:

```sh
rivemu -workspace -it
```

The `-it` options standards for interactive terminal,
it gives the ability to type commands inside a terminal running inside a running RIV machine.
You can type really almost any Linux command, for example:

```sh
rivos:/workspace# uname -mns
Linux rivos riscv64
rivos:/workspace# pwd
/workspace
rivos:/workspace# ls *.c
hello.c
```

Notice the `hello.c` from last example is there,
because `-workspace` mounted the current working directory,
while we are inside a RISC-V 64-bit Linux machine.

You can also run your `hello.c` directly from the shell inside riv:
```sh
riv-jit-c hello.c
```

If you edit the file `hello.c` in a file editor in your host,
the changes will also take effect inside RIV OS.
But why would you want to run commands inside the RIV OS terminal?
Well because the RIV also has the RIV SDK that comes with lot of utilities
for developing cartridges,
tools for compiling RISC-V binaries, compressing files into cartridge archives,
and debuggers for debugging RISC-V applications.

After you experiment with it, make sure to exit the interactive terminal session
to continue to the next section.

## Installing the RIV SDK

RIVEMU comes with a builtin and minimal operating system called RIV OS,
this RIV OS contains the bare minimal libraries and utilities
to run cartridges, but in other to actually create cartridges
you need many more tools like programming language compilers
and archive compressors.

You may also want to customize the RIV SDK with your own Linux tools,
such as other programming languages (e.g Rust, Odin, Zig, Nim..).
With the SDK you can do this, because it's writable and customizable,
while the builtin standard minimal RIV OS is read-only.

Bear in mind the standard SDK is very large (about 500MB),
because it contains compilers, debuggers, editors, and many other tools for development only.

Let's download the SDK and try it:

```sh
wget -O rivos-sdk.ext2 https://github.com/rives-io/riv/releases/latest/download/rivos-sdk.ext2
rivemu -sdk=rivos-sdk.ext2 -exec gcc --version
```

You probably got an output that contains something like:
```
gcc (Alpine 13.2.1_git20240309) 13.2.1 20240309
Copyright (C) 2023 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

The `-sdk=rivos-sdk.ext2` overrides the standard operating system
to use the SDK operating system,
the command `-exec gcc --version` should show the GCC C compiler version
that is included in the SDK, which was not included in the minimal RIV OS.

Lets now install this SDK globally, to make available in any terminal in your system:
```sh
mkdir -p $HOME/.riv/
mv rivos-sdk.ext2 $HOME/.riv/
echo 'export RIVEMU_SDK=$HOME/.riv/rivos-sdk.ext2' >> $HOME/.bashrc
export RIVEMU_SDK=$HOME/.riv/rivos-sdk.ext2
```

When setting the environment `RIVEMU_SDK`,
RIVEMU will be able to auto detect and load the SDK,
so you can use from anywhere in your system,
let's try it:

```sh
rivemu -quiet -no-window -sdk -exec /usr/lib/libriv.so version
```

You should see and output similar to:
```
0.3.0
```

We have introduced new command line options here.
The `-quiet` makes RIVEMU less verbose because we just
want to see an output from a command executed inside the machine.
The `-no-window` makes RIVEMU never create a graphical window,
because we want to just run a console application in this case, that is, we won't be playing a cartridge.
The `-sdk` without a filename will try to read the SDK filesystem from `RIVEMU_SDK` environment variable.
The `-exec /usr/lib/libriv.so version` will execute a command to check the `libriv` version in the OS.

## Creating your first cartridge

Now with the SDK installed,
let's get back to the `hello.c` example and actually turn it into a cartridge.

Every cartridge must have an *entrypoint* to actually run,
first let create a shell script file named `0-entry.sh`:

```sh
echo "riv-jit-c hello.c" > 0-entry.sh
chmod +x 0-entry.sh
```

Why the name is `0-entry.sh`? When running a cartridge,
RIV actually executes the first executable file in a cartridge,
sorted alphabetically.
In case you have other executable binaries in your cartridge,
they could executed before the entrypoint as a mistake,
so it is recommended to make the entrypoint be the first file
when ordering its files by name.

Now lets invoke RIVEMU to actually compress both `0-entry.sh` and `hello.c` into
a cartridge archive.

```sh
rivemu -quiet -no-window -sdk -workspace -exec riv-mksqfs 0-entry.sh hello.c hello.sqfs
```

The `riv-mksqfs` command should create the actual cartridge `.sqfs` file, lets inspect it:

```sh
rivemu -quiet -no-window -sdk -workspace -exec 'ls -l hello.sqfs && file hello.sqfs'
```

It should output something similar to:
```sh
-rw-r--r--    1 user     user          4096 Apr 19 20:48 hello.sqfs
hello.sqfs: Squashfs filesystem, little endian, version 4.0, zlib compressed, 434 bytes, 3 inodes, blocksize: 131072 bytes, created: Thu Jan  1 00:00:00 1970
```

You can see the cartridge file has 4096 bytes,
this is because cartridges actually are SquashFS compressed filesystem,
which are always saved in multiple of 4KB,
this also means the minimum size for a cartridge is always 4KB.
Notice the cartridge contents were compressed with *zlib*.

The compression could also be tuned to generate smaller cartridges,
for example you could use more aggressive compression appending `-comp xz`,
like `riv-mksqfs 0-entry.sh hello.c hello.sqfs -comp xz`
for checking available compressions read `mksquashfs -help`


Now lets finally run it:

```sh
rivemu hello.sqfs
```

If everything worked, you should see "hello world!" screen show up.
This is your first cartridge!

This cartridge file is also the one you should upload on RIVES.

## Testing cartridges in a web browser

Having a cartridge running locally in the RIVEMU is great,
but most players will probably play your cartridges in a Web Browser,
luckily RIVEMU was also ported to WebAssembly,
RIV has a test page for you to test a cartridge in the browser
at [this testing page](https://rives-io.github.io/riv/).
Go there and search for the "Upload" button in the "Cartridge" section,
use it to upload your `hello.sqfs`.

If everything works fine, you should be able to see the "hello world!"
in the browser.
Every time you finish developing a cartridge,
it's recommended to actually test it in the browser,
because the performance can be different than running natively,
usually performance drops by 20% or more.

Also in the browser you can even use game pads to test your game!
If you have a PS4, Xbox or Nintendo Switch controller,
just plug in your computer and most browser supports them through
the HTML5 Gamepad API specification.
Before trying this, you should check if your game pad is working in your browser
in third party services such as
[this game pad tester](https://hardwaretester.com/gamepad).

## Compiling cartridges

In our first cartridge `hello.sqfs`, we used `riv-jit-c` to run `hello.c`,
although this was very easy we can actually compile it into a `hello` RISC-V executable binary:

```sh
rivemu -quiet -no-window -sdk -workspace -exec gcc hello.c -o hello -lriv
```

The command `gcc hello.c -o hello -lriv` compiles `hello.c`
source code into `hello` Linux RISC-V ELF binary linked to `libriv`.

Before generating a cartridge for it, let's test it:

```sh
rivemu -workspace -exec ./hello
```

Then you should see a hello world screen like before.
Now let's actually compile a cartridge and run it:

```sh
rivemu -quiet -no-window -sdk -workspace -exec riv-mksqfs hello hello-compiled.sqfs
rivemu hello-compiled.sqfs
```

Like before you should see hello world screen,
but this cartridge is different from the first one,
because it used ahead of time compilation now,
which makes your cartridge faster to load and run and smaller for cartridges with lot of code.
We recommend to use JIT just for prototyping,
when possible your final release cartridge should be statically compiled.

## Compiling optimized cartridges

In last example we compiled a C code, but we did not enable any optimization,
lets enable them not only for improve its performance, but also to reduce its final size,
so we can make a cartridge smaller.
RIV SDK comes with a tool to assist you choosing C compilers flags to
to generate optimized and small binaries called `riv-opt-flags`.

Lets recompile a new cartridge for the last example again using these tools:

```sh
alias rivemu-exec='rivemu -quiet -no-window -sdk -workspace -exec'
rivemu-exec gcc hello.c -o hello-optimized '$(riv-opt-flags -Ospeed)'
rivemu-exec riv-strip hello-optimized
rivemu-exec riv-mksqfs hello-optimized hello-optimized.sqfs
rivemu hello-optimized.sqfs
```

Let's break what happened:
1. In the first line we create the alias `rivemu-exec`, a shortcut to invoke commands inside RIV OS.
2. In the second line we compile `hello.c` again, but this time to `hello-optimized` using compiler flags from `riv-opt-flags -Ospeed`, this will supply compiler flags to optimized for speed because we passed `-Ospeed`, this tool also accepts `-Osize` to optimize for size, and `-Odebug` to optimize for debugging.
3. In the third line we strip unwanted data from the ELF binary `hello-optimized`, this should compress the file a little more.
4. In the fourth line we create the cartridge `hello-optimized.sqfs`
5. Finally in the last line we test it.

Lets compare the final with old one we created:

```sh
$ rivemu-exec 'unsquashfs -stat hello-compiled.sqfs | grep bytes'
Filesystem size 3517 bytes (3.43 Kbytes / 0.00 Mbytes)
$ rivemu-exec 'unsquashfs -stat hello-optimized.sqfs | grep bytes'
Filesystem size 1661 bytes (1.62 Kbytes / 0.00 Mbytes)
```

Notice the data in `hello-optimized.sqfs` cartridge is smaller than `hello-compiled.sqfs`.
However if you take a look with `ls -la`, you will notice both cartridges files are still at 4KB,
this is because SquashFS files add 0 padding at the end, to make us able to use the `mount` command.
This cartridge is very small, only in a more complex one you would see the benefits.

If you can afford, prefer to use `-Ospeed` to optimize for performance,
use `-Osize` only if you really want to reduce its size as priority,
and use `-Odebug` only to debug while in development.

On RIVES it is recommended to upload optimized and small cartridges to reduce costs,
and improve its verification performance.
Even though both are 4KB, the ones containing zeros will cost less to upload on RIVES,
because transactions with zeros in them cost less in some blockchain base layers.

## Debugging cartridges

In the last example we mentioned `-Odebug` to optimize for debugging,
but how can you debug? Well you can just use the GDB debugger.

Lets compile again our minimal `hello.c` program and debug it with GDB:

```sh
rivemu -quiet -no-window -sdk -workspace -exec gcc hello.c -o hello-debug '$(riv-opt-flags -Odebug)'
rivemu -quiet -sdk -workspace -it -exec gdb -silent ./hello-debug
```

This should open `gdb` session so you can debug the cartridge,
here is a copy of a session demonstrating how to step over each function:

```c
$ rivemu -quiet -sdk -workspace -it -exec gdb -silent ./hello-debug
Reading symbols from ./hello-debug...
(gdb) break main
Breakpoint 1 at 0x8b0: file hello.c, line 6.
(gdb) run
Starting program: /workspace/hello-debug

Breakpoint 1.1, main () at hello.c:6
6           riv_clear(RIV_COLOR_BLACK);
(gdb) step
8           riv_draw_text("hello world!", RIV_SPRITESHEET_FONT_5X7, RIV_CENTER, 128, 128, 2, RIV_COLOR_WHITE);
(gdb) step
9       } while(riv_present());
(gdb) step

Breakpoint 1.1, main () at hello.c:6
8           riv_clear(RIV_COLOR_BLACK);
(gdb) p riv.frame
$1 = 1
```

Notice in this session we break at the program startup, and then run each line by line.
At the end we print which frame we are with.
In the end, you can debug cartridges normally as debugging C programs.

If you debug a cartridge with a *tape*,
you could inspect the game frame by frame.
Sounds useful for debugging nasty bugs while developing a game right?
For example, imagine a player found a bug in your game,
you could just ask him to record a tape of the bug,
then you can use it to debug frame by frame.

## Inspecting cartridges

What if you got a cartridge from someone and would like to inspect it?
You can use RIVEMU in interactive mode for that.
Lets inspect the `antcopter.sqfs` cartridge
that was previously downloaded in the getting started page:

```sh
$ rivemu -quiet -sdk -workspace -it
rivos-sdk:/workspace# mount antcopter.sqfs /cartridge
rivos-sdk:/workspace# cd /cartridge
rivos:/cartridge# ls -l
total 43
-rwxr-xr-x    1 root     root         42632 Jan  1  1970 antcopter
-rw-r--r--    1 root     root          1006 Jan  1  1970 info.json
rivos:/cartridge# file antcopter
antcopter: ELF 64-bit LSB pie executable, UCB RISC-V, RVC, double-float ABI, version 1 (SYSV), dynamically linked, interpreter /lib/ld-musl-riscv64.so.1, no section header
rivos:/cartridge# jq < info.json
{
  "name": "Antcopter",
  "summary": "A 2D speed run platform game where you are a little ant that can glide in the air",
...
```

The text above is a copy of an interactive session inspecting `antcopter.sqfs`.
You can see it has only 2 files, yhe `antcopter` RISC-V binary that contains the game,
and `info.json` JSON file that contains some metadata,
this metadata is used on RIVES to create a catalog of games,
cartridges are recommended to have this metadata included to distribute them on RIVES.

Go ahead and run `/cartridge/antcopter` command in the interactive terminal,
you should be able to play it.

## Benchmarking cartridges

It's a good practice to measure the performance of your cartridge before distributing it,
for two reasons.
One because you have a quota limit on how much computation a cartridge tape can process,
and also because you want your cartridges execution to be lightweight enough
to run with acceptable performance on different systems the emulator is running on.

RIVEMU has the command `-bench` to help benchmarking your cartridge,
it's recommended to benchmark only your final cartridge `.sqfs` file,
lets benchmark `hello-optimized.sqfs`:

```sh
rivemu -quiet -bench hello-optimized.sqfs
```

You should get an output similar to:

```
[RIVEMU] frame=1 fps=0.97 cpu_cost=4190.38MIPS cpu_speed=372.25MIPS cpu_usage=18.15% cpu_quota=0.07%
[RIVEMU] frame=16 fps=59.99 cpu_cost=2.30MIPS cpu_speed=412.79MIPS cpu_usage=0.56% cpu_quota=0.07%
[RIVEMU] frame=31 fps=59.99 cpu_cost=2.31MIPS cpu_speed=384.98MIPS cpu_usage=0.60% cpu_quota=0.07%
[RIVEMU] frame=46 fps=59.97 cpu_cost=2.32MIPS cpu_speed=407.28MIPS cpu_usage=0.57% cpu_quota=0.07%
[RIVEMU] frame=62 fps=59.99 cpu_cost=2.30MIPS cpu_speed=420.51MIPS cpu_usage=0.55% cpu_quota=0.08%
```

Let's break down this information:
- `frame`: it's the frame processed so far,
cartridges by default usually process 60 frames per second, but this can be customized.
- `fps`: it's how much frames per second your computer is capable of processing,
usually we want this number to be close to 60.
- `cpu_cost`: it's how much CPU instructions costed to process exactly 1 second of computation,
it's measured in MIPS (million instructions per second),
usually we want this number to be low and below `cpu_speed`,
high values means the cartridge is sub optimized,
cartridges must try to keep this value below 128MIPS when running to allow everyone to run it smoothly.
- `cpu_speed`: it's how much CPU instruction per second your computer is capable of emulating,
this value may vary on the system you are running,
RIV is designed to run on system that is at least capable of running 128MIPS,
systems that are not able to emulate at that speed may experience stuttering when playing heavy cartridges.
- `cpu_usage`: it's how much of the CPU the cartridge is using,
if this value is above 90%, you may experience stuttering when playing the cartridge.
- `cpu_quota`: it's how much CPU computation quota has been processed so far,
RIV hardware spec set the quota to be 96 billion CPU instructions,
when CPU quota reaches 100%, the game halts immediately.

If you look closely the first frame always costs more MIPS,
because the machine has to boot and load the game.
After the first frame the CPU quota keeps increasing slowly,
it will increase indefinitely until the game ends.
The FPS should remain stable most of the time.
The CPU speed may vary a lot, because different instructions has different speeds,
and also because your system may be busy processing other programs.
The CPU cost should remain stable for well designed cartridges,
cartridges that increase CPU cost at random frames may experience
stuttering, degrading game play quality.

This RIVEMU web page can also show all this information when testing a cartridge.

The `hello-optimized.sqfs` is not doing much, if you play a heavy cartridge,
like DOOM, you will see numbers similar to:

```
[RIVEMU] frame=1 fps=0.18 cpu_cost=12404.20MIPS cpu_speed=567.54MIPS cpu_usage=11.20% cpu_quota=0.37%
[RIVEMU] frame=10 fps=33.75 cpu_cost=55.21MIPS cpu_speed=619.72MIPS cpu_usage=8.59% cpu_quota=0.38%
[RIVEMU] frame=19 fps=35.99 cpu_cost=56.00MIPS cpu_speed=630.41MIPS cpu_usage=9.13% cpu_quota=0.40%
[RIVEMU] frame=28 fps=33.76 cpu_cost=56.01MIPS cpu_speed=622.77MIPS cpu_usage=8.68% cpu_quota=0.41%
```

So DOOM costs about 56MIPS, still way below the recommended max amount of 128MIPS,
most cartridges processing probably will not cost more CPU than that.

## Customizing SDK to install new tools

You have learned so far how to
create cartridges, compile them, compress them, inspect them, debug them and finally benchmark them.
All of this you did using tools we already provide in the SDK,
however you can also add your own tools and programming languages to the SDK,
while you could use them from outside, using from inside the RIVEMU RISC-V machine
you know the tool will work correctly for RISC-V.

Lets download the Nim programming language, and in the section below we will use it to compile a cartridge.

First let's get into a RIVEMU shell with networking support:

```sh
rivemu -quiet -sdk -workspace -net -persist -it
```

Notice the added `-net` here, this enables networking so you can install packages from the internet.
Notice we also added the `-persist` option,
this will make changes to the SDK persist, even when you exit the machine.
Up until now any changes made in the filesystem were being
discarded after the emulator quits.

Inside a RIV SDK interactive terminal with networking, run `apk add nim` to install Nim compiler,
it will download and install Nim package from Alpine Linux package manager,
and then test it with `nim -v`, here is the session output:

```sh
$ rivemu -quiet -sdk -workspace -net -persist -it
rivos-sdk:/workspace# apk add nim
fetch https://dl-cdn.alpinelinux.org/alpine/edge/main/riscv64/APKINDEX.tar.gz
fetch https://dl-cdn.alpinelinux.org/alpine/edge/community/riscv64/APKINDEX.tar.gz
fetch https://dl-cdn.alpinelinux.org/alpine/edge/testing/riscv64/APKINDEX.tar.gz
(1/3) Installing libucontext (1.2-r3)
(2/3) Installing libucontext-dev (1.2-r3)
(3/3) Installing nim (2.0.2-r0)
Executing busybox-1.36.1-r25.trigger
OK: 321 MiB in 93 packages
rivos-sdk:/workspace# nim -v
Nim Compiler Version 2.0.2 [Linux: riscv64]
Compiled at 2024-04-03
Copyright (c) 2006-2023 by Andreas Rumpf

active boot switches: -d:release
```

You can also customize your own set of tools in the SDK.
Beware that using `-persist` you are modifying the original SDK,
if you want to rollback the stock defaults, just download `rivos-sdk.ext2` again.

But did the changes really persist? Let's check:

```sh
$ rivemu -quiet -no-window -sdk -exec nim -v
Nim Compiler Version 2.0.2 [Linux: riscv64]
Compiled at 2024-04-03
Copyright (c) 2006-2023 by Andreas Rumpf

active boot switches: -d:release
```

Yes, it's there, Nim compiler is now part of our customized SDK.

As you install new stuff to the SDK, its disk will grow
and eventually you might be out of disk space.
To fix this, you can increase the size of SDK disk with the following command:

```sh
rivemu -quiet -no-window -sdk -workspace="$(dirname $RIVEMU_SDK)" -exec resize2fs -f rivos-sdk.ext2 1024M
```

This will set the SDK disk size to 1024M, which should double from its default size.

## Using other programming languages

We can use any programming language to make cartridges,
as long it compiles to a self contained RISC-V ELF binary,
or its cartridge brings together all dependencies it needs.

In last section we installed Nim,
we could have installed any other programming as well.
Since our customized SDK already have Nim,
let's show how you can use Nim to create cartridges,
even though there is no official support for Nim in RIV.

Lets just port our hello example, create this file `hello.nim`:

```python
proc riv_present(): bool {.importc, header: "<riv.h>".}
proc riv_clear(col: uint32): void {.importc, header: "<riv.h>".}
proc riv_draw_text(text: cstring, sps_id: uint64, anchor: uint32, x: int64, y: int64, size: int64, col: int64): void {.importc, header: "<riv.h>".}
const
    RIV_COLOR_BLACK: uint32 = 0
    RIV_COLOR_WHITE: uint32 = 1
    RIV_CENTER: uint32 = 4
    RIV_SPRITESHEET_FONT_5X7: uint64 = 1023

while true:
    # clear screen
    riv_clear(ord(RIV_COLOR_BLACK))
    # draw hello world
    riv_draw_text("hello world!", ord(RIV_SPRITESHEET_FONT_5X7), ord(RIV_CENTER), 128, 128, 2, ord(RIV_COLOR_WHITE))
    if not riv_present():
        break
```

Did you notice this code looks similar to Python?
That is because Nim syntax is inspired by Python syntax,
but don't be fooled, this is a statically compiled programming language.

Let's compile this file with Nim compiler using our customized SDK, and run it:

```sh
rivemu -quiet -no-window -sdk -workspace -exec nim compile --opt:size --mm:none -d:release --passL:-lriv --out:hello-nim hello.nim
rivemu -workspace -exec ./hello-nim
```

You should see a screen with "hello world!" again, but this time the original source code was made in Nim.

We could have installed other system programming languages, such as
Rust, Zig, Odin, Zig, and Nelua.
These are the ones recommended because they are compiled.
You can also use scripting programming languages, which will be covered in the next section.

By the way, the official SDK come for compilers for C, C++ and Nelua.
Nelua even already has bindings for all `riv.h` from `libriv` in the SDK.
The RIV minimal OS also comes with Lua 5.4 interpreter and a JIT for C11
[with MIR JIT](https://github.com/vnmakarov/mir).
But it's recommended to start using programming languages you are already familiar,
learning `libriv` API plus a programming language might be too much to do at once.

## Using scripting languages

In the last section we mentioned that RIV OS comes with Lua programming language,
let's try it.
In other to call functions from the C library `libriv`,
we will use the [cffi-lua](https://github.com/q66/cffi-lua) to import and call C functions.

First create the file `hello.lua` to be used with LuaJIT:
```lua
#!/usr/bin/lua5.4
local ffi = require 'cffi'
ffi.cdef[[
typedef enum riv_color_id {
  RIV_COLOR_BLACK       = 0,
  RIV_COLOR_WHITE       = 1,
} riv_color_id;
typedef enum riv_spritesheet_id {
  RIV_SPRITESHEET_FONT_5X7 = 1023,
} riv_spritesheet_id;
typedef enum riv_align {
  RIV_CENTER = 4,
} riv_align;
typedef struct riv_vec2i {int64_t x; int64_t y;} riv_vec2i;
bool riv_present();
void riv_clear(uint32_t col);
riv_vec2i riv_draw_text(const char* text, uint64_t sps_id, riv_align anchor, int64_t x, int64_t y, int64_t size, int64_t col);
]]
local L = ffi.load("riv")

repeat
  L.riv_clear(L.RIV_COLOR_BLACK)
  L.riv_draw_text("hello world!", L.RIV_SPRITESHEET_FONT_5X7, L.RIV_CENTER, 128, 128, 2, L.RIV_COLOR_WHITE)
until not L.riv_present()
```

Notice in this code the at the beginning we are manually importing `libriv` APIs,
at the bottom it's just again our hello example, lets run it:

```sh
chmod +x hello.lua
rivemu -workspace -exec ./hello.lua
```

And again, you will see the "hello world!" screen!

As mentioned before, Lua is contained the official RIV OS,
but interpreters for other programming languages are not.
If you would like to use Python or JavaScript for example,
you will have to embed their interpreter inside the cartridge which could make it very big,
this is it's recommended to use just official supported interpreted languages,
or use interpreted languages small enough to embed in a cartridge.
There exists tiny versions for both Python and Javascript,
they are [Micropython](https://micropython.org/) and [QuickJS](https://bellard.org/quickjs/),
I would recommend embedding those inside cartridges instead,
maybe we could add official RIV support for them in the future.

If you have read this entire page, you have by now:
- run `hello.c` using a `riv-jit-c`
- run `hello.c` compiled with GCC compiler
- run `hello.c` compiled with GCC compiler with optimizations
- run `hello.nim` compiled with Nim compiler
- run `hello.lua` interpreted with Lua

It's recommended to use C for now,
but if you are adventurous,
you could use other languages as long as work out the bindings.

## Porting other games

You can port existing games to RIV, as long you have the code to compile it to RISC-V,
have all the tools you need in the SDK
and the game works under all the constraints presented so far.
Porting a game basically is a matter of changing the graphics, audio and input APIs
to use `libriv` C API.

The [DOOM cartridge repository](https://github.com/rives-io/cartridge-freedoom)
repository should serve as an example
on how a source code that was originally using the SDL library for handling
graphics/audio/input,
was ported to use the RIV library.

In the next chapter we will learn more details about the libriv API
while creating a Snake game.
