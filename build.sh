#FILES="\
#       drivers/ata \
#       drives \
#       input \
#       interrupts \
#       io \
#       memory \
#       string \
#       math \
#       drivers/video \
#       drivers/pci \
#       drivers/ahci \
#       KERNEL_MAIN \
#       terminal \
#       terminalPrograms \
#       advanced_string \
#       mandelbrot \
#       infoPrinters \
#       deviceManager \
#       drivers/multimedia"

FILES="\
       interrupts \
       memory \
       io \
       taskmanager \
       drivers/video \
       string \
       KERNEL_MAIN"

echo "Deleting old files"
rm build/bootsector.bin
rm build/kernel.bin
rm LaciOS.bin

for file in $FILES
do
    rm build/$file.o
done

echo "Compiling"
nasm boot/bootsector.asm -f bin -o build/bootsector.bin
nasm kernel/src/interrupts.asm -o build/interrupts_asm.o -f elf32

for file in $FILES
do
    gcc -ffreestanding -c kernel/src/$file.c -o build/$file.o -m32 -fno-pie -fno-pic -ffunction-sections -static-libgcc -lgcc -Wno-unused-variable -mno-red-zone
done

gcc -ffreestanding -c kernel/src/loader.c -o build/loader.o -m32 -fno-pie -fno-pic -ffunction-sections -static-libgcc -lgcc -Wno-unused-variable -mno-red-zone

#as kernel/src/interrupts.s -o build/interrupts_asm.o --32
#as kernel/src/kernel_start.s -o build/kernel_start.o --32

echo "Linking"

TMP="build/interrupts_asm.o"

for file in $FILES
do
    TMP=$TMP\ build/$file.o
done


ld -o build/kernel.bin -T linker.ld -Ttext 0x100000 $TMP -m elf_i386 --oformat binary
ld -o build/loader.bin -T linker.ld -Ttext 0x7e00 build/loader.o build/io.o -m elf_i386 --oformat binary

echo "Loader file size: "
wc build/loader.bin | awk '{print $3}'
truncate build/loader.bin -s 29184
echo "Kernel file size: "
wc build/kernel.bin | awk '{print $3}'
truncate build/kernel.bin -s 61440

cat build/bootsector.bin build/loader.bin build/kernel.bin > LaciOS.bin