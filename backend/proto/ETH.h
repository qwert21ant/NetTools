#ifndef ETH_H_INCLUDED
#define ETH_H_INCLUDED

#include "../includes.h"

#define ETH_TYPE_IP  0x0008
#define ETH_TYPE_ARP 0x0608

#define SIZE_ETH sizeof(ETH)

#pragma pack(1)
typedef struct ETH{
    UCHAR  dmac[6];
    UCHAR  smac[6];
    USHORT type;
} *PETH;
#pragma pop

PETH getETH(PUCHAR data){
    return (PETH) data;
}

void makeETH(DEVADDR src, DEVADDR dst, UINT type, PUCHAR buf){
    memcpy(buf, dst.mac, 6);
    memcpy(buf + 6, src.mac, 6);
    memcpy(buf + 12, (PUCHAR) &type, 2);
}

#endif // ETH_H_INCLUDED
