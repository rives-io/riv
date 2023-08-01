# RIV

RIV is a framework for building complex single player games that can be verifiable on-chain.

RIV allows the creation of games that will be run inside a RISC-V Linux system
using the Cartesi Machine emulator.
The Cartesi Machine is not only used for verifying a game replay on-chain with Cartesi Rollups,
but also for playing the game while recording the game input actions locally.

RIV provides a customized Cartesi Machine emulator, powerful enough
to run retro style games with full graphics, audio and keyboard input in real-time
directly in a Web Browser, or in a native Linux or Windows emulator application.

RIV uses the official Cartesi Machine for emulation, and Cartesi Rollups for verifying
recorded game replays on-chain.

## Advantages

### Implement a game only once

Usually when creating verifiable on-chain single player games,
developers have to implement the game twice.
One time for the client side logic that includes all the visuals, sounds and input handling.
And a second time with the on-chain smart contract,
where its sole purpose is to verify if a game replay submission produced a valid game result.

With RIV a game needs to be implemented only once,
because the same game binary is used to play the game locally in real-time using a Cartesi Machine,
and also to verify the game replay using the Cartesi Machine again through Cartesi Rollups.

### Have every tool Linux offers

A game made with RIV runs inside a full Linux operating system,
meaning almost any library or programming language supported by Linux
can be used to create the game.

### Have game verification out of the box

A game made with RIV is deterministic and automatically verifiable.
RIV makes difficult to have different behavior or outcomes between a local game play,
and the game replay verification backend,
because for both cases, the game runs on the same deterministic VM,
environment, operating system, architecture and game binary.

### Have the power of Cartesi Machine for game verification

A game verification performed using Cartesi Machine is orders of magnetite faster
than running a game verification on Ethereum, it's so fast that games made with RIV
can be verified on chain with full rendering, audio and input logic contained.

### Have a game that will work the same on every platform

Traditionally when making a game, developers often want to port the game to run on Windows,
Linux, Web Browser, other platforms and architectures.
Porting a game to every platform can take a tremendous effort,
making sure the game behaves exactly on different platforms even more so.

Every game made with RIV behaves exactly as it was created and compiled,
because it will always run in the same environment, operating system, architecture,
and amount of physical memory.
There are almost no external factors that can affect its behavior and
everything is deterministic.

### Port existing retro games to be verifiable on-chain

RIV can be used to port existing single player games to be verified on-chain,
however the developer has to make an effort to edit original game sources to use
the custom RIV library for graphics, audio and input.

This has been done with the open source release of DOOM for example.

### Compose on top of it

RIV can be used as a component of a larger decentralized application,
it's a way to have proof that someone played a game with a certain outcome.
The proof of a game outcome can be used to create for example tournaments, battles, bets,
mint, or anything on-chain where a proof of a single player game outcome is useful.

### Have a single binary for entire games running on any platform RIV emulator supports

A game made with RIV is a single compressed file containing all games binaries and assets, called cartridge.
This cartridge will run anywhere the RIV emulator runs.
This means there is no need to upgrade a game to add support for a new operating system,
Web Browser, platform or architecture.
The game should work perpetually with RIV as long as there is a working RIV emulator in existence
for the target platform.

## Limitations

### Intended for single player game verification

The proof of concept of this framework is intended for verifying single player games only,
although something like multiplayer interactions can be composed on top of it,
like tournaments.

### Limited resources

RIV only supports software rendering while running inside the Cartesi Machine emulator.

The emulator performance is somewhat similar to a 100MHz CPU when running on a modern computer,
this means game loop and rendering must not use more than 100 millions RISC-V instructions
in a second, otherwise its frame rate will drop.

A high resolution 2D game, or 3D games cannot work under these constraints,
therefore RIV is suitable for making retro games, like we were back in the 2000s,
with limited CPU resources.

Nevertheless, RIV is still powerful enough to run 1996 DOOM
with its full resolution in real-time on a modern computer at 320x200 resolution
using software rendering.

There are ideas on adding support for 3D accelerated rendering,
but this is not planned for this proof of concept.

But hey, it can be fun to work in a game with these constraints!
Some people also see this as a feature, because by making the game scope narrow,
game developers must keep things simple and often ship games faster.

### Custom rendering library

RIV uses a custom library for exchanging graphics, audio and inputs with the host running
its emulator.
Therefore while any library Linux provides can be used for game development,
The final graphics, audio and inputs must be exchanged with the RIV library.

### No mouse tracking input

Currently there is no mouse tracking,
this can be implemented, although not planned for the first proof of concept,
mainly because mouse tracking logs can become too large to send on-chain.

## How it works

A game made with RIV is compiled to a RISC-V binary and packaged with its assets into
a SquashFS compressed filesystem, that is called a *game cartridge*.
The only requirement when building the game is to use the RIV C library for the following:

- Audio
- Graphics
- Keyboard Input
- Time
- Random

It's possible to use other languages than C, as long as there is a binding of the RIV library
for the language, and the language is available for RISC-V Linux.

Once the game is squashed into SquashFS cartridge,
this cartridge can be used to run the game using the RIV emulator locally,
either in a Web Browser version of the RIV emulator or using a native application of the RIV emulator.
The game can be run with a given set of *game parameters*, which can be used to add some variability
to a game play, such as choosing a different game level, difficulty or initial random seed.

