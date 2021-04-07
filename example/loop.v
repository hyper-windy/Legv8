.global main
.func main
   
main:
Loop: LSL X10, X22,#3
ADD X10, X10, X25
LDUR X9,[X10,#0]
SUB X11, X9, X24
CBNZ X11, Exit
ADDI X22, X22,#1
B Loop

Exit: