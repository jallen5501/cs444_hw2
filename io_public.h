/*********************************************************************
*
*       file:           io_public.h
*       author:         betty o'neil
*
*       API for device-independent i/o package for SAPC
*       hw2: phase this out as user-callable API, replace with syscalls
*       (except ioinit() becomes a kernel-internal function)
*/

#ifndef IO_PUBLIC_H
#define IO_PUBLIC_H

#include "tty_public.h"

/* initialize io package*/
void ioinit(void);	/* read nchar bytes into buf from dev */
int sysread(int dev, char *buf, int nchar);	/* write nchar bytes from buf to dev */
int syswrite(int dev, char *buf, int nchar);

#endif
