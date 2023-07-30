# RIV

RIV is a framework for building complex single player games that can be verifiable on-chain.

RIV allows to create games that will be run inside a RISC-V Linux system
using the Cartesi Machine emulator.
The Cartesi Machine is not only used for verifying a game replay on-chain with Cartesi Rollups,
but also for playing the game while recording the game input actions.

RIV provides a customized Cartesi Machine emulator, powerful enough to
to run retro style games with full graphics, audio and keyboard input in real-time
directly in a Web Browser, or in a native Linux or Windows application.

RIV uses the official Cartesi Machine emulator for emulation, and Cartesi Rollups for verifying
game replays recorded locally on-chain.

## Advantages

### Implement the game only once

Usually when creating verifiable on-chain single player games,
developers has to implement the game twice.
One time for the client side logic that includes all the visuals, sounds and input handling.
And a second time with the on-chain smart contract, where its sole purpose is to verify if a game replay
submission produced a valid game result.

With RIV a game needs to be implemented only once,
because the same game binary is used to play the game locally in real-time using a Cartesi Machine,
and to verify the game replay using Cartesi Rollups.

### Have every tool Linux offers

A game made with RIV runs inside a full Linux operation system,
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
making sure the game behave exactly in different platforms is also difficult.

Every game made with RIV behaves exactly as it was created and compiled,
because it will always run in the same environment, operating system, architecture,
and amount of physical memory.
There are almost no external factors that can affect its behavior,
everything is deterministic, the only external factor are the player inputs.

### Port existing retro games to be verifiable on-chain

RIV can be used to port existing single player games to be verified on-chain,
however the developer has to make an effort edit original game sources to use
the custom RIV library for graphics, audio and input.

This has been done with the open source release of DOOM for example.

### Compose on top of it

RIV can be used as a component of a larger decentralized application,
it's a way to have a proof that someone played a game with certain outcome.
The proof of a game outcome can be used to create for example tournaments, battles, bets,
mint, or anything on-chain where a proof of a single player game outcome is useful.

### Have a single binary for entire games running on any platform RIV emulator supports

A game made with RIV is a single compressed file containing all games binaries and assets, called cartridge.
This cartridge will run anywhere RIV emulator runs.
This means no need to upgrade your game to add support for a new operating system,
Web Browser, platform or architecture.
The game should work perpetually with RIV as long there is a working RIV emulator in existence
for the platform you care.

## Limitations

### Intended for single player game verification

The proof of concept of this framework is intended for verifying single player games only,
although something like multiple players interactions can be composed on top of it,
like tournaments.

### Limited resources

RIV only supports software rendering while running inside the Cartesi Machine emulator.

The emulator performance is somewhat similar to a 100MHz CPU when running on a modern computer,
this means game loop and rendering must not use more than 100 millions RISC-V instructions
in a second, otherwise its frame rate will drop.

A high resolution 2D game, or 3D games cannot work under this constrains,
therefore RIV is suitable for making game retro like we are back in the 2000s,
with limited CPU resources.

Nevertheless, RIV still powerful enough to run 1996 DOOM
with its full resolution in real-time on a modern computer at 320x200 resolution
using software rendering.

There are ideas on to add support for 3D accelerated rendering,
but this is not planned for this proof of concept.

But hey, can be fun to work on game under these constrains!
Some people also see this a feature, because by making the game scope narrow,
game developers often ships a game faster.

### Custom rendering library

RIV uses a custom library for exchanging graphics, audio and inputs with the host running
its emulator.
Therefore while any library Linux provides can be used for game development,
the final graphics, audio and inputs must be exchanged with the RIV library.

### No mouse tracking input

Currently there is no mouse tracking,
this can be implemented, although not planned for the first proof of concept,
mainly because mouse tracking logs can become too large to sent on chain.

## How it works

A game made with RIV is must be compiled to a RISC-V binary and bundled with its assets into
a SquashFS filesystem, that is called a *game cartridge*.
The only requirement when building the game is to use the RIV C library for the following:

