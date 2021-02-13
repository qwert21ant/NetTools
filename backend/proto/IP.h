#ifndef IP_H_INCLUDED
#define IP_H_INCLUDED

#include "../includes.h"

#define SIZE_IP SIZE_ETH + sizeof(IP)

#pragma pack(1)
typedef struct IP{
    UCHAR  ver_hlen;
    UCHAR  tos;
    USHORT length;
    USHORT id;
    UCHAR  flags;
    UCHAR  offset;
    UCHAR  ttl;
    UCHAR  proto;
    USHORT crc;
    UINT   sip;
    UINT   dip;
} *PIP;
#pragma pop

#define IP_VER_HLEN  0x45

#define IP_TOS_NONE  0x00

#define IP_FLAGS_DF  0x40
#define IP_FLAGS_MF  0x20

#define IP_TTL_DEFAULT 128

#define IP_PROTO_TCP 0x06
#define IP_PROTO_UDP 0x11

BOOL isIP(PUCHAR data){
    return (getETH(data)->type == ETH_TYPE_IP);
}

PUCHAR gotoIP(PUCHAR data){
    return data + SIZE_ETH;
}

PIP getIP(PUCHAR data){
    return (PIP) gotoIP(data);
}

USHORT countCRC(PUCHAR buf, int szBuf){
    ULONG sum = 0;
    for(int i = 0; i < szBuf; i++) sum += *((PUSHORT) (buf) + i);

    while(sum >> 16) sum = (sum & 0xFFFF) + (sum >> 16);

    return ~((USHORT)sum);
}

void makeIP(DEVADDR src, DEVADDR dst, UINT proto, PUCHAR buf, int nLen){
    makeETH(src, dst, ETH_TYPE_IP, buf);
    IP ip;
    ip.ver_hlen = IP_VER_HLEN;
    ip.tos = IP_TOS_NONE;
    ip.length = ntohs(sizeof(IP) + nLen);
    ip.id = ntohs(rand());
    ip.flags = IP_FLAGS_DF;
    ip.offset = 0;
    ip.ttl = IP_TTL_DEFAULT;
    ip.proto = proto;
    ip.crc = 0;
    ip.sip = ntohl(src.ip);
    ip.dip = ntohl(dst.ip);

    memcpy(gotoIP(buf), (PUCHAR) &ip, sizeof(IP));
    ip.crc = ntohs(countCRC(gotoIP(buf), sizeof(IP) + nLen));
    memcpy(gotoIP(buf) + offsetof(IP, crc), (PUCHAR) &ip.crc, 2);
}

#endif // IP_H_INCLUDED
