/*This is from VLC

http://permalink.gmane.org/gmane.comp.video.videolan.vlc.devel/41052

*/
#include <winsock2.h>
#include <ws2tcpip.h>
#include <errno.h>

#if 0
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
#endif

#include "sendrecv.h"

#   ifndef IPV6_V6ONLY
#       define IPV6_V6ONLY 27
#   endif

size_t sendmsg (int s, struct msghdr *hdr, int flags)
{
size_t i;
    /* WSASendMsg would be more straightforward, and would support ancilliary
     * data, but it's not yet in mingw32. */
    if ((hdr->msg_iovlen > 100) || (hdr->msg_controllen > 0))
    {
        errno = EINVAL;
        return -1;
    }

    WSABUF buf[hdr->msg_iovlen];
    for (i = 0; i < sizeof (buf) / sizeof (buf[0]); i++)
        buf[i].buf = hdr->msg_iov[i].iov_base,
        buf[i].len = hdr->msg_iov[i].iov_len;

    DWORD sent;
    if (WSASendTo (s, buf, sizeof (buf) / sizeof (buf[0]), &sent, flags,
                   hdr->msg_name, hdr->msg_namelen, NULL, NULL) == 0)
        return sent;
    return -1;
}

size_t recvmsg (int s, struct msghdr *hdr, int flags)
{
size_t i;
    /* WSARecvMsg would be more straightforward, and would support ancilliary
     * data, but it's not yet in mingw32. */
    if (hdr->msg_iovlen > 100)
    {
        errno = EINVAL;
        return -1;
    }

    WSABUF buf[hdr->msg_iovlen];
    for (i = 0; i < sizeof (buf) / sizeof (buf[0]); i++)
        buf[i].buf = hdr->msg_iov[i].iov_base,
        buf[i].len = hdr->msg_iov[i].iov_len;

    DWORD recvd;
    hdr->msg_controllen = 0;
    hdr->msg_flags = 0;

    if (WSARecvFrom (s, buf, sizeof (buf) / sizeof (buf[0]), &recvd, flags,
                     hdr->msg_name, hdr->msg_namelen, NULL, NULL) == 0)
        return recvd;

#ifdef MSG_TRUNC
    if (WSAGetLastError() == WSAEMSGSIZE)
    {
        hdr->msg_flags |= MSG_TRUNC;
        return recvd;
    }
#else
# warning Out-of-date Winsock header files!
#endif
    return -1;
}