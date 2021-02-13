#ifndef DEVICE_H_INCLUDED
#define DEVICE_H_INCLUDED

#include "includes.h"

struct DEVADDR{
    UCHAR  mac[6];
    UINT   ip;
    UINT   mask;
    USHORT port;

    DEVADDR(PUCHAR Mac, UINT Ip, UINT Mask = 0, UINT Port = 0){
        memcpy(mac, Mac, 6);
        ip = Ip;
        mask = Mask;
        port = Port;
    }

    DEVADDR(){
        mac[0] = mac[1] = mac[2] = \
        mac[3] = mac[4] = mac[5] = \
        ip = mask = port = 0;
    }
};

const DEVADDR DEVADDR_NULL =      DEVADDR((UCHAR[6]){0, 0, 0, 0, 0, 0}, 0);
const DEVADDR DEVADDR_BROADCAST = DEVADDR((UCHAR[6]){0xff, 0xff, 0xff, 0xff, 0xff, 0xff}, 0xffffffff);

struct DEVDESC{
    DEVADDR addr;
    CHAR    name[255];

    DEVDESC(DEVADDR Addr = DEVADDR_NULL, PCHAR Name = ""){
        addr = Addr;
        strcpy(name, Name);
    }
};

#endif // DEVICE_H_INCLUDED