- Audio
- Graphics
- Keyboard Input
- Time
- Random

It's possible to use other languages than C, as long there is a binding of the RIV library
for the language.

Once the game is squashed into SquashFS cartridge,
this cartridge can be used to run the game using the RIV emulator locally,
either in a Web Browser version of the RIV emulator or using a native application of the RIV emulator.
The game can be run with a given set of *game parameters*, which can be used to add some variability
to a game play, such as choosing a different game level, difficulty or initial random seed.

The RIV emulator will record all keyboard and mouse inputs while playing the game.
Once the game play is finished the compressed log of the inputs produces
a file that is called a *game replay* and also produce the final *game result*,
which typically contains the game outcomes, such as a list of scores or achievements.
Notice this *game result* was produced locally therefore cannot be trusted yet.

The *game replay* recorded locally can later be submitted along with its *game parameters* on-chain
to be verified by a remote Cartesi Machine that will be running in a node of the Cartesi Rollups,
that will also contains the same *game cartridge*, run the replay,
and produce a notice with the truthful *game result*.
In most case this truthful *game result* will match the *game result* produced locally,
unless the player tried to cheat.

The *game result* generated by the notice, can then be trusted to be a valid outcome of the game for the
given *game replay*, *game parameters* and *game cartridge*.

With a *game result* that can can trusted, interesting decentralized applications can be
composed on top, such as a tournament using the game result with some value at stake.

## FAQ

### How can RIV generate random numbers while being deterministic?

Typically humans press keystrokes at different timestamps,
it's very hard for a human to press he same set of keystrokes at the same timestamps.

RIV takes advantage of that to generates entropy in its pseudo random generator,
so every input action and timestamp is used to seed the pseudo random generator,
therefore different game plays will have different random numbers generated, unless a bot is playing.

It's also possible to give an initial seed as a *game parameter*.

### How RIV deals with repeated game replays?

RIV is a game framework for verifying game replays, but not a fully decentralized application.
This problem must be solved in the application layer that will be using RIV.
A common technique for this problem is to use the commit-reveal strategy,
where someones commit to submit a replay and not reveal it yet, until a submission deadline is reached.

### How RIV deals with bots or IA playing the game?

It doesn't, it's completely possible to make a bot smart enough to surpass human skills
in RIV games by using artificial intelligence.
It's out of the scope of the project to deal with this problem,
it's something to be dealt with in the application that will be using RIV.

But I like to think either game developers should embrace this possibility,
or make scope of tournament applications of single player games much narrow,
by allowing only humans that are known to not take advantage of artificial intelligence to join,
either by a reputation system, or by running small tournaments in a controlled environment.

### Why RIV was created?

RIV is a step towards a on-chain fantasy game console I plan to create using the Cartesi Machine emulator,
it might become a full fantasy game console with proofs on-chain someday, that is the ultimate goal.
I decided to make it as a minimal game framework first as proof of concept,
just to validated its core principles.

Also being a framework other developers can attempt to reuse it on-chain to compose other kind
of decentralized application that requires a proof of a retro style game result.

### Directory structure

RIV is divided in several components, a kernel, an operation system, an client side emulator, a game library for graphics, and more. Here is a list of each component:

- `rom` - Cartesi Machine ROM used on boot
- `kernel` - RIV Linux kernel, with custom configs for sandboxing games.
- `rootfs` - RIV minimal operating system based on Alpine Linux where games are run.
- `toolchain` - RISC-V toolchain based on Alpine Linux, required for building games for RIV.
- `rivcm` - RIV Cartesi Machine, this is the emulator players use to run the game locally.
- `libriv` - RIV middle-layers library that every game must use to exchange game graphics, audio and inputs.
- `docs` - RIV documentation.
- `demos` - Simple games made with RIV that are provable on-chain to showcase it.
- `bwrapbox` - Utility for sandboxing games on top of [bubblewrap](https://github.com/GoogleChromeLabs/bubblewrap), with additions for limiting cpu usage and memory usage.

Check README of each directory for more details on them.

## Authors

Eduardo Bart
