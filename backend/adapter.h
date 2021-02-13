#ifndef ADAPTER_H_INCLUDED
#define ADAPTER_H_INCLUDED

#include "includes.h"
#include "device.h"
#include "tools.h"

class Adapter{
private:
    PIP_ADAPTER_INFO pAdapterInfo;
    UINT _nAdapters = 0;

    pcap_t *dev = NULL;
    BOOL isOpened = FALSE;

    DEVADDR addr = DEVADDR_NULL;
    DEVADDR gway = DEVADDR_NULL;

public:
    UINT enumAdapters(){
        _nAdapters = 0;
        if(!pAdapterInfo) pAdapterInfo = (PIP_ADAPTER_INFO) new char[sizeof(IP_ADAPTER_INFO)];

        ULONG szBuf = sizeof(IP_ADAPTER_INFO);

        if(GetAdaptersInfo(pAdapterInfo, &szBuf) == ERROR_BUFFER_OVERFLOW){
            delete pAdapterInfo;
            pAdapterInfo = (PIP_ADAPTER_INFO) new char[szBuf];
        }

        if(GetAdaptersInfo(pAdapterInfo, &szBuf) != ERROR_SUCCESS){
            cerr<<"Error GetAdaptersInfo!"<<endl;
            delete [] (char*) pAdapterInfo;
            return -1;
        }

        PIP_ADAPTER_INFO pAdapter = pAdapterInfo;
        while(pAdapter){
            _nAdapters++;
            pAdapter = pAdapter->Next;
        }

        return 0;
    }

    UINT nAdapters(){
        return _nAdapters;
    }

    PIP_ADAPTER_INFO getAdapter(UINT ind){
        if(ind >= _nAdapters || ind < 0)
            return NULL;

        PIP_ADAPTER_INFO pAdapter = pAdapterInfo;
        for(int i = 0; i < ind; i++) pAdapter = pAdapter->Next;

        return pAdapter;
    }


    BOOL open(UINT ind){
        if(isOpened || ind >= _nAdapters || ind < 0)
            return FALSE;

        PIP_ADAPTER_INFO pAdapter;
        char errbuf[1000];
        char name[1000] = "";

        pAdapter = getAdapter(ind);

        sprintf(name, "\\Device\\NPF_%s", pAdapter->AdapterName);
        if(!(dev = pcap_open_live(name, 65535, 1, 1000, errbuf))){
            cerr<<"Error: pcap_open_live"<<endl;
            cerr<<errbuf<<endl;
            return FALSE;
        }

        addr.ip   = ntohl(inet_addr(pAdapter->IpAddressList.IpAddress.String));
        addr.mask = ntohl(inet_addr(pAdapter->IpAddressList.IpMask.String));
        memcpy(addr.mac, pAdapter->Address, 6);

        gway.ip   = ntohl(inet_addr(pAdapter->GatewayList.IpAddress.String));
        gway.mask = ntohl(inet_addr(pAdapter->GatewayList.IpMask.String));

        if(!getMACfromTABLE(pAdapter->GatewayList.IpAddress.String, gway.mac)){
            cerr<<"Cannot find gateway MAC in table!"<<endl;
            if(!getMAC(addr, inet_addr(pAdapter->GatewayList.IpAddress.String), gway.mac)){
                cerr<<"Cannot find gateway MAC!"<<endl;
            }
        }

        isOpened = TRUE;
        return TRUE;
    }

    BOOL isOpen(){
        return isOpened;
    }

    void close(){
        if(isOpened) pcap_close(dev);
        isOpened = FALSE;
    }

    DEVADDR address(){
        return addr;
    }

    DEVADDR gateway(){
        return gway;
    }

    BOOL send(PUCHAR buf, UINT szBuf){
        if(pcap_sendpacket(dev, buf, szBuf)){
            cerr<<"Send Error"<<endl;
            return FALSE;
        }
        return TRUE;
    }

    UINT recv(PUCHAR& buf){
        pcap_pkthdr hdr;
        buf = (PUCHAR) pcap_next(dev, &hdr);
        return hdr.len;
    }

    ~Adapter(){
        close();
        delete [] (char*) pAdapterInfo;
    }
};

#endif // ADAPTER_H_INCLUDED
