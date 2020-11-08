sh build.sh 2> errors.txt

echo "Starting"
qemu-system-x86_64 -hda LaciOS.bin \
-m 3583M \
-device ahci,id=ahci,bus=pci.0,addr=1f,multifunction=on \
-device ide-hd,drive=disk,bus=ahci.0 \
-drive id=disk,file=LaciOS.bin,if=none,format=raw \
#-soundhw hda \
#-drive id=disk,file=LaciOS.bin,if=none,format=raw -device ide-hd,drive=disk,bus=ahci.0 # -device ide-hd,drive=disk2,bus=ahci.1 #-device pci-bridge,id=pci_bridge1,bus=pci.0,chassis_nr=1 -device ahci,id=ahci,bus=pci_bridge1 #-device ahci,id=ahci -device ide-hd,drive=disk,bus=ahci.0
