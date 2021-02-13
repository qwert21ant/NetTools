#ifndef NBNS_H_INCLUDED
#define NBNS_H_INCLUDED

#include "../includes.h"

enum NBNSFL{
    AA = 64,
    TC = 32,
    RD = 16,
    RA = 8,
    B  = 1
};

#define SIZE_NBNS SIZE_UDP + sizeof(NBNS)

#pragma pack(1)
typedef struct NBNS{
    USHORT id;      ///уникальный id транзакции
    USHORT flags;   ///флаги: OPCODE NM_FLAGS RCODE
    USHORT qdcount; ///кол-во вопросов
    USHORT ancount; ///кол-во ответов
    USHORT nscount; ///кол-во записей ресурсов в разделе полномочий пакета службы имен (???)
    USHORT arcount; ///кол-во доп. записей

    BOOL r(){
        return (ntohs(flags) >> 15) & 1;
    }

    UCHAR opcode(){
        return (ntohs(flags) >> 11) & 15;
    }

    UCHAR nm_flags(){
        return (ntohs(flags) >> 7) & 127;
    }

    UCHAR rcode(){
        return ntohs(flags) & 15;
    }
} *PNBNS;
#pragma pop

#define NBNS_Q_NAME_LEN    0x21

#define NBNS_Q_TYPE_NB     0x2000
#define NBNS_Q_TYPE_NBSTAT 0x2100

#define NBNS_Q_CLASS_IN    0x0100

PUCHAR gotoNBNS(PUCHAR data){
    return data + SIZE_UDP;
}

BOOL codeNBName(PUCHAR name, int len, PUCHAR res){
    if(len > 0x10) return FALSE;
    *res = 0x20; res++;
    for(int i = 0; i < len; i++){
        res[i*2]   = (name[i] >> 4) + 'A';
        res[i*2+1] = (name[i] & 0xF) + 'A';
    }
    for(int i = len; i < 0xF; i++){
        res[i*2]   = (' ' >> 4) + 'A';
        res[i*2+1] = (' ' & 0xF) + 'A';
    }
    res[0xF * 2] = res[0xF * 2 + 1] = 'A';
    res[0x21] = 0x00;
    return TRUE;
}

BOOL decodeNBName(PUCHAR name, int len, PUCHAR res){
    if(name[0] != 0x20) return FALSE;

    name++;
    for(int i = 0; i < len / 2; i++){
        res[i] = ((name[i*2] - 'A') << 4) | ((name[i*2+1] - 'A') & 0xF);
    }
    res[len] = 0x00;

    return TRUE;
}

int makeNBNSQuestion(DEVADDR src, DEVADDR dst, PUCHAR buf){
    NBNS nbns;
    nbns.id = rand();
    nbns.flags = ntohs(0);
    nbns.qdcount = ntohs(1);
    nbns.ancount = 0;
    nbns.nscount = 0;
    nbns.arcount = 0;

    memcpy(gotoNBNS(buf), (PUCHAR) &nbns, sizeof(NBNS));

    UCHAR name[0x10];
    ZeroMemory(name, 0x10);
    name[0] = '*';

    codeNBName(name, 0x10, buf + SIZE_NBNS);

    int x = 0;
    while(buf[x + SIZE_NBNS]) x++;

    *((PUSHORT)(buf + x)) = NBNS_Q_TYPE_NBSTAT; x += 2;
    *((PUSHORT)(buf + x)) = NBNS_Q_CLASS_IN;

    makeUDP(src, dst, buf, x + 1);

    return SIZE_NBNS + NBNS_Q_NAME_LEN + 4;
}

#endif // NBNS_H_INCLUDED
