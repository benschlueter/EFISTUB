//* Most Code is copied from https://github.com/karelzak/gummiboot/blob/master/src/efi/gummiboot.c
// and slightly adjusted 

#include "include/efi/efi.h"
#include "include/efi/efilib.h"
#include "include/efi/x86_64/efibind.h"

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

    entry->options = L"PLACEHOLDERCMDLINE";
   	
    entry->loader = L"\\vmlinuz-linux";

    entry->device = &loaded_image->DeviceHandle;

    path = FileDevicePath(*entry->device, entry->loader);

    err = uefi_call_wrapper(BS->LoadImage, 6, FALSE, ImageHandle, path, NULL, 0, &image);

    err = uefi_call_wrapper(BS->OpenProtocol, 6, image, &LoadedImageProtocol, &loaded_image, ImageHandle, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
    
    loaded_image->LoadOptions = entry->options;
    loaded_image->LoadOptionsSize = (StrLen(loaded_image->LoadOptions)+1) * sizeof(CHAR16);

    err = uefi_call_wrapper(BS->StartImage, 3, image, NULL, NULL);
    if (EFI_ERROR(err)) {
        Print(L"Error getting LoadedImageProtocol handle: %r", err);
        uefi_call_wrapper(BS->Stall, 1, 3 * 1000 * 1000);
    }
    uefi_call_wrapper(BS->UnloadImage, 1, image);
    FreePool(entry);
    FreePool(path);
    return err;
}
