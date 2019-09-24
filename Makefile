ARCH            = x86_64
CC				= gcc
SRC 			= main.c
OBJS            = efistub.o
TARGET          = efistub.efi

EFIINC          = include/efi
EFIINCS         = -I$(EFIINC) -I$(EFIINC)/$(ARCH) -I$(EFIINC)/protocol
LIB 			= lib#/usr/lib64
EFILIB          = lib#/usr/lib
EFI_CRT_OBJS    = $(EFILIB)/crt0-efi-$(ARCH).o
EFI_LDS         = $(EFILIB)/elf_$(ARCH)_efi.lds

CFLAGS          = $(EFIINCS) -fno-stack-protector -fpic -fshort-wchar -mno-red-zone -Wall 
ifeq ($(ARCH),x86_64)
  CFLAGS += -DEFI_FUNCTION_WRAPPER
endif

LDFLAGS         = -nostdlib -znocombreloc -T $(EFI_LDS) -shared -Bsymbolic -L $(EFILIB) -L $(LIB) $(EFI_CRT_OBJS) 

all: $(TARGET)

efistub.o:
		$(CC) -g -O3 -c $(SRC) $(CFLAGS) -o efistub.o

efistub.so: $(OBJS)
	ld $(LDFLAGS) $(OBJS) -o $@ -lefi -lgnuefi

%.efi: %.so
	objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym  -j .rel -j .rela -j .reloc --target=efi-app-$(ARCH) $^ $@

clean:
	rm $(TARGET) $(OBJS) efistub.so main.c

install: $(TARGET)
		mkdir -p $(DESTDIR)/boot/
		mv $(TARGET) $(DESTDIR)