/* This file was taken from:
https://patchwork.ozlabs.org/patch/186158/

This patch is for Qemu, which is a GPL 2 program.

On Sat, Sep 22, 2012 at 8:26 PM, Stefan Weil <sw@weilnetz.de> wrote:
> Those functions are missing in MinGW.
>
> Some versions of MinGW-w64 include defines for gmtime_r and localtime_r.
> Older versions of these macros are buggy (they return a pointer to a
> static variable), therefore we don't want them. Newer versions are
> similar to the code used here, but without the memset.
>
> The implementation which is used here is not strictly reentrant,
> but sufficiently good for QEMU on w32 or w64.

For now, but nothing shows that there is a problem. Adding a few
simple locks shouldn't be difficult, or FIXME/XXX comment otherwise.
*/

#include <time.h>
struct tm *localtime_r(const time_t *timep, struct tm *result)
{
    struct tm *p = localtime(timep);
    memset(result, 0, sizeof(*result));
    if (p) {
        *result = *p;
        p = result;
   }
    return p;
}