f=open("pci_companies.bin","rb")

data=f.read()
buf=[]
names=[]
values=[]
status=False
l=0
for i in range(len(data)):
    if status:
        if l==2 or i==len(data)-1:
            values.append(int.from_bytes(buf,"little"))
            status=False
            buf=[]
            l=0
            buf.append(data[i])
        else:
            l+=1
            buf.append(data[i])
    else:
        if data[i] != 0:
            buf.append(data[i])
        else:
            names.append(bytearray(buf).decode("utf-8"))
            status=True
            buf=[]

for i in range(len(names)):
    if values[i]==4660:
        print(names[i], "  ", values[i])