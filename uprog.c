/*********************************************************************
*
*       file:           testio.c
*       author:         betty o'neil
*
*       test program (application code) for 
*       the device-independent i/o package for SAPC
*
*       Modified by Ron Cheung on 9/2016 to have a bigger
*       DELAYLOOPCOUNT for faster VM
*/

#include <stdio.h>              /* for lib's device # defs, protos */
#include "io_public.h"		/* for our packages devs, API prototypes */
#include "tunistd.h"


#define MILLION 1000000
#define DELAYLOOPCOUNT (1000000000)
#define BUFLEN 80

void delay(void);
/* Note that kprintf is supplied with the SAPC support library.  It does 
  output polling to the console device with interrupts (temporarily) off.
  We are using it as a debugging tool while working with a development
  system, especially one using interrupts.  */

int main(void)
{
  char buf[BUFLEN];
  int got, i, lib_console_dev, ldev;

  /* Determine the SAPC's "console" device, the serial port for user i/o */
  lib_console_dev = sys_get_console_dev();  /* SAPC support lib fn */
  switch(lib_console_dev) {
  case COM1: ldev = TTY0;	/* convert to our dev #'s */
    break;
  case COM2: ldev = TTY1;
    break;
  default: printf("Unknown console device\n");
    return 1;
  }
  kprintf("Running with device TTY%d\n",ldev);
  /* Now have a usable device to talk to with i/o package-- */

  ioinit();  /* Initialize devices */
  kprintf("\nTrying simple write(4 chars)...\n");
  got = write(ldev,"hi!\n",4);
  kprintf("write of 4 returned %d\n",got); 
  delay();  /* time for output to finish, once write-behind is working */

  
  kprintf("Trying longer write (9 chars)\n"); 
  got = write(ldev, "abcdefghi", 9);
  kprintf("write of 9 returned %d\n",got);
  delay();  /* time for output to finish, once write-behind is working */

  for (i = 0; i < BUFLEN; i++)
      buf[i] = 'A'+ i/2;
  kprintf("\nTrying write of %d-char string...\n", BUFLEN);
  got = write(ldev, buf, BUFLEN);
  kprintf("\nwrite returned %d\n", got);
  delay();

  kprintf("\nType 10 chars input to test typeahead while looping for delay...\n");
  delay();
  got = read(ldev, buf, 10);	/* should wait for all 10 chars, once fixed */
  kprintf("\nGot %d chars into buf. Trying write of buf...\n", got);
  write(ldev, buf, got);

  kprintf("\nTrying another 10 chars read right away...\n");
  got = read(ldev, buf, 10);	/* should wait for input, once fixed */
  kprintf("\nGot %d chars on second read\n",got);
  if (got == 0) 
      kprintf("nothing in buffer\n");	/* expected result until fixed */
  else 
      write(ldev, buf, got);	/* should write 10 chars once fixed */

  kprintf("\n\nNow turning echo off--\n");
  control(ldev, ECHOCONTROL, 0);
  kprintf("\nType 20 chars input, note lack of echoes...\n");
  delay();
  got = read(ldev, buf, 20);
  kprintf("\nTrying write of buf...\n");
  write(ldev, buf, got);
  kprintf("\nAsked for 20 characters; got %d\n", got);
  return 0;
}

/* the faster the machine you're on, the longer this loop must be */
void delay()
{
  unsigned  long int i;

  kprintf("<doing delay>\n");
  for (i = 0; i <(DELAYLOOPCOUNT); i++)
   ;
   }
