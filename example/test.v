.global main
.func main
   
main:
    ADDI R0, R0, #1             @ branch to printf procedure with return
    ADDI R1, R1, #2            @ branch to scanf procedure with return
    ADD R2, R0, R1              @ move return value R0 to argument register R1
    B def
abc:
    


def: 
