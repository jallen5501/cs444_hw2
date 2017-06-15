# CS444 hw2 makefile
# Usage: make U=test1   to build test1.lnx with tiny UNIX
#   also "make testio.lnx" to make hw1 solution

# system directories needed for compilers, libraries, header files--
# assumes the environment variables SAPC_TOOLS, SAPC_GNUBIN, and SAPC_SRC
# are set up, usually by the ulab module
PC_LIB = $(SAPC_TOOLS)/lib
PC_INC = $(SAPC_TOOLS)/include

WARNINGS =  -Wall -Wstrict-prototypes -Wmissing-prototypes \
		-Wno-uninitialized -Wshadow -pedantic \
		-D__USE_FIXED_PROTOTYPES__

PC_CC   = $(SAPC_GNUBIN)/i386-gcc
PC_CFLAGS = -g $(WARNINGS) -I$(PC_INC)
PC_AS   = $(SAPC_GNUBIN)/i386-as
PC_LD   = $(SAPC_GNUBIN)/i386-ld
PC_NM   = $(SAPC_GNUBIN)/i386-nm

# File suffixes:
# .c	C source (often useful both for UNIX host and SAPC)
# .s 	assembly language source
# .opc  relocatable machine code, initialized data, etc., for SA PC
# .lnx  executable image (bits as in memory), for SA PC (Linux a.out format)
# .syms text file of .lnx's symbols and their values (the "symbol table")
# Symbol file "syms"--for most recently built executable in directory

# PC executable--tell ld to start code at 0x1000e0, load special startup
# module, special PC C libraries--
# Code has 0x20 byte header, so use "go 100100" (hopefully easier to
# remember than 100020)
U = uprog

# for CS444 hw2
# Object files for tiny unix OS
# You may need to add to these lists, and/or put in appropriate .opc rules
#  in the later part of this file

IO_OFILES = io.opc tty.opc ioconf.opc queue/queue.opc
KERNEL_OFILES = startup0.opc startup1.opc tunix.opc sysentry.opc $(IO_OFILES)
USER_OFILES = crt0.opc $(U).opc ulib.opc

#hw2 executable, for user program U--
# note that startup0.opc needs to be first, so that "go 100100"
# executes it to start everything 
$(U).lnx: $(KERNEL_OFILES) $(USER_OFILES)
	$(PC_LD) -N -Ttext 1000e0 -o $(U).lnx \
	  $(KERNEL_OFILES) $(USER_OFILES) $(PC_LIB)/libc.a
	rm -f syms;$(PC_NM) -n $(U).lnx>$(U).syms;ln -s $(U).syms syms

#hw1 solution--
testio.lnx: testio.opc $(IO_OFILES)
	$(PC_LD) -N -Ttext 1000e0 -o testio.lnx \
		$(PC_LIB)/startup0.opc $(PC_LIB)/startup.opc \
		testio.opc $(IO_OFILES) $(PC_LIB)/libc.a
	rm -f syms;$(PC_NM) -n testio.lnx>$(C).syms;ln -s testio.syms syms

$(U).opc: $(U).c tunistd.h tty_public.h 
	$(PC_CC) $(PC_CFLAGS) -I$(PC_INC) -c -o $(U).opc $(U).c

io.opc: io.c ioconf.h
	$(PC_CC) $(PC_CFLAGS) -I$(PC_INC) -c -o io.opc io.c

ioconf.opc: ioconf.c ioconf.h
	$(PC_CC) $(PC_CFLAGS) -I$(PC_INC) -c -o ioconf.opc ioconf.c

tty.opc: tty.c tty.h tty_public.h
	$(PC_CC) $(PC_CFLAGS) -I$(PC_INC) -c -o tty.opc tty.c

queue.opc: queue/queue.c  queue/queue.h
	$(PC_CC) $(PC_CFLAGS) -I$(PC_INC) -c -o queue.opc queue/queue.c

tunix.opc: tunix.c
	$(PC_CC) $(PC_CFLAGS) -I$(PC_INC) -c -o tunix.opc  tunix.c

# Note we can use gcc to assemble for us--
startup0.opc: startup0.s
	$(PC_CC) -c -o startup0.opc startup0.s

startup1.opc: startup1.c
	$(PC_CC) $(PC_CFLAGS) -I$(PC_INC) -c -o startup1.opc startup1.c

sysentry.opc: sysentry.s
	$(PC_CC) -c -o sysentry.opc sysentry.s

crt0.opc: crt0.s
	$(PC_CC) -c -o crt0.opc crt0.s

ulib.opc: ulib.s
	$(PC_CC) -c -o ulib.opc ulib.s

testio.opc: testio.c io_public.h
	$(PC_CC) $(PC_CFLAGS) -I$(PC_INC) -c -o testio.opc  testio.c

clean:
	rm -f *.o *.opc *.lnx core 

# "make spotless" to remove (hopefully) everything except sources
#  use this after grading is done
spotless:
	rm -f *.o *.opc *syms *.lnx
