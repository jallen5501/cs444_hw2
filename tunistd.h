#include "tty_public.h"

/* exit from program */
int exit(int exitcode);
/* read nchar bytes into buf from dev */
int read(int dev, char *buf, int nchar);
/* write nchar bytes from buf to dev */
int write(int dev, char *buf, int nchar);

int control(int dev, int fncode, int val);
