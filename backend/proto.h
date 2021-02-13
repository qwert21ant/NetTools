#ifndef PROTO_H_INCLUDED
#define PROTO_H_INCLUDED

#include "proto/ETH.h"

#include "proto/ARP.h"
#include "proto/IP.h"

#include "proto/UDP.h"
#include "proto/TCP.h"

#include "proto/NBNS.h"
#include "proto/DNS.h"
#include "proto/RTP.h"

#define PROTO_IPv4 ETH_TYPE_IP
#define PROTO_IPv6 -1
#define PROTO_ARP  ETH_TYPE_ARP

#define PROTO_ICMP -1
#define PROTO_TCP  IP_PROTO_TCP
#define PROTO_UDP  IP_PROTO_UDP

#endif // PROTO_H_INCLUDED
