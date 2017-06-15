John Allen 
Tanmoy Barua

We use a slightly modified version of Professor Cheung's solution to HW1. 

in our uprog.c: ttyread ttywrite

before:
int ttyread(int dev, char *buf, int nchar)
{
    .....
    baseport = devtab[dev].dvbaseport; /* hardware addr from devtab */
    tty = (struct tty *)devtab[dev].dvdata;   /* software data for line */
    while (i < nchar) { /* loop until we get user-specified # of chars */
        saved_eflags = get_eflags();
        cli();                  /* disable ints in CPU */
	.....
}
int ttywrite(int dev, char *buf, int nchar) {
    .....
    baseport = devtab[dev].dvbaseport; /* hardware addr from devtab */
    tty = (struct tty *)devtab[dev].dvdata;   /* software data for line */
    saved_eflags = get_eflags();
    cli();                      /* disable ints in CPU */
    ..... 
}

after:
int ttyread(int dev, char *buf, int nchar)
{
    .....
    baseport = devtab[dev].dvbaseport; /* hardware addr from devtab */
    tty = (struct tty *)devtab[dev].dvdata;   /* software data for line */
    sti();      /* CHANGE: JA */ 
    while (i < nchar) { /* loop until we get user-specified # of chars */
        saved_eflags = get_eflags();
        cli();                  /* disable ints in CPU */
        .....
}
int ttywrite(int dev, char *buf, int nchar) {
    .....
    baseport = devtab[dev].dvbaseport; /* hardware addr from devtab */
    tty = (struct tty *)devtab[dev].dvdata;   /* software data for line */
    sti();	/* CHANGE JA */
    saved_eflags = get_eflags();
    cli();                      /* disable ints in CPU */
    ..... 
}
