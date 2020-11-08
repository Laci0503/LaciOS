clear
powershell.exe -File ".\\stopvb.ps1"
sh build.sh
rm Lacios.iso
truncate LaciOS.bin -s 1M
VBoxManage convertfromraw LaciOS.bin Lacios.iso
cp ./Lacios.iso /mnt/d/Lacios.iso
powershell.exe -File ".\\startvb.ps1"