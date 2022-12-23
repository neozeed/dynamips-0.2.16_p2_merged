/*
 * A very simple SLiRP UDP redirect program
 * Copyright (c) 2015 Jason Stevens (jsteve@superglobalmegacorp.com)
 *
 * Redirects UDP into SLiRP, and sends SLiRP's output to a UDP host/port.
 * Just what you need to do usermode NAT in Dynamips.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>

#include "utils.h"
#include "net.h"
#include "../contrib/slirp/slirp.h"

#define MAX_PKT_SIZE  2048

FILE *log_file;	//needed by utils.
extern FILE *dfd;

#ifdef WIN32
void startwinsock(void)
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

/* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
    wVersionRequested = MAKEWORD(2, 0);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        /* Tell the user that we could not find a usable */
        /* Winsock DLL.                                  */
        printf("FATAL WinSock error!!\n");
        printf("WSAStartup failed with error: %d\n", err);
        exit(0);
    }
}
#endif

int slirp_inited=0;
int sck = -1;
unsigned long long data_sent=0LL;
unsigned long long data_recv=0LL;
void slirp_tic();
extern int dostats;

int main(int argc,char *argv[])
{
   struct in_addr myaddr; 
   char pkt[MAX_PKT_SIZE];
   ssize_t pkt_size;
   int rc=-1;
   u_long iMode=1;	//Set to a non-blocking socket.
   int skip=0;
   unsigned char kp;	//keypress
   int done=0;		//we aren-t done

dfd=stdout;
dostats=1;
log_file=stdout;
if(argc<4)
	{
	printf("Usage is listen_port remote_host remote_port\n");
	exit(0);
	}
#ifdef WIN32
	startwinsock();
#endif
	rc=slirp_init();
	if(rc==0)
		slirp_inited=1;
	else
		{printf("error with slirp, aborting!\n");exit(-1);}

	inet_aton("10.0.2.15",&myaddr);
	//YES THIS NEEDS TO PULL FROM A CONFIG FILE... but for now.
	rc=slirp_redir(0,42323,myaddr,23);


   /* Connect to remote port
   udp_connect(int local_port,char *remote_host,int remote_port)
    */
   if ((sck = udp_connect(atoi(argv[1]),argv[2],atoi(argv[3]))) < 0)
      exit(EXIT_FAILURE);

   ioctlsocket(sck,FIONBIO,&iMode);	//set to non-blocking

fflush(stdin);
fflush(stdout);
printf("SLiRP redirector started!\n");
printf("Press 's' for SLiRP stats\n");
printf("Press 'e' to exit.\n\n");
printf("Sent:\t\tRecv:\n",data_sent,data_recv);

while (!done)	{
      /* Receive packet and store it */
      if((pkt_size = recvfrom(sck,pkt,sizeof(pkt),0,NULL,NULL)) > 0)
	{
         slirp_input((unsigned char*)pkt,pkt_size);
	data_recv+=pkt_size;
	}
   skip++;
   if(skip>600) {
	slirp_tic();
	skip=0;
	Sleep(5);
	printf("%lld\t\t%lld\t\t\r",data_sent,data_recv);
	}
   if(_kbhit()) {
	kp=_getch();
	if(kp=='e')
		done=1;
	if(kp=='q')
		done=1;
	if(kp==27)
		done=1;
	if(kp=='s')
		{
		printf("stats!\n");
		printf_sockstats();
		printf("Sent:\t\tRecv:\n",data_sent,data_recv);
		}
	}
   //Sleep(1);
   }//end while(true)

   printf("\nExiting!\n");
   closesocket(sck);
   return(0);
}

void slirp_output(const uint8 *pkt, int pkt_len)
{
//printf("slirp_output %d bytes\n",pkt_len);
   if (send(sck,(const char*)pkt,pkt_len,0) < 0)
      exit(EXIT_FAILURE);
data_sent+=pkt_len;
}

int slirp_can_output(void)
{
return (slirp_inited);
}


// Instead of calling this and crashing some times
// or experencing jitter, this is called by the 
// 60Hz clock which seems to do the job.
void slirp_tic()
{
       int ret2,nfds;
        struct timeval tv;
        fd_set rfds, wfds, xfds;
        int timeout;
        nfds=-1;

      if(slirp_inited)
              {
        FD_ZERO(&rfds);
        FD_ZERO(&wfds);
        FD_ZERO(&xfds);
       timeout=slirp_select_fill(&nfds,&rfds,&wfds,&xfds); //this can crash

	if(timeout<0)
		timeout=500;
       tv.tv_sec=0;
       tv.tv_usec = timeout;    //basilisk default 10000

       ret2 = select(nfds + 1, &rfds, &wfds, &xfds, &tv);
        if(ret2>=0){
       slirp_select_poll(&rfds, &wfds, &xfds);
          }
	//printf("slirp_tic()\n");
       }//end if slirp inited
}
