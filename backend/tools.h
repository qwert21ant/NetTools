#ifndef TOOLS_H_INCLUDED
#define TOOLS_H_INCLUDED

#include "includes.h"
#include "device.h"

using namespace std;

void printIP(UINT ip){
    cout<<(ip >> 24 & 0xFF)<<"."<<(ip >> 16 & 0xFF)<<"."<<(ip >> 8 & 0xFF)<<"."<<(ip & 0xFF);
}

void printMAC(UCHAR* mac){
    printf("%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

void sprintIP(PCHAR buf, UINT ip){
    sprintf(buf, "%d.%d.%d.%d", ip & 0xFF, ip >> 8 & 0xFF, ip >> 16 & 0xFF, ip >> 24 & 0xFF);
}

void sprintMAC(PCHAR buf, UCHAR* mac){
    sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

BOOL getMAC(DEVADDR host, UINT ip, PUCHAR mac){
    ULONG n = 6;
    if(SendARP(ip, host.ip, (PULONG) mac, &n) == ERROR_SUCCESS){
        return TRUE;
    }
    return FALSE;
}

BOOL getMACfromTABLE(char* ip, PUCHAR mac){
    ULONG szTable = 1000;
    PMIB_IPNETTABLE table = (PMIB_IPNETTABLE) new char[szTable];
    if(GetIpNetTable(table, &szTable, TRUE) != ERROR_SUCCESS){
        delete [] (char*) table;
        table = (PMIB_IPNETTABLE) new char[szTable];
    }
    if(GetIpNetTable(table, &szTable, TRUE) != ERROR_SUCCESS){
        cout<<"FAIL get ARP table"<<endl;
        delete [] (char*) table;
        return FALSE;
    }

    UINT _ip = inet_addr(ip);

    for(int i = 0; i < table->dwNumEntries; i++){
        if(_ip == table->table[i].dwAddr){
            memcpy(mac, table->table[i].bPhysAddr, 6);
            return TRUE;
            break;
        }
    }
    return FALSE;
}

#endif // TOOLS_H_INCLUDED
