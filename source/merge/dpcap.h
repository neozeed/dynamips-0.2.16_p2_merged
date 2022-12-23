#ifndef DPCAP_H
#define DPCAP_H
//#include <pcap.h>
#include "..\contrib\WpdPack\Include\pcap.h"

//Windows dynamic load pcap
//HINSTANCE hPcapLib = 0;                      /* handle to DLL */
extern HINSTANCE hPcapLib;
static char* Pcaplib_name = "wpcap.dll";
//why isnt this gettig pulled in?!
#ifndef PCAP_OPENFLAG_PROMISCUOUS
#define PCAP_OPENFLAG_PROMISCUOUS	1
#define PCAP_OPENFLAG_NOCAPTURE_RPCAP      4
#define PCAP_OPENFLAG_NOCAPTURE_LOCAL      8
#define PCAP_OPENFLAG_MAX_RESPONSIVENESS   16
#define PCAP_SRC_IF_STRING "rpcap://"
#endif

typedef pcap_t* (__cdecl * PCAP_OPEN_LIVE)(const char *, int, int, int, char *);
typedef int (__cdecl * PCAP_SENDPACKET)(pcap_t* handle, const u_char* msg, int len);
typedef int (__cdecl * PCAP_SETNONBLOCK)(pcap_t *, int, char *);
typedef const u_char*(__cdecl *PCAP_NEXT)(pcap_t *, struct pcap_pkthdr *);
typedef const char*(__cdecl *PCAP_LIB_VERSION)(void);
typedef void (__cdecl *PCAP_CLOSE)(pcap_t *);
typedef int  (__cdecl *PCAP_GETNONBLOCK)(pcap_t *p, char *errbuf);
typedef int (__cdecl *PCAP_COMPILE)(pcap_t *p, struct bpf_program *fp, const char *str, int optimize, bpf_u_int32 netmask);
typedef int (__cdecl *PCAP_SETFILTER)(pcap_t *p, struct bpf_program *fp);
typedef int (__cdecl *PCAP_DATALINK_NAME_TO_VAL)(const char *);
typedef pcap_t* (__cdecl *PCAP_OPEN_DEAD)(int, int);
typedef int (__cdecl *PCAP_SNAPSHOT)(pcap_t *);
typedef void (__cdecl *PCAP_DUMP)(u_char *, const struct pcap_pkthdr *, const u_char *);
typedef pcap_dumper_t * (__cdecl *PCAP_DUMP_OPEN)(pcap_t *, const char *);
typedef void (__cdecl *PCAP_DUMP_CLOSE)(pcap_dumper_t *);
typedef void (__cdecl *PCAP_DUMP_FLUSH)(pcap_dumper_t *);
typedef int (__cdecl *PCAP_SETDIRECTION)(pcap_t *, pcap_direction_t);
typedef int (__cdecl *PCAP_FINDALLDEVS)(pcap_if_t **, char *);
typedef void (__cdecl *PCAP_FREEALLDEVS)(pcap_if_t *);


extern PCAP_LIB_VERSION 	_pcap_lib_version;
extern PCAP_OPEN_LIVE		_pcap_open_live;
extern PCAP_SENDPACKET		_pcap_sendpacket;
extern PCAP_SETNONBLOCK	_pcap_setnonblock;
extern PCAP_NEXT		_pcap_next;
extern PCAP_CLOSE		_pcap_close;
extern PCAP_GETNONBLOCK	_pcap_getnonblock;
extern PCAP_COMPILE		_pcap_compile;
extern PCAP_SETFILTER		_pcap_setfilter;
extern PCAP_DATALINK_NAME_TO_VAL	_pcap_datalink_name_to_val;
extern PCAP_OPEN_DEAD		_pcap_open_dead;
extern PCAP_SNAPSHOT		_pcap_snapshot;
extern PCAP_DUMP		_pcap_dump;
extern PCAP_DUMP_OPEN		_pcap_dump_open;
extern PCAP_DUMP_CLOSE		_pcap_dump_close;
extern PCAP_DUMP_FLUSH		_pcap_dump_flush;
extern PCAP_SETDIRECTION	_pcap_setdirection;
extern PCAP_FINDALLDEVS	_pcap_findalldevs;
extern PCAP_FREEALLDEVS	_pcap_freealldevs;

//for net_io_filter
//yuck!
#define pcap_snapshot _pcap_snapshot
#define pcap_dump _pcap_dump
#define pcap_dump_flush _pcap_dump_flush
#define pcap_dump_close _pcap_dump_close
#define pcap_datalink_name_to_val _pcap_datalink_name_to_val
#define pcap_open_dead _pcap_open_dead
#define pcap_dump_open _pcap_dump_open
#define pcap_close _pcap_close
#define pcap_findalldevs _pcap_findalldevs
#define pcap_open _pcap_open
#define pcap_close _pcap_close
#define pcap_next _pcap_next
#define pcap_open_live _pcap_open_live
#define pcap_sendpacket _pcap_sendpacket
#define pcap_freealldevs _pcap_freealldevs
#define pcap_setdirection _pcap_setdirection
#endif