#ifndef _MULTIMEDIA_H
#define _MULTIMEDIA_H
#include "types.h"

typedef volatile struct{
    uint16 gcap;
    uint8 vmin;
    uint8 vmaj;
    uint16 outpay;
    uint16 inpay;
    uint32 gctl;
    uint16 wakeen;
    uint16 wakests;
    uint16 gsts;
    uint8 RESV[6];
    uint16 outstrmpay;
    uint16 instrmpay;
    uint32 RESV2;
    uint32 intctl;
    uint32 intsts;
    uint8 RESV3[8];
    uint32 walclk;
    uint32 RESV4;
    uint32 ssync;
    uint32 RESV5;
    uint32 corblbase;
    uint32 corbubase;
    uint16 corbwp;
    uint16 corbrp;
    uint8 corbctl;
    uint8 corbsts;
    uint8 corbsize;
    uint8 RESV6;
    uint32 icoi;
    uint32 icii;
    uint16 icis;
    uint8 RESV7[6];
    uint32 dpiblbase;
    uint32 dpibubase;
    uint16 RESV8;
} IntelHDAudioRegisterSet;

typedef volatile struct{
    IntelHDAudioRegisterSet* registers;
    IntelHDAudioStreamDescriptor* inputStreams;
    IntelHDAudioStreamDescriptor* outputStreams;
    IntelHDAudioStreamDescriptor* bidirectionalStreams;
} IntelHDAudio;

typedef volatile struct{
    uint16 ctl1;
    uint8 ctl2;
    uint8 sts;
    uint32 lpib;
    uint32 cbl;
    uint16 lvi;
    uint16 RESV;
    uint16 fifod;
    uint16 fmt;
    uint32 RESV2;
    uint32 bdpl;
    uint32 bdplu;
} IntelHDAudioStreamDescriptor;

void setUpAudioDevice(PCI_device* pcidevice);

typedef enum{
    IHDANumberOfOutputStreams,
    IHDANumberOfInputStreams,
    IHDANumberOfBidirectionalStreams,
    IHDANumberOfSerialDataOutSignals,
    IHDA64BitAddressSupported,
    IHDAOutputPayloadCapability,
    IHDAInputPayloadCapability,
    
} IntelHDAudioProperties;

#endif