#ifndef UDP_H_INCLUDED
#define UDP_H_INCLUDED

#include "../includes.h"

#define SIZE_UDP SIZE_IP + sizeof(UDP)

#pragma pack(1)
typedef struct UDP{
    USHORT sport; ///порт отправителя
    USHORT dport; ///порт получателя
    USHORT len;   ///длина датаграммы (включая заголовок)
    USHORT crc;   ///контрольная сумма
} *PUDP;
#pragma pop

BOOL isUDP(PUCHAR data){
    if(!isIP(data)) return FALSE;
    return (getIP(data)->proto == IP_PROTO_UDP);
}

PUCHAR gotoUDP(PUCHAR data){
    return data + SIZE_IP;
}

PUDP getUDP(PUCHAR data){
    return (PUDP) gotoUDP(data);
}

void makeUDP(DEVADDR src, DEVADDR dst, PUCHAR buf, int nLen){
    UDP udp;
    udp.sport = ntohs(src.port);
    udp.dport = ntohs(dst.port);
    udp.len   = ntohs(sizeof(UDP) + nLen);
    udp.crc   = 0;

    memcpy(gotoUDP(buf), &udp, sizeof(UDP));

    udp.crc = ntohs(countCRC(gotoUDP(buf), sizeof(UDP) + nLen));

    memcpy(gotoUDP(buf) + offsetof(UDP, crc), (PUCHAR) &udp.crc, 2);

    makeIP(src, dst, IP_PROTO_UDP, buf, nLen + sizeof(UDP));
}

#endif // UDP_H_INCLUDED
