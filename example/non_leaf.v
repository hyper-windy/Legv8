.global main
.func main

main:

fact:
SUBI SP,SP,#16
STUR LR,[SP,#8]
STUR X0,[SP,#0]         @ Save return address and n on stack        
SUBIS XZR,X0,#1         @ compare n and 1        
B.GE L1                 @ Else, set return value to 1
ADDI X1,XZR,#1          @ n = n - 1        
ADDI SP,SP,#16          @ if n >= 1, go to L1        
BR LR                   @ call fact(n-1)
L1: SUBI X0,X0,#1       @ Pop stack, don’t bother restoring values            
BL fact                 @ Return
LDUR X0,[SP,#0]         @ Restore caller’s n        
LDUR LR,[SP,#8]         @ Restore caller’s return address        
ADDI SP,SP,#16          @ Pop stack        
MUL X1,X0,X1            @ return n * fact(n-1)        
BR LR                   @ return





























