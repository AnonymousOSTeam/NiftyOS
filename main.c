EFI_STATUS status;
EFI_HANDLE image;
EFI_DEVICE_PATH_PROTOCOL *path;

//status going from 0 to 1 for active trans
status = BS->LoadImage(FALSE, ImageHandle,
DevicePathFromText(L"\\EFI\\GRUB\\grubx64.efi"), NULL,
0, &image);
if (!EFI_ERROR(status)) {
    status = BS->StartImage(image, NULL, NULL);
}
