#ifndef ICMP_H_INCLUDED
#define ICMP_H_INCLUDED

#include "../includes.h"

#define ICMP_TC_ECHO_REPLY   0x0000
#define ICMP_TC_ECHO_REQUEST 0x0008

#pragma pack(1)
typedef struct ICMP{
    USHORT type_code;
    USHORT crc;
} *PICMP;

typedef struct ICMP_ECHO{
    USHORT id;
    USHORT seq;
} *PICMP_ECHO;
#pragma pop

#endif // ICMP_H_INCLUDED
