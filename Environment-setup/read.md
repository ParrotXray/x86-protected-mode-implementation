## Preparing the development environment

### Task list
- Building a pure GCC cross-compilation toolchain (i686-elf), not bundled with the system.
- Building/Installing Bochs and QEMU.

### Compiling i686-elf-gcc
Open the terminal and execute the [gcc-build.sh](gcc-build.sh) script; this may take about half an hour.
```sh=
bash ./gcc-build.sh
```
**Permanently add the newly compiled GCC to the PATH environment variable**

Permanently add the newly compiled GCC to the PATH environment variable. The configuration file depends on the shell you are using, such as `${HOME}/.zshrc` for zsh or `${HOME}/.bashrc` for bash. Alternatively, you can directly modify the global `/etc/profile` file (though it's not recommended). Afterward, reboot your system.
```sh=
vim ~/.bashrc
source ~/.bashrc
```
Add `export PATH="$HOME/cross-compiler/bin:$PATH"`

**Test the command**
```sh=
i686-elf-gcc --version
```
output
```sh=
i686-elf-gcc (GCC) 11.2.0
Copyright (C) 2021 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```
### Compiling Bochs
Download the [source code](https://sourceforge.net/projects/bochs/files/bochs/2.8/), extract it, and place the [bochs-build.sh](bochs-build.sh) script in the folder. Then, execute the script in the current folder.
```sh=
sudo apt install libx11-dev xserver-xorg-dev xorg-dev -y
sudo apt install libsdl2-dev -y
sudo apt install libncurses5-dev -y
sudo apt install xorriso -y
sudo apt install mtools -y
sudo apt install qemu-system-i386 -y
bash ./bochs-build.sh
```
**Test the command**
```sh=
bochs
```
output
```sh=
========================================================================
                        Bochs x86 Emulator 2.7
              Built from SVN snapshot on August  1, 2021
                Timestamp: Sun Aug  1 10:07:00 CEST 2021
========================================================================
00000000000i[      ] BXSHARE not set. using compile time default '/usr/local/share/bochs'
------------------------------
Bochs Configuration: Main Menu
------------------------------

This is the Bochs Configuration Interface, where you can describe the
machine that you want to simulate.  Bochs has already searched for a
configuration file (typically called bochsrc.txt) and loaded it if it
could be found.  When you are satisfied with the configuration, go
ahead and start the simulation.

You can also start bochs with the -q option to skip these menus.

1. Restore factory default configuration
2. Read options from...
3. Edit options
4. Save options to...
5. Restore the Bochs state from...
6. Begin simulation
7. Quit now

Please choose one: [2] 7

```

