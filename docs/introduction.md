# Introduction

## What is RIV?

RIV is a low-level verifiable fantasy console for building small games that runs in special RISC-V Linux machine.

RIV can run *cartridges*, which are small self-contained compressed files
that can be built with any programming language that compiles to RISC-V.

RIV is a verifiable fantasy console, and most likely the first of its kind.
Actions of game plays can be recorded into *tapes* to later be deterministically replayed,
allowing to be used by blockchain smart contracts to verify game *outcomes*.
This opens the possibility of creating new interesting decentralized gaming applications,
such as a tournament platform without a central authority.
*Tapes* can also be used to store game plays in a very compressed file,
to later be re-watched and processed.

RIV is the core component for verifying game plays in RIVES.
But it's decoupled from it, so it can be used a standalone as game console emulator.

RIV has its official emulator called RIVEMU designed to be portable for any platform, currently it can run on WebAssembly, Linux and Windows. Soon also to be ported to MacOS.
RIVEMU is powerful enough to run retro style games with full graphics, audio and keyboard input in real-time directly in a Web Browser, Linux or Windows emulator application.
RIVEMU embeds the Cartesi Machine emulator,
a special kind of emulator capable of efficiently running a full RISC-V architecture while allowing its execution to be strongly deterministic and its computations to bisected,
so machine state transitions can be used for verification in blockchain smart contracts for fraud proofs.

## Console Specs

Fantasy consoles usually force developers to work narrow limitations.
The idea is that by removing excess and focusing on the essential, it becomes easier to start and finish developing a game.
This is in contrast to large game engines like Unity, Unreal,
which can be daunting and distracting.
RIV imposes many limitations,
although not very strict like other fantasy consoles.

- Display: 256x256 pixels by default, customizable, maximum of 1280x720
- Frame rate: 60Hz by default, customizable, maximum of 60Hz
- Colors: 32 by default, customizable, maximum of 256
- Audio: 48kHz sample rate, 2 channels (stereo), maximum of 32 simultaneous sounds
- Input: 16 digital button game pad or 105 buttons keyboard, up to 4 game pads for local co-op, no support for mouse or analog buttons
- CPU: RISC-V 64-bit CPU, base clock of 128MHz, up to 512MHz+ depending the system the emulator is running on, games are recommended to push up to 128 million instructions per second so everyone can play smoothly
- GPU: no accelerated graphics, CPU software rendering
- Operating system: RIV OS, a custom Linux distribution based on Alpine Linux
- Processing limit: 96 billion RISC-V instructions, enough for ~30min session of DOOM or ~120min session of Antcopter
- Memory limit: 96MB
- Cartridge size limit: 8MB, RIVES further limits to 512KB, cartridges are compressed

## Components

RIV is a fantasy console divided in several components, which are:

- RIVEMU: It's emulator, used for running and developing cartridges
- libriv: It's library used for developing games, provides a public API and its implements a driver responsible communication graphics, audio and inputs with the emulator is running.
- RIV OS: It's minimal operating system based on Alpine Linux, the environment where every cartridge runs.
- RIV OS SDK: It's operation system with additional utilities for developing cartridges, contains programming language compilers, debuggers and other tools.
