#ifndef DNS_H_INCLUDED
#define DNS_H_INCLUDED

#include "../includes.h"

#pragma pack(1)
typedef struct DNS{
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
} *PDNS;
#pragma pop



#endif // DNS_H_INCLUDED
