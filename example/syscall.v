.global main
.data
exit_str:       .asciz      "This is string data.\n"
.func main

main:
    ADDI X0, XZR, 8
    syscall
    ADDI X0, XZR, 4
    syscall
    LDUR, X1, exit_str
    ADDI X0, XZR, 4 
    syscall