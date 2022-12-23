/*This is from VLC

http://permalink.gmane.org/gmane.comp.video.videolan.vlc.devel/41052

*/
#ifndef _SEND_RECV_H
#define _SEND_RECV_H
#ifdef WIN32
#include <ws2tcpip.h>
#include <errno.h>


struct iovec
{
    void  *iov_base;
    size_t iov_len;
};

struct msghdr
{
    void         *msg_name;
    size_t        msg_namelen;
    struct iovec *msg_iov;
    size_t        msg_iovlen;
    void         *msg_control;
    size_t        msg_controllen;
    int           msg_flags;
};

size_t sendmsg ( int, struct msghdr *, int );
size_t recvmsg ( int, struct msghdr *, int );
#else
	//Non Win32 get this from somewhere else, and this is probably not needed at all
#endif	//End WIN32
#endif	//End _SEND_RECV_H