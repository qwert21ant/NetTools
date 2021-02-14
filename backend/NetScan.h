#ifndef NETSCAN_H_INCLUDED
#define NETSCAN_H_INCLUDED

#include "adapter.h"
#include "proto.h"

using namespace std;

class NetScan{
public:
    vector<DEVADDR> data;

private:
    Adapter *adp  = NULL;
    UINT scanTime = 0;
    BOOL isWork   = FALSE;
    BOOL isFind   = FALSE;
    BOOL isJoined = TRUE;

    BOOL fARPWatch = FALSE;

    thread thSend, thCapture;

    UINT sip = 0, dip = 0;

    void SendThread(){
        UCHAR buf[1024];
        DEVADDR temp = DEVADDR_NULL;
        makeARPReq(adp->address(), temp, buf);
        for(UINT ip = sip; ip <= dip; ip++){
            getARP(buf)->tip = ntohl(ip);
            adp->send(buf, sizeof(ETH) + sizeof(ARP));
        }
        this_thread::sleep_for(chrono::milliseconds(scanTime));
        isWork = FALSE;
    }

    void CaptureThread(){
        PUCHAR buf;
        UINT szBuf;

        while(isWork){
            szBuf = adp->recv(buf);

            if(!buf || !isARP(buf)) continue;

            PARP arp = getARP(buf);
            BOOL skip = FALSE;
            if(fARPWatch){
                for(int i = 0; i < data.size(); i++) if(data[i].ip == arp->sip){
                    skip = TRUE;
                    break;
                }
                if(skip) continue;
                data.push_back(DEVADDR(arp->smac, arp->sip));
            }else{
                if(arp->oper == ARP_OPER_REPLY || arp->tip == adp->address().ip){
                    for(int i = 0; i < data.size(); i++) if(data[i].ip == arp->sip){
                        skip = TRUE;
                        break;
                    }
                    if(skip) continue;
                    data.push_back(DEVADDR(arp->smac, arp->sip));
                }
            }
        }
    }

public:

    void setAdapter(Adapter *adapter){
        adp = adapter;
    }

    void setScanTime(UINT Time){
        scanTime = Time;
    }

    void switchARPWatch(){
        fARPWatch = (fARPWatch ? FALSE : TRUE);
    }

    void scan(UINT Sip, UINT Dip){
        sip = Sip;
        dip = Dip;

        if(!isJoined) join();
        isWork      = TRUE;
        thSend      = thread(SendThread, this);
        thCapture   = thread(CaptureThread, this);
    }

    void join(){
        isWork   = FALSE;
        isJoined = TRUE;
        thSend.join();
        thCapture.join();
    }
};

#endif // NETSCAN_H_INCLUDED
