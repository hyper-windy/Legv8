/******************************************************************************
* @file compare_char.s
* @brief simple get keyboard character and compare
*
* Simple example of invoking syscall to retrieve a char from keyboard input,
* and testing to see if it is equal to a given value
*
* @author Christopher D. McMurrough
******************************************************************************/
 
.global main
.func main
   
main:
    BL  _prompt             @ branch to printf procedure with return
    BL  _getchar            @ branch to scanf procedure with return
    ADD R1, XZR, R0              @ ADDe return XZR, value R0 to argument register R1
    BL  _compare            @ check the scanf input
    B   _exit               @ branch to exit procedure with no return
   
_exit:  
    ADD R7, XZR, #4              @ write syscall, 4
    ADD R0, XZR, #1              @ output stream to monitor, 1
    ADD R2, XZR, #21             @ print string length
    LDR R1, =exit_str       @ string at label exit_str:
    SWI 0                   @ execute syscall
    ADD R7, XZR, #1              @ terminate syscall, 1
    SWI 0                   @ execute syscall
 
_prompt:
    ADD R7, XZR, #4              @ write syscall, 4
    ADD R0, XZR, #1              @ output stream to monitor, 1
    ADD R2, XZR, #23             @ print string length
    LDR R1, =prompt_str     @ string at label prompt_str:
    SWI 0                   @ execute syscall
    ADD PC, XZR, LR              @ return
   
_getchar:
    ADD R7, XZR, #3              @ write syscall, 3
    ADD R0, XZR, #0              @ input stream from monitor, 0
    ADD R2, XZR, #1              @ read a single character
    LDR R1, =read_char      @ store the character in data memory
    SWI 0                   @ execute the system call
    LDR R0, [R1]            @ ADDe the XZR, character to the return register
    AND R0, #0xFF           @ mask out all but the lowest 8 bits
    ADD PC, XZR, LR              @ return
 
_compare:
    CMP R1, #'@'            @ compare against the constant char '@'
    BEQ _correct            @ branch to equal handler
    BNE _incorrect          @ branch to not equal handler
 
_correct:
    ADD R5, XZR, LR              @ store LR since printf call overwrites
    LDR R0, =equal_str      @ R0 contains formatted string address
    BL printf               @ call printf
    ADD PC, XZR, R5              @ return
 
_incorrect:
    ADD R5, XZR, LR              @ store LR since printf call overwrites
    LDR R0, =nequal_str     @ R0 contains formatted string address
    BL printf               @ call printf
    ADD PC, XZR, R5              @ return
 
.data
read_char:      .asciz      " "
prompt_str:     .asciz      "Enter the @ character: "
equal_str:      .asciz      "CORRECT \n"
nequal_str:     .asciz      "INCORRECT: %c \n"
exit_str:       .asciz      "Terminating program.\n"