#include <stdio.h>
#include <cpu.h>
#include <gates.h>
#include <pic.h>
#include <serial.h>
#include "tsyscall.h"
#include "io_public.h"
#include "tunistd.h"


extern IntHandler syscall; /* the assembler envelope routine    */
extern void locate_idt(unsigned int *limitp, char ** idtp);
extern void finish(void);
void k_init(void);
void set_trap_gate(int n, IntHandler *inthand_addr); 
void debug_set_trap_gate(int n, IntHandler *inthand_addr, int debug);
int sysexit(void);
void syscallc(int user_eax, int devcode, char *buff , int bufflen);

void k_init(){
  ioinit();            /* ABSTRACTION of ioinit()*/
  set_trap_gate(0x80, &syscall);   /* SET THE TRAP GATE*/
}

void syscallc( int user_eax, int devcode, char *buff , int bufflen) {

  int trap_reference = user_eax;

  switch (trap_reference) {
    case SYS_exit :
      sysexit();
      break;
    case SYS_write :
      syswrite(devcode, buff, bufflen);
      break;
    case SYS_read :
      sysread(devcode, buff, bufflen);
      break;
    default : kprintf("eax value %d is not a reference to a valid trap.\n");
  }
}

void set_trap_gate(int n, IntHandler *inthand_addr) {
  debug_set_intr_gate(n, inthand_addr, 0);
}

void debug_set_trap_gate(int n, IntHandler *inthand_addr, int debug) {
  char *idt_LAaddr;		/* linear address of IDT */
  char *idt_VAaddr;		/* virtual address of IDT */
  Gate_descriptor *idt, *desc;
  unsigned int limit = 0;

  if (debug)
    printf("Calling locate_idt to do sidt instruction...\n");
  locate_idt(&limit,&idt_LAaddr);
  /* convert to virtual address, i.e., ordinary address */
  idt_VAaddr = idt_LAaddr - KERNEL_BASE_LA;  /* usable address of IDT */
  /* convert to a typed pointer, for an array of Gate_descriptor */
  idt = (Gate_descriptor *)idt_VAaddr;
  if (debug)
    printf("Found idt at %x, lim %x\n",idt, limit);
  desc = &idt[n];		/* select nth descriptor in idt table */
  /* fill in descriptor */
  if (debug)
    printf("Filling in desc at %x with addr %x\n",(unsigned int)desc,
	   (unsigned int)inthand_addr);
  desc->selector = KERNEL_CS;	/* CS seg selector for int. handler */
  desc->addr_hi = ((unsigned int)inthand_addr)>>16; /* CS seg offset of inthand  */
  desc->addr_lo = ((unsigned int)inthand_addr)&0xffff;
  desc->flags = GATE_P|GATE_DPL_KERNEL|GATE_TRAPGATE; /* valid, interrupt */
  desc->zero = 0;
}

int sysexit() {
  finish();
  return SYS_exit;
} 
