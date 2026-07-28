# Plaits

## Author

Emilie Gillet, port by Ewan Hemingway

## Description

Work in progress port of Mutable Instruments Plaits to Patch.init(). 

Clone with `git clone --recurse-submodules`, or initialize an existing checkout
with `git submodule update --init --recursive`. Then build the tracked
dependencies and firmware together with `make`.

## Programming

The firmware uses the Daisy bootloader's `BOOT_SRAM` application type so the
Plaits chord engine is not constrained by the Patch SM's 128 KiB internal
flash. The application is stored in QSPI flash and copied into SRAM by the
bootloader when it starts.

### Install the bootloader once

Enter the STM32 ROM bootloader by holding BOOT while pressing and releasing
RESET, then run:

```sh
make program-boot
```

Alternatively, install it with the
[Daisy Web Programmer](https://electro-smith.github.io/Programmer/). This is
normally a one-time setup. Repeat it only if the Daisy bootloader is erased or
replaced, for example by programming a normal `BOOT_NONE` application into
internal flash, or if you intentionally want to update the bootloader.

`dfu-util` can report an error while requesting status after a successful
download and leave request because the board has already reset. If the entire
bootloader file was downloaded successfully, continue with the application
upload.

### Upload this application

For this and subsequent firmware uploads, do not enter the STM32 ROM
bootloader again:

1. Press and release RESET without holding BOOT.
2. During the Daisy bootloader's breathing-LED grace period, press BOOT without
   pressing RESET to keep the bootloader waiting.
3. Run `make program-dfu`.
4. Reset or power-cycle the Patch SM to start the application.

A Daisy bootloader upload reports a `Flash` interface, a 4096-byte transfer
size, and writes the application to `0x90040000`. If `dfu-util` instead reports
`Internal Flash` with a 1024-byte transfer size, the board is in the STM32 ROM
bootloader and cannot write that address. Press RESET alone and retry during
the Daisy bootloader grace period.

## Controls

| Pin Name | Pin Location | Function | Comment |
| --- | --- | --- | --- |
| CV_1 | C5 | Coarse Tuning | Sets the pitch of the oscillator |
| CV_2 | C4 | Harmonics | Harmonics |
| CV_3 | C3 | Timbre | Sets the pitch of the oscillator |
| CV_4 | C2 | Morph | Sets the pitch of the oscillator |
| B7 | B7 | Advance model | Cycles through the engines sequentially |
| B8 | B8 | Use Plaits LPG | As we can't detect if input is used, this toggle is used to signal "Trigger is patched" |
| Gate In 1 | B10 | Trigger LPG  | If toggle is active, trigger LPG |
| Gate In 2 | B9 | Advance model  | Advance the plait engine model |
| CV_5 | C6 | 1V/Octave Input | Musical pitch tracking input |
| Audio Out Left | B2 | Main out | ... |
| Audio Out Right | C1 | Aux out | ... |

### License

This program is free software: you can redistribute it and/or modify it under the terms of the [GNU General Public License](https://www.gnu.org/licenses/gpl-3.0.en.html) as published by the [Free Software Foundation](https://www.fsf.org/), either version 3 of the License, or (at your option) any later version.

**Dependencies** included in the `eurorack` submodule (by Emilie Gillet) are licensed as follows

```

Copyright 2014-2019 Emilie Gillet.

Author: Emilie Gillet (emilie.o.gillet@gmail.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

See http://creativecommons.org/licenses/MIT/ for more information.


```
