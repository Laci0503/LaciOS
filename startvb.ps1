cd "C:\Program Files\Oracle\VirtualBox"
.\VBoxManage.exe storageattach "Test" --storagectl AHCI --port 0 --type hdd --medium none
.\VBoxManage.exe closemedium disk "D:\Lacios.iso"
.\VBoxManage.exe storageattach "Test" --storagectl AHCI --port 0 --type hdd --medium "D:\Lacios.iso"
.\VirtualBoxVM.exe --startvm "Test"
cd "C:\Users\kecsk\OneDrive_sulid\OneDrive - Korm�nyzati Informatikai Fejleszt�si �gyn�ks�g\lacios\LaciOS 1.1"