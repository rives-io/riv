# Developing cartridges

This tutorial will guide on how to setup your
development workflow and how to create your first cartridge.

If you not have installed RIVEMU yet, please read getting started page first.

This tutorial will begin quickly by running your first graphical application inside RIV,
and then expand slowly about all development tools RIV offers to create cartridges,
while you will learn about some RIVEMU options on the go.

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
        riv_clear_screen(RIV_COLOR_BLACK);
        // draw hello world
        riv_draw_text("hello world", RIV_SPRITESHEET_FONT_5X7, 63, 121, RIV_COLOR_WHITE, 2, 2, 2, 2);
    } while(riv_present());
    return 0;
}
```

The standard RIV OS comes with a JIT for C programming language called `riv-jit-c`,
so you can run this example directly without having to actually compile it:

```sh
rivemu -workspace -exec riv-jit-c hello.c
```

You should see a screen popup, and that is it, you first hello world program running in RIV!

The `-exec` option overrides the standard machine entrypoint to execute a custom
command, in this case we are executing `riv-jit-c` to run `hello.c` file.

The C code presented so far should be self explanatory,
we have a line that clear the screen,
another to draw a text and finally a line to present the cartridge frame.
You can read the full *libriv* C API in its
header at [riv.h](https://github.com/rives-io/riv/blob/main/libriv/riv.h),
for now we won't give much details on its functions,
let's continue understanding the RIV development workflow first.

## Interactive terminal with workspaces

Notice in the last example the `-workspace` option,
it mounts the current working directory from your host to `/workspace` directory in the RIV machine, to make development easier.

This is a good time show how RIV is also a Linux, where you can work in its own terminal when combining with the `-it` option:

```sh
rivemu -workspace -it
```

The `-it` options standards for interactive terminal,
it gives ability to type commands inside a terminal running inside RIV OS.
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
But why would you want to run commands inside RIV OS terminal?
Well because the RIV also has the RIV SDK that comes with lot of utilities
for developing cartridges,
tools for compiling RISC-V binaries, compressing files into cartridge archives,
and debuggers for debugging RISC-V applications.

## Installing the SDK

RIVEMU comes with a builtin and minimal operation system called RIV OS,
this RIV OS contains the bare minimal libraries and utilities
to run cartridges, but in other to actually create cartridges
you need many more tools like programming language compilers
and archive compressors.

You may also want to customize the RIV OS SDK with your own Linux tools,
such as other programming languages (e.g Rust, Odin, Zig, Nim..).
With the SDK you can do this, because it's writable and customizable,
while the builtin standard minimal RIV OS is read-only.

Bear in mind the standard SDK is very large (about 500MB),
because it contains compilers, debuggers, editors, and many other Linux tools.

Let's download the SDK and try it:

```sh
wget -O rivos-sdk.ext2 https://github.com/rives-io/riv/releases/latest/download/rivos-sdk.ext2
rivemu -sdk=rivos-sdk.ext2 -exec gcc --version
```

The `-sdk=rivos-sdk.ext2` overrides the standard operating system
to use the SDK operating system,
the command `-exec gcc --version` should show the GCC C compiler version
that is included in the SDK, which was not included in the minimal RIV OS.

You probably got an output like:
```
gcc (Alpine 13.2.1_git20240309) 13.2.1 20240309
Copyright (C) 2023 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

Let now install this SDK globally, to make available in any terminal in your system:
```sh
mv rivos-sdk.ext2 $HOME/.riv/
export RIVEMU_SDK=$HOME/.riv/
echo "export RIVEMU_SDK=$HOME/.riv/" >> ~/.bashrc
```

When setting the environment `RIVEMU_SDK`,
RIVEMU will be able to auto detect and load the SDK,
so you can use from anywhere in your system,
lets try it:

```sh
rivemu -quiet -no-window -sdk -exec riv-tool version
```

You should see and output similar to:
```
0.3.0
```

We have introduced two new command line options here.
The `-quiet` makes RIVEMU less verbose because we just
want to see an output from a command executed inside the machine.
The `-no-window` makes RIVEMU never create a graphical window,
because we want to just run a console application in this case.
The `-sdk` without a filename will try to read the SDK filesystem from `RIVEMU_SDK` environment variable.
The `-exec /usr/lib/libriv.so version` will execute a command to check the `libriv` version in the OS.

## Creating your first cartridge

Now with the SDK installed,
lets get back to the `hello.c` example and actually turn it into a cartridge.

Every cartridge must have an *entrypoint* to actually run,
first let create a shell script file named `0-entry.sh`:

