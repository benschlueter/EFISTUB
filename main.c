//* Most Code is copied from https://github.com/karelzak/gummiboot/blob/master/src/efi/gummiboot.c
// and slightly adjusted 

#include <efi/efi.h>
#include <efi/efilib.h>
#include <efi/x86_64/efibind.h>

//#define DEBUG

typedef struct {
        VOID *options;
        EFI_HANDLE *device;
        CHAR16 *loader;

} ConfigEntry;

EFI_STATUS EFIAPI efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
   	InitializeLib(ImageHandle, SystemTable);
   	ConfigEntry *entry;
   	EFI_STATUS err;
   	EFI_DEVICE_PATH *path;
   	EFI_LOADED_IMAGE *loaded_image;
   	EFI_HANDLE image;

   	entry = AllocatePool(sizeof(ConfigEntry));

   	err = uefi_call_wrapper(BS->OpenProtocol, 6, ImageHandle, &LoadedImageProtocol, &loaded_image, ImageHandle, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);

   	entry->options = L"initrd=\\intel-ucode.img initrd=\\initramfs-linux.img root=UUID=3263e30d-3412-4563-9f9a-90a095306d7e rw elevator=noop quiet loglevel=3 rd.udev.log_priority=3 rd.systemd.show_status=auto systemd.show_status=auto systemd.fsck=skip amdgpu.dc=1";
   	
   	entry->loader = L"\\vmlinuz-linux";

   	entry->device = &loaded_image->DeviceHandle;

   	path = FileDevicePath(*entry->device, entry->loader);

   	err = uefi_call_wrapper(BS->LoadImage, 6, FALSE, ImageHandle, path, NULL, 0, &image);

   	err = uefi_call_wrapper(BS->OpenProtocol, 6, image, &LoadedImageProtocol, &loaded_image, ImageHandle, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);

   	loaded_image->LoadOptions = entry->options;
   	loaded_image->LoadOptionsSize = (StrLen(loaded_image->LoadOptions)+1) * sizeof(CHAR16);

   	err = uefi_call_wrapper(BS->StartImage, 3, image, NULL, NULL);

   	return err;
}
