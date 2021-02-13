#ifndef RTP_H_INCLUDED
#define RTP_H_INCLUDED

#include "../includes.h"

#pragma pack(1)
typedef struct RTP{
    USHORT flags;
    USHORT id;
    UINT   time;
    UINT   ssrc;
} *PRTP;
#pragma pop

void getRTPData(PUCHAR rtp, PUCHAR data){
    PRTP _rtp = (PRTP) rtp;
}

#endif // RTP_H_INCLUDED
