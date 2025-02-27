rem qemu-system-aarch64 -M raspi3b -vga cirrus -m 1G -kernel kernel.img -dtb bcm2710-rpi-3-b.dtb -drive file=disk.img,format=raw,if=sd 
@REM qemu-system-arm -M raspi2b -m 1G -kernel boot.bin -initrd kernel.bin
rem qemu-system-arm -m 256 -M raspi2 -serial stdio -kernel
rem arm-none-eabi-gcc

rem qemu-system-arm -m 1G -M raspi2b -serial stdio -kernel kernel.bin
@REM -d in_asm

qemu-system-arm -M raspi2b -m 1G -serial stdio -device loader,file=boot.bin,addr=0x8000
@REM qemu-system-arm -M raspi2b -m 1G -serial stdio -kernel boot.elf