The RIV emulator will record all keyboard and mouse inputs while playing the game.
Once the game play is finished the compressed log of the inputs produces
a *game replay* file and also produce the final *game result*,
which typically contains the game outcomes, such as a list of scores or achievements.
Notice this *game result* was produced locally therefore cannot be trusted yet.

The *game replay* recorded locally can later be submitted along with its *game parameters* on-chain
to be verified by a remote Cartesi Machine that will be running in a node of the Cartesi Rollups,
that will also contains the same *game cartridge*, run the replay,
and produce an on-chain notice with the truthful *game result*.
In most cases this truthful *game result* will match the *game result* produced locally,
unless the player tried to cheat or sent an invalid replay.

The *game result* generated by the notice, can then be trusted to be a valid outcome of the game for the
given *game replay*, *game parameters* and *game cartridge*.

With a *game result* that can be trusted, interesting decentralized applications can be
composed on top, such as a tournament application using game results with some value at stake.

## FAQ

### How can RIV generate random numbers while being deterministic?

Typically humans press keystrokes at different timestamps,
it's very hard for a human to press the same set of keystrokes at the same timestamps.
RIV takes advantage of that to generates entropy in its pseudo random generator,
So every input action and timestamp is used to seed the pseudo random generator,
therefore different game plays will have different random numbers generated,
unless it's not a human playing.

It's also possible to give an initial seed as a *game parameter*.

### How RIV deals with stolen game replays?

RIV is a game framework for playing and verifying game replays, not a full application.
This problem must be solved in the application layer that will be using RIV.
A common technique for this problem is to use the commit-reveal strategy,
where someone commits to submit a replay and not reveal it yet, until a submission deadline is reached.

### How RIV deals with bots or IA playing a game?

It's completely possible to make a bot smart enough to surpass human skills
in RIV games by using artificial intelligence.
It's out of the scope of the project to deal with this problem,
it's something to be dealt with in the application that will be using RIV.

I usually think either game developers should embrace this possibility,
or the scope of tournament applications of single player games should be much narrow,
by allowing only humans that are known to not take advantage of artificial intelligence to join,
either by a reputation system, or by running small tournaments in a controlled environment or privately.

### What if a game has a vulnerability, or uses too much CPU? Can it break the Linux system?

RIV games are always run in a sandboxed container using a combination of
[Linux namespaces](https://en.wikipedia.org/wiki/Linux_namespaces),
[Linux cgroups](https://en.wikipedia.org/wiki/Cgroups) and
[Linux seccomp](https://en.wikipedia.org/wiki/Seccomp).
In case a game play or replay uses too much CPU time, or memory, it will be killed to make
sure on-chain verification ends in a limited amount of time, this can be thought of as a metering system.

Each game replay is also run as an unprivileged user in secure sandboxed container,
that has no access to rollups infrastructure and have limited Linux system calls at disposal,
making nearly impossible for a game to takeover the Linux system and affect other verification
in case of an exploit.

This protects the RIV machine against malicious player inputs or malfunctioning games.

### Can I use RIV inside another application using Cartesi?

With the Hypervisor extension being worked on for the Cartesi Machine,
it will be possible in the future to just grab the RIV kernel, rootfs, game cartridges and
run inside another descentralized application. The application will still have to use the original RIV emulator
to produce the game replays locally, also notice that the RIV emulator can work
in the Web Browser with WebAssembly, so you can bundle it in your frontend.

### How RIV can run on the web browser? Is the Cartesi Machine running on the browser?

Yes, the Cartesi Machine was compiled to WebAssembly,
therefore you can run the RIV emulator in a Web Browser.

### Why RIV was created?

RIV is a step towards an on-chain fantasy game console I plan to create using the Cartesi Machine emulator,
it might become a full fantasy game console with proofs on-chain someday, that is the ultimate goal.
I decided to make it as a minimal game framework for Linux RISC-V games first as proof of concept,
to validate its core principles.

Also being a framework other developers could attempt to reuse it on-chain to compose another kind
of decentralized application that requires a proof of a retro style game result.
Or developers could even use off-chain as a new platform to develop games.

### Directory structure

RIV is divided into several components, a kernel, an operation system, an emulator with graphics,
a game library and a sandboxing utility. Here is a list of each folder component:

- `kernel` - RIV Linux custom kernel, it has features enabled for sandboxing games.
- `rootfs` - RIV minimal operating system based on Alpine Linux where games are run, it also builds a RISC-V toolchain compiling games for it.
- `rivcm` - RIV Cartesi Machine, this is the emulator players use to play the games locally with graphics and audio.
- `libriv` - RIV library used as middle-layers, that every game must use for graphics, audio, inputs, time and random.
- `demos` - Simple games made with RIV that are provable on-chain to showcase it.
- `docs` - RIV documentation.
- `bwrapbox` - Sandboxing on top of [bubblewrap](https://github.com/GoogleChromeLabs/bubblewrap) for running games, with additions for limiting CPU usage and memory usage.

Check README of each directory for more details on them.

## Authors

Eduardo Bart
