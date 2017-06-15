#crt01.s: startup main1
	
.globl _ustart, _main
.text
			
_ustart:	
	call _main                   #call main in the uprog.c
        pushl %eax                   #push the retval=exit_code on stack
        call _exit                   #call sysycall exit
