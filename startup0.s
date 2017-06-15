# asm startup file
# very first module in load
.globl _finish
  movl $0x3ffff0, %esp  # set initial kernel stack
  call _startupc         # call C startup
_finish:  int $3                #  return to Tutor