```sh
echo "riv-jit-c hello.c" > 0-entry.sh
chmod +x 0-entry.sh
```

Why the name is `0-entry.sh`? When running a cartridge,
RIV actually executes the first executable file in a cartridge
in alphabetical order.
In case you have other executable binaries in your cartridge,
they could executed before the entrypoint as a mistake,
so it is recommended to make the entrypoint be the first file listed
when ordering its files by name.

Now lets invoke RIVEMU to actually compress both `0-entry.sh` and `hello.c` into
a cartridge archive.

```sh
rivemu -quiet -no-window -sdk -workspace -exec riv-mksqfs 0-entry.sh hello.c hello.sqfs
```

The `riv-mksqfs` command should the actual cartridge, lets inspect it with:

```sh
rivemu -quiet -no-window -sdk -workspace -exec "ls -l hello.sqfs && file hello.sqfs"
```

It should output something similar to:
```sh
-rw-r--r--    1 user     user          4096 Apr 14 19:10 hello.sqfs
hello.sqfs: Squashfs filesystem, little endian, version 4.0, xz compressed, 573 bytes, 3 inodes, blocksize: 131072 bytes, created: Thu Jan  1 00:00:00 1970
```

You can see the cartridge file is 4096 bytes,
this is because cartridges actually are SquashFS compressed filesystem,
which are always saved in multiple of 4KB,
this also means the minimum size for a cartridge is always 4KB.
Noticed the cartridge contents were compressed with *xz* compression,
the compression could also be tuned to generated smaller cartridges.


Now lets finally run it:

```sh
rivemu hello.sqfs
```

If everything worked, you should see you "hello world!" cartridge pop up.
This is your first cartridge!

This cartridge file is also the one you should upload on RIVES.

## Testing cartridges in a web browser

Having a cartridge running locally in the RIVEMU is great,
but most players will probably play your cartridges in a Web Browser,
luckily RIVEMU was also ported to WebAssembly,
RIV has a test page for you to test a cartridge in the browser
at https://rives-io.github.io/riv/ .
Go there search for the "Upload" button in the "Cartridge" section,
and upload your `hello.sqfs`.

If everything works fine, you should be able to see the "hello world!"
in the browser.
Everytime you finish developing a cartridge,
it is recommended to actually test them in the browser,
because the performance can be difference.

