# Quick start

This tutorial will guide step by step on how to install RIVEMU,
play cartridges, record tapes, replay tapes and verify tapes.

It's mandatory for those trying to grasp what RIV is,
before actually developing games for it.

This tutorial will assume you are running commands in a POSIX compatible terminal emulator, such as the ones found natively on Linux and MacOS.
If you don't have this set up yet in your system yet,
I recommend setting it up first.
For Windows you could check out [MSYS2](https://www.msys2.org/).

## Installing

RIVEMU is all you need to install in your system for both playing and developing cartridges.
To install RIVEMU you just need to download its binary for your system
at [GitHub releases page](https://github.com/rives-io/riv/releases/latest).

For example, if you are on a system with Linux x86_64, you can download with:

```sh
wget -O rivemu https://github.com/rives-io/riv/releases/latest/download/rivemu-linux-amd64
chmod +x rivemu
```

After downloading, check if it's working properly with the `-version` command:

```sh
./rivemu -version
rivemu 0.3.0
libriv 0.3.0
platform: linux
architecture: amd64
build date: 2024-04-13 19:54:03 GMT
```

If you see a similar output, it should be working.

To make `rivemu` available in any shell in your system,
I recommended installing it in your system environment `PATH`:

```sh
mkdir -p $HOME/.riv
mv rivemu $HOME/.riv/
echo 'export PATH=$HOME/.riv:$PATH' >> $HOME/.bashrc
export PATH=$HOME/.riv:$PATH
```

This way you should be able to type `rivemu` from any working directory in your system.
The rest of this tutorial will assume `rivemu` is available in your `PATH`.

## Playing a cartridges

To play a cartridge you just need to download its respective `.sqfs` file
and call `rivemu` using it as the first argument:

```sh
wget -O antcopter.sqfs https://rives-io.github.io/riv/cartridges/antcopter.sqfs
rivemu antcopter.sqfs
```

After executing the above commands a window with the Antcopter game should pop up.
You should be able to play it using arrows and Z X keys.

## Recording and replaying tapes

With RIV you can record gameplays sessions, we call them *tapes*,
you can record a tape with `-record` option,
and you can replay a tape with `-replay` options.

Let's record a tape for Antcopter:

```sh
rivemu -record=antcopter.rivtape antcopter.sqfs 1
```

Did you notice the `1` added argument?
For this specific cartridge this argument will change the game *parameters*,
it will set the number of lifes to 1, so go ahead and play until you die.

After you die the file `antcopter.rivtape` will be saved,
and can re-watch you game play with:

```sh
rivemu -replay=antcopter.rivtape antcopter.sqfs 1
```

You will watch you die again.
You can even replay in 2x speed with:

```sh
rivemu -replay=antcopter.rivtape -speed=2 antcopter.sqfs 1
```

If you inspect the tape file, it is very small,
it's much cheaper to store just this file rather than a gameplay video.

## Viewing tape outcards

Did you notice you had a score on the last screen when you died in Antcopter in last section?
This score is also the game outcome, we call it *outcard*,
you can replay and print the *outcard* from a *tape* you recorded with:

```sh
rivemu -replay=antcopter.rivtape -no-window -print-outcard -print-outhash antcopter.sqfs 1
```

Notice the `-no-window` option was added,
because we just want to show the game final outcard,
so there is no need to render every game frames at all,
with this option the game is fast forwarded until it ends.

When executing the above command, the output should have something similar to:
```
[RIVEMU] Outcard:
JSON{"score":23193,"level":2,"berries":4,"frames":407,"deaths":1,"finished":true}
```

An *outcard* in case of Antcopter is a JSON with a list of scores,
but really could be any kind of file, like an image, or a state for saving the game.

Notice in this outcard, we can see the amount of deaths, frames and berries for this *tape*.
In RIVES for instance this information can be used to create a applications that uses
this information in smart contracts.

## Verifying tape outcards

Notice in the last example we also added the option `-print-outhash`,
this option should have printed a SHA-256 hash of the *outcard* like the following:

```
[RIVEMU] Outcard sha256sum:
b30bbe797707c9d04c675ecbfbc862585b959b0d45ecb9aee9197c137eafffb7
```

This hash can be used to verify of tape outcomes did match an expected outcome,

```sh
rivemu -verify=antcopter.rivtape -verify-outhash=b30bbe797707c9d04c675ecbfbc862585b959b0d45ecb9aee9197c137eafffb7 -no-window antcopter.sqfs 1
```

If the outcard matches you should get the following output:
```
[RIVEMU] Outcard sha256sum verification SUCCEEDED!
```

In case you use the same hash of the above text,
of course it will not match with the tape you recorded locally,
change to the expected hash to see the `SUCCEEDED!` message.

Notice that in order to verify a tape
you needed to have the combination of the *cartridge*, *tape*, *parameters* and *outcard* hash.
RIVES for instance store all this information in smart contracts in order to verify tape outcomes.

Tapes can also be very useful while developing games,
to check if game behavior is not impacted while you modify the game.
You could create a test suite of tapes along with their respective outcard hashes,
and then verify if they match while you do cosmetic changes in your game.

## Other options

RIVEMU has many other options, mostly useful for developing cartridges,
you can check the `-help` option to see its list.
If you are interested learning more about them,
go to the next chapter were we get into actually developing your first cartridge
and learn about many options along the way.
