#ifndef TCP_H_INCLUDED
#define TCP_H_INCLUDED

#include "../includes.h"

enum TCPFL{
    URG = 5,
    ACK = 4,
    PSH = 3,
    RST = 2,
    SYN = 1,
    FIN = 0
};

#pragma pack(1)
typedef struct TCP{
    USHORT sport;
    USHORT dport;
    UINT   seq;
    UINT   ack;
    USHORT hlen_flags;
    USHORT window;
    USHORT crc;
    USHORT urgent;

    BOOL getFlag(TCPFL flag){
        return hlen_flags >> (8 + flag) & 1;
    }

    UINT getHLen(){
        return (hlen_flags >> 4 & 0xF) * 32;
    }
} *PTCP;
#pragma pop

BOOL isTCP(PUCHAR data){
    if(!isIP(data)) return FALSE;
    return (getIP(data)->proto == IP_PROTO_TCP);
}

PUCHAR gotoTCP(PUCHAR data){
    return data + SIZE_IP;
}

PTCP getTCP(PUCHAR data){
    return (PTCP) gotoTCP(data);
}

#endif // TCP_H_INCLUDED