Also in the browser you can even use game pads to test your game!
If you have a PS4, Xbox or Nintendo Switch controller,
just plug in your computer and most browser supports them through
the HTML5 Gamepad API specification.
Before trying this, you should check if your gamepad is working in the browser
in third party services
like [this link](https://hardwaretester.com/gamepad).

## Compiling cartridges

For out first `hello.sqfs` we used `riv-jit-c` to run `hello.c`,
although this was very easy we can actually compile a `hello` RISC-V ELF binary:

```sh
rivemu -quiet -no-window -sdk -workspace -exec gcc hello.c -o hello -lriv
```

The command `gcc hello.c -o hello -lriv` compiles `hello.c`
source code into `hello` Linux ELF binary linked to `libriv.so`.

Before generating a cartridge for it, lets test it:

```sh
rivemu -workspace -exec ./hello
```

Then you should see a hello world screen like before.
Now lets actually compile a cartridge for it and run it:

```sh
rivemu -quiet -no-window -sdk -workspace -exec riv-mksqfs hello hello-compiled.sqfs
rivemu hello-compiled.sqfs
```

Again, like before you should see hello world screen,
but this cartridge is different from the first one using that used JIT,
it uses ahead of time compilation,
which makes your cartridge faster to run, and perhaps smaller.
We recommend to use JIT just for  prototyping,
ideally your final product should use compiled cartridges.

## Compiling optimized cartridges

In last example we compiled a C code, but we did not enable any optimization,
lets enable them not only for improve its performance, but also to reduce its final size,
so we can make a small cartridge.
RIV SDK comes with a tool to assist you choosing C compilers flags to
to generate optimized and small binaries called `riv-opt-flags`.

Lets recompile a new cartridge for the last example again using these tools:

```sh
alias rivemu-exec='rivemu -quiet -no-window -sdk -workspace -exec'
rivemu-exec gcc hello.c -o hello-optimized '$(riv-opt-flags -Osize)'
rivemu-exec riv-strip hello-optimized
rivemu-exec riv-mksqfs hello-optimized hello-optimized.sqfs
rivemu hello-optimized.sqfs
```

A lot happened above, let's break it:
1. In the first line we create the alias `rivemu-exec` to make easy for us to invoke commands inside the machine.
2. In the second line we compile `hello.c` again, but this time to `hello-optimized` using compiler flags from `riv-opt-flags -Osize --cflags --ldflags`, this will supply flags to optimized for size because we passed `-Osize`, this tool also accepts `-Ospeed` to optimize for performance, and `-Odebug` for debugging.
3. In the third line we strip unwanted data from the ELF binary `hello-optimized`, this should compress the file a little more.
4. In the forth line we created a cartridge `hello-optimized.sqfs`
5. Finally in the last line we test it.

Lets compare the final with old one we created:

```sh
$ unsquashfs -stat hello-compiled.sqfs | grep "Filesystem size"
Filesystem size 3195 bytes (3.12 Kbytes / 0.00 Mbytes)
$ unsquashfs -stat hello-optimized.sqfs | grep "Filesystem size"
Filesystem size 1573 bytes (1.54 Kbytes / 0.00 Mbytes)
```

Notice the data in `hello-optimized.sqfs` cartridge is smaller than `hello-compiled.sqfs`.
However if you take a look, both cartridges files are still at 4KB,
this is because SquashFS files add 0 padding at the end, to make us able to use `mount` command.
This cartridge was very simple, only in a more complex you would see the benefits.

On RIVES it is recommended to upload optimized and small cartridges to reduce costs,
and improve its performance.
If you can afford, prefer to use `-Ospeed` to optimize for performance,
and only use `-Odebug` to debug while in development.

## Debugging cartridges

In the last example we mentioned `-Odebug` to optimize for debugging,
but how can you debug? Well you can just use the GDB debugger.

Lets compile again our minimal `hello.c` program and debug it with GDB:

```sh
rivemu -quiet -no-window -sdk -workspace -exec gcc hello.c -o hello-debug '$(riv-opt-flags -Odebug)' -ggdb
rivemu -quiet -sdk -workspace -it -exec gdb -silent ./hello-debug
```

This should open `gdb` so you can debug the cartridge,
and here is a session demonstrating how to step over each function:

```sh
$ rivemu -quiet -sdk -workspace -it -exec gdb -silent ./hello-debug
Reading symbols from ./hello-debug...
(gdb) break main
Breakpoint 1 at 0x8b6: file hello.c, line 8.
(gdb) run
Starting program: /workspace/hello-debug

Breakpoint 1.1, main () at hello.c:8
8           riv_clear_screen(RIV_COLOR_BLACK);
(gdb) step
10          riv_draw_text("hello world!", RIV_SPRITESHEET_FONT_5X7, 63, 121, RIV_COLOR_WHITE, 2, 2, 2, 2);
(gdb) step
11      } while(riv_present());
(gdb) step

Breakpoint 1.1, main () at hello.c:8
8           riv_clear_screen(RIV_COLOR_BLACK);
(gdb) p riv.frame
$1 = 1
```

Notice in this session we break at the program startup, and then run each line by line.
At the end we print which frame we are with.
You can debug cartridges normally as debugging C programs.

## Inspecting cartridges

What if you got a cartridge from someone and would like to inspect it?
You can use RIVEMU in interactive mode for that.
Lets inspect the `antcopter.sqfs` cartridge
that was previously downloaded in the getting started page:

```sh
$ rivemu -quiet -sdk -workspace -it
rivos:/workspace# mount antcopter.sqfs /cartridge
rivos:/workspace# cd /cartridge
rivos:/cartridge# ls -l
total 43
-rwxr-xr-x    1 root     root         42632 Jan  1  1970 antcopter
-rw-r--r--    1 root     root          1006 Jan  1  1970 info.json
rivos:/cartridge# file antcopter
antcopter: ELF 64-bit LSB pie executable, UCB RISC-V, RVC, double-float ABI, version 1 (SYSV), dynamically linked, interpreter /lib/ld-musl.so, no section header
rivos:/cartridge# jq < info.json
{
  "name": "Antcopter",
  "summary": "A 2D speed run platform game where you are a little ant that can glide in the air",
...
```

The above is an interactive session output, you can see `antcopter.sqfs` has only 2 files.
The `antcopter` RISC-V binary that contains the game,
and `info.json` JSON file that contains some game metadata,
this metadata is used on RIVES to create a catalog of games,
cartridges are recommended to have include this metadata to distribute them on RIVES.

Go ahead an run `/cartridge/antcopter` command in the interactive terminal,
you should be able to play it.


## Benchmarking cartridges

## Customizing the SDK

TODO

## Using other programming languages
TODO

## Porting other games
TODO