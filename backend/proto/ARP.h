#ifndef ARP_H_INCLUDED
#define ARP_H_INCLUDED

#include "../includes.h"

#define ARP_HTYPE_ETHERNET 0x0100
#define ARP_PTYPE_IPv4     0x0008

#define ARP_HLEN_ETHERNET  0x06
#define ARP_PLEN_IPv4      0x04

#define ARP_OPER_REQUEST   0x0100
#define ARP_OPER_REPLY     0x0200

#pragma pack(1)
typedef struct ARP{
    USHORT htype;
    USHORT ptype;
    UCHAR  hlen;
    UCHAR  plen;
    USHORT oper;
    UCHAR  smac[6];
    UINT   sip;
    UCHAR  tmac[6];
    UINT   tip;
} *PARP;
#pragma pop

BOOL isARP(PUCHAR data){
    return (getETH(data)->type == ETH_TYPE_ARP);
}

PUCHAR gotoARP(PUCHAR data){
    return data + sizeof(ETH);
}

PARP getARP(PUCHAR data){
    return (PARP) gotoARP(data);
}

void makeARP(DEVADDR sender, DEVADDR target, UINT operation, PUCHAR buf){
    makeETH(sender, target, ETH_TYPE_ARP, buf);

    ARP arp;
    arp.htype = ARP_HTYPE_ETHERNET;
    arp.ptype = ARP_PTYPE_IPv4;
    arp.hlen  = ARP_HLEN_ETHERNET;
    arp.plen  = ARP_PLEN_IPv4;
    arp.oper  = operation;
    memcpy(arp.smac, sender.mac, 6);
    arp.sip   = ntohl(sender.ip);
    memcpy(arp.tmac, target.mac, 6);
    arp.tip   = ntohl(target.ip);

    memcpy(gotoARP(buf), (PUCHAR) &arp, sizeof(ARP));
}

void makeARPReq(DEVADDR sender, DEVADDR target, PUCHAR buf){
    makeARP(sender, target, ARP_OPER_REQUEST, buf);
    makeETH(sender, DEVADDR_BROADCAST, ETH_TYPE_ARP, buf);
}

#endif // ARP_H_INCLUDED
