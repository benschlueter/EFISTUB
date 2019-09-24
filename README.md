# EFISTUB
Some Notebooks e.g my xps 9360 do not pass the kernel-commandline to the image if I try to boot with efistub.
Resulting in a kernel panic. There are 2 options now.

The first is to rebuild the kernel with my Laptop what kinda sucks. 

# The second is to pass the commandline somehow to the kernel.

There are options to do so but they use the systemd and in my opinion there is no reason to do so.
One can just install bootctl and set timeout 0. And the 243 patch caused ugly messages and I want a clean boot.

So I decided to write a small wrapper, which is in fact a UEFI programm with builin kernel-commandline which executes the kernel.

It works flawless on arch, it should work with other distroy just change the kernel-image name or the c code.

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
