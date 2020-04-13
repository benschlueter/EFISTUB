# EFISTUB
Some Notebooks e.g my XPS 9360 do not pass the kernel-commandline to the image if I try to boot with Efistub.
Resulting in a kernel panic. There are 2 options to fix it.

The first one is to recompile a custom kernel with my Laptop what kinda sucks. 

# The second is to pass the commandline somehow to the kernel.

There are options to do so but they use systemd-boot and in my opinion there is no reason for it.
One can just install systemd-boot and set timeout to 0. And the 243 patch caused ugly messages and I want a clean boot.

So I decided to write a small wrapper, which is a small UEFI programm with builin kernel-commandline which executes the kernel and passes the commandline to it.

It works flawless on arch, it should work with other distros just change the kernel-image name or the c code.

If you have any problems to set it up feel free to ask. 

# How to install

```
git clone https://github.com/Kakashiiiiy/EFISTUB.git
cd EFISTUB
./configure
make
mv efistub.efi /boot/
efibootmgr -c -d /dev/sd* -p *PARTITIONNUMBER* -l 'efistub.efi' -L 'Arch-EFI' 
```

On Arch Linux:
```
git clone https://github.com/Kakashiiiiy/EFISTUB.git
cd EFISTUB
makepkg -si
efibootmgr -c -d /dev/sd* -p *PARTITIONNUMBER* -l 'efistub.efi' -L 'Arch-EFI' 
```

enjoy.

If you do not trust my precompiled binaries or want the uptodate version of them change 
```
LIB = lib -> /usr/lib64
EFILIB = lib -> /usr/lib
```
in the Makefile. Make sure you've installed gnu-efi-libs for Arch or the specific package for your distribution.
