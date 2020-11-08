import requests
from bs4 import BeautifulSoup

sites=[
    "http://pcisig.com:80/membership/member-companies",
    "http://pcisig.com:80/membership/member-companies?page=1",
    "http://pcisig.com:80/membership/member-companies?page=2",
    "http://pcisig.com:80/membership/member-companies?page=3",
    "http://pcisig.com:80/membership/member-companies?page=4",
    "http://pcisig.com:80/membership/member-companies?page=5",
    "http://pcisig.com:80/membership/member-companies?page=6",
    "http://pcisig.com:80/membership/member-companies?page=7",
    "http://pcisig.com:80/membership/member-companies?page=8"
]

f=open("pci_companies.bin","xb")

for site in sites:
    wp=requests.get(site,verify=False).text
    soup=BeautifulSoup(wp)
    tbody=soup.find_all("tbody")[0]
    trs=tbody.find_all("tr")
    for tr in trs:
        name=tr.find_all("td")[0].find_all("a")[0].string
        tmp=tr.find_all("td")[1].string.split("(")[0][1:].replace(" ","")
        try:
            value=int(tmp)
        except:
            value=0
        
        for i in range(len(name)):
            if ord(name[i])>127:
                name=list(name)
                name[i]=" "
                name="".join(name)

        f.write(name.encode("ascii"))
        f.write(b'\x00')
        f.write(value.to_bytes(2,"little"))
        print(name, ": ", value)